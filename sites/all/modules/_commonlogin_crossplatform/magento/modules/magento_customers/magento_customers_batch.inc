<?php
// $Id$
/**
 * @file
 * Implements batch operations
 */
/**
 * Synchronize customers from Magento into Drupal
 *
 */
function magento_customers_fetch_batch_create(&$params) {
  $customers = $params['customers'];
  $ready_count = $params['current'] * variable_get('magento_customers_sync_page_size_to_drupal', 25);
  if ($ready_count >= variable_get('magento_customers_sync_limit', 1000)) {
      $params['message'] = t("Limit of customers count for synchronization is reached (%limit)", array('%limit' => variable_get('magento_customers_sync_limit', 1000)));
      $params['finished'] = 1;
      return;
  }
  if (is_array($customers)) {
    if (0 == count($customers)) {
      $params['finished'] = 1;
      return;
    }
    $success = array();
    $errors = array();
    foreach ($customers as $customer) {
      $addresses = array();
      if (!empty($customer['addresses']) && is_array($customer['addresses'])) {
        foreach ($customer['addresses'] as $address_id) {
          $addresses[$address_id] = magento_api_customer_get_address_info($address_id);
        }
      }
      $customer['addresses'] = $addresses;

      if (magento_customers_synchronize_customer($customer)) {
        $success[] = $customer['email'];

      }
      else {
        $errors[] = $customer['email'];
      }
    }

    if (!isset($params['current_total'])) {
      $params['current_total'] = 0;
    }
    if (!isset($params['current_errors'])) {
      $params['current_errors'] = 0;
    }
    if (!isset($params['current_success'])) {
      $params['current_success'] = 0;
    }

    $params['current_total'] += count($customers);
    $params['current_errors'] += count($errors);
    $params['current_success'] += count($success);

    $params['message'] = t("Synchronized %total_cnt of %total customers (%success_cnt success, %error_cnt errors). Last customer updated: '%customer'", array('%customer' => $customer['email'], '%total_cnt' => $params['current_total'], '%success_cnt' => $params['current_total'], '%error_cnt' => $params['current_errors'], '%total' => $params['total']));
  }

  $params['current'] += 1;
  $params['finished'] = $params['steps'] > 0 ? (float) $params['current'] / ((int)$params['steps']) : 1;
}

/**
 *
 * On start we don't know how many customers we will get
 */
function magento_customers_fetch_batch_get(&$params) {
  if (!$params['finished']) {

    if (empty($params['current'])) {
      $params['current'] = 0;
    }
  }

  // get new portion of customers
  $website_id = magento_customers_get_current_website();

  //$filters = array('website_id' => array('eq' => $website_id));
  $filters = array();

  $params['customers'] = array();

  //TODO: ADD `updated only` filter by
  //variable_get('magento_customers_last_sync_date', 0);

  // try to get the number of Customers we will have
  $params['total'] = $count = magento_api_customer_get_customer_count($filters);
  $params['page_size'] = variable_get('magento_customers_sync_page_size_to_drupal', 25);
  $params['steps'] = ceil($count / $params['page_size']);

  $data = magento_api_customer_get_customer_list($filters, $params['current']+1, $params['page_size']);

  if (is_array($data)) {
    $params['customers'] = $data;
  }

  /*
  if (is_array($data) && !empty($data['items'])) {
    foreach ($data['items'] as $customer) {
      $params['customers'][] = $customer;
    }
  }
  */
}

/**
 * Send customers to Magento
 *
 */
function magento_customers_send_batch_create(&$params) {
  $customers = $params['customers'];
  $ready_count = $params['current'] * variable_get('magento_customers_sync_page_size_to_magento', 25);
  if ($ready_count >= variable_get('magento_customers_sync_limit', 1000)) {
      $params['message'] = t("Limit of customers count for synchronization is reached (%limit)", array('%limit' => variable_get('magento_customers_sync_limit', 1000)));
      $params['finished'] = 1;
      return;
  }

  if ($params['skip'] === TRUE) {
    $params['message'] = t("Full current portion of Drupal users is not good for synchronization. (%current of %steps steps)", array('%current' => $params['current'], '%steps' => $params['steps']));
    $params['current'] += 1;
    if (empty($params['steps'])) {
      $params['finished'] = 1;
      return;
    }
    $params['finished'] = (float) $params['current'] / ((int)$params['steps']);
    return;
  }
  if (is_array($customers)) {
    if (0 == count($customers)) {
      $params['finished'] = 1;
      return;
    }
    $result = magento_api_customer_send_customer($customers);
    if (is_array($result)) {
      $processed_result = _magento_customers_process_sync_result($result);
      if ($processed_result['global_status'] == TRUE) {
        $params['message'] = t("Successfully synchronized next %total_cnt of %total customers. (%current of %steps steps)", array('%total_cnt' => count($customers), '%current' => $params['current'], '%steps' => $params['steps'], '%total' => $params['total']));
      }
      else {
        watchdog('magento_customers', 'Errors occurs while customers synchronization to Magento. Data: %result', array('%result' => serialize($result)), WATCHDOG_WARNING);
        $params['message'] = t("Synchronized next %total_cnt customers of %total customers. (%success_cnt success, %error_cnt errors). (%current of %steps steps)", array('%total_cnt' => count($customers), '%success_cnt' => $processed_result['success'], '%error_cnt' => $processed_result['error'], '%current' => $params['current'], '%steps' => $params['steps'], '%total' => $params['total']));
      }
    }
    else {
      $params['message'] = t("Bad response occurs while current portions of customers synchronized to Magento. (%current of %steps steps)", array('%current' => $params['current'], '%steps' => $params['steps']));
      watchdog('magento_customers', 'Bad response occurs while customers synchronization to Magento. Response: %response', array('%response' => serialize($result)), WATCHDOG_WARNING);
    }
  }
  $params['current'] += 1;
  $params['finished'] = (float) $params['current'] / ((int)$params['steps']);
}

/**
 * magento_customers_send_batch_get
 * Get portion of customers to send to Magento
 */
function magento_customers_send_batch_get(&$params) {
  if (!$params['finished']) {

    if (!$params['current']) {
      $params['current'] = 0;
    }
  }

  // get new portion of customers
  $website_id = magento_customers_get_current_website();

  $filters = array('website_id' => array('eq' => $website_id));

  $params['customers'] = array();
  // try to get the number of Customers we will have

  //TODO: ADD `updated only` filter by
  $date = variable_get('magento_customers_last_sync_date_to_magento', 0);
  //$date = time() - (30 * 24 * 60 * 60);

  $count = _magento_customers_get_users_count($date);
  $page_size = variable_get('magento_customers_sync_page_size_to_magento', 25);
  $pages_count = ceil($count / $page_size);

  $params['steps'] = $pages_count;
  $offset = $params['current'] * $page_size;
  $data = _magento_customers_get_users_list($date, $offset, $page_size);
  $params['skip'] = FALSE;
  if (is_array($data)) {
    $params['customers'] = $data;
    if (count($data) === 0) {
      //all Drupal users from current portion are not good for sync, skip this step
      $params['skip'] = TRUE;
    }
  }
  else {
    $params['finished'] = 1;
    $params['message'] = t('Error while loading Drupal users.');
  }
}