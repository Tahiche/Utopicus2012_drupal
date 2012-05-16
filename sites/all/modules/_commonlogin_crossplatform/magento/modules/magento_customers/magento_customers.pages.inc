<?php
// $Id$
/**
 * @file
 * Implements forms/callbacks for managing customers info
 */
/**
 * Page builder; Present the page to Manage customers's info.
 */
function magento_customers_customer_info($account, $category = 'account') {
  drupal_set_title(check_plain($account->name) . ': ' . t("Magento Customer Info"));
  return drupal_get_form('magento_customers_customer_info_form', $account);
}

/**
 * Generate form for add/edit user adress
 */
function magento_customers_customer_info_form($form_state, $account = NULL) {
  $form = array();

  _magento_customers_customer_info_form_builder($form, $account);

  $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

  #$form['#submit'][] = 'magento_customers_customer_info_form_submit';
  #$form['#validate'][] = 'magento_customers_customer_info_form_validate';
  return $form;
}
/**
 * Helper for magento_customers_customer_info_form builder
 */
function _magento_customers_customer_info_form_builder(&$form, $account) {
  $_defaults = array(
      'prefix' => '',
      'firstname' => '',
      'lastname' => '',
      'middlename' => '',
      'suffix' => '',
      'dob' => strtotime("now"),
      'taxvat' => '',
      'gender' => '1',
    );

  $account_info = $_defaults;
  if (!empty($account->customer) && is_array($account->customer)) {
    $account_info = array_merge($_defaults, $account->customer);
  }

  if (!empty($account->uid)) {
    $form['user_uid'] = array(
      '#type' => 'value',
      '#value' => $account->uid,
    );
  }

  $form['customer'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Customer Information'),
  );
  $form['customer']['prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => $account_info['prefix'],
  );

  $form['customer']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#default_value' => $account_info['firstname'],
  );

  $form['customer']['middlename'] = array(
    '#type' => 'textfield',
    '#title' => t('Middle Name/Initial'),
    '#default_value' => $account_info['middlename'],
  );

  $form['customer']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
    '#default_value' => $account_info['lastname'],
  );

  $form['customer']['suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => $account_info['suffix'],
  );

  if (!is_numeric($account_info['dob'])) {
    $account_info['dob'] = strtotime($account_info['dob']);
  }

  $account_info['dob'] = explode(':', date('d:m:Y', $account_info['dob']));

  if (count($account_info['dob']) == 3) {
    $account_info['dob'] = array(
      'day' => intval($account_info['dob']['0']),
      'month' => intval($account_info['dob']['1']),
      'year' => $account_info['dob']['2'],
      );
  }
  $form['customer']['dob'] = array(
    '#type' => 'date',
    '#title' => t('Date Of Birth'),
    '#required' => FALSE,
    '#default_value' => $account_info['dob'],
  );

  $form['customer']['taxvat'] = array(
    '#type' => 'textfield',
    '#title' => t('Tax/VAT Number'),
    '#default_value' => $account_info['taxvat'],
  );

  $options = array(
    '1' => 'Male',
    '2' => 'Female',
  );
  $form['customer']['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#options' => $options,
    '#default_value' => $account_info['gender'],
  );
}

/**
 * Validate magento customer's account info
 */
function magento_customers_customer_info_form_validate($form, &$form_state) {
  if (empty($form_state['values']['customer']['dob'])) {
    $form_state['values']['customer']['dob'] = '';
    return;
  }

  $dob = $form_state['values']['customer']['dob'];

  if (!empty($dob['year']) && !empty($dob['month']) && !empty($dob['day'])) {
    $bitrhday = strtotime($dob['day'] . '-'. $dob['month'] .'-'. $dob['year'])+3600*11;

    $min_age = variable_get( 'magento_customers_min_age', 0);

    if (!_magento_customers_validate_age($bitrhday, $min_age) ) {
      form_set_error("customer']['dob", t('You have to be over %min_age years old.', array('%min_age' => $min_age)));
      return;
    }

    $form_state['values']['customer']['dob'] = $bitrhday;
  }
  else {
    $form_state['values']['customer']['dob'] = '';
  }
}
/**
 *  Save magento customer's account info
 */
function magento_customers_customer_info_form_submit($form, &$form_state) {
  $account_info = $form_state['values']['customer'];

  if (!empty($form_state['values']['user_uid'])) {
    // update account
    $account = user_load($form_state['values']['user_uid']);
    if (!is_null($account->customer)) {
      $profile = $account->customer;
      $profile = array_merge($profile, $account_info);
    }
    else {
      $profile = $account_info;
    }

    $data = array( 'customer' => $profile );
    user_save($account, $data);
  }

  drupal_set_message(t('Customer information was saved.'));
  drupal_goto('user/' . $account->uid);
}

/**
 * Validate if user is over 18 years old
 */
function _magento_customers_validate_age($birthday, $age = 18) {

  // $birthday can be UNIX_TIMESTAMP or just a string-date.
  if (is_string($birthday)) {
    $birthday = strtotime($birthday);
  }

  // check
  // 31536000 is the number of seconds in a 365 days year.
  if (time() - $birthday < $age * 31536000) {
    return FALSE;
  }

  return TRUE;
}