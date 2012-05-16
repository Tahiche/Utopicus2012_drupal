<?php
// $Id$

/**
 * @file
 * Implements forms/callbacks for displaying orders info
 */

/**
 * Customer's Orders list page callback.
 */
function magento_my_orders_list($account) {
  drupal_set_title(t('My Orders'));

  if (!empty($account->customer['customer_id'])) {
    $customer_id = $account->customer['customer_id'];
    $orders =  magento_api_get_orders_list($customer_id);
  }

  if (empty($orders)) {
    return theme('magento_no_orders');
  }

  return theme('magento_my_orders_list', $account, $orders);
}

/**
 * Get number of orders per page.
 * Order list paginator helper
 *
 * @return integer
 *   Number of orders per page.
 */
function magento_my_orders_items_per_page() {
  return variable_get('magento_my_orders_per_page', 10);
}

/**
 * Get number of pages on order list
 * Order list paginator helper
 *
 * @param $orders_count
 *   Integer - total number of orders
 *
 * @return integer
 *   Number of pages.
 */
function magento_my_orders_number_of_pages($orders_count) {
  return ceil($orders_count/magento_my_orders_items_per_page());
}

/**
 * Customer's Order View page callback.
 */
function magento_my_orders_order_info($account, $order_id) {

  if (empty($order_id) || empty($account->customer['customer_id'])) {
    drupal_access_denied();
    return;
  }

  $customer_id = $account->customer['customer_id'];
  $order =  magento_api_get_order_info($order_id);
  //dd($order, 'magento_my_orders_order_info');

  if (empty($order['customer_id']) || $order['customer_id'] != $customer_id) {
    drupal_access_denied();
    return;
  }

  drupal_set_title(t('Order #{order_id} - {order_status}', array('{order_id}' => $order['increment_id'], '{order_status}' => $order['status'])));
    drupal_add_css(drupal_get_path('module', 'magento_my_orders') . '/magento_my_orders.css', 'module', 'all', FALSE);

  return theme('magento_order_info', $account, $order);
}

/**
 * Process variables for magento_my_orders_list.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $order: array of order data
 * - $account: user account object
 *
 * @see magento_my_orders_list.tpl.php
 */
function template_preprocess_magento_my_orders_list(&$variables) {
  // Two global variables needed by the pager.
  // Taken from pager_query() in pager.inc
  global $pager_page_array, $pager_total;

  // Grab the 'page' query parameter.
  // Taken from pager_query() in pager.inc
  $page = isset($_GET['page']) ? $_GET['page'] : '';

  // Convert comma-separated $page to an array, used by other functions.
  // Taken from pager_query() in pager.inc
  $pager_page_array = explode(',', $page);

  // Generate the data for page the requested and add it to the output.
  $orders = $variables['orders'];
  $account = $variables['account'];

  $total_counts = count($orders);

  $start_offset = magento_my_orders_items_per_page() * $pager_page_array[0];
  $orders = array_slice($orders, $start_offset, magento_my_orders_items_per_page());

  $headers = array(
    t('Orders #'),
    t('Date'),
    t('Ship To'),
    t('Order Total'),
    t('Order Status'),
    '&nbsp;',
  );
  $rows = array();

  foreach ($orders as $order) {
    $rows[] = array(
      'increment_id' => $order['increment_id'],
      'created_at' => format_date(strtotime($order['created_at'])),
      'shipping_name' => $order['shipping_name'],
      'grand_total' => theme('magento_product_format_price', $order['grand_total']),
      'status' => t($order['status']),
      'view'   => l(t('View order'), 'user/' . $account->uid . '/order/' . $order['increment_id']),
    );
  }

  // Put some magic in the two global variables
  // Based on code in pager_query() in pager.inc
  $pager_total[0] = magento_my_orders_number_of_pages($total_counts);
  $pager_page_array[0] =
    max(0, min(
      (int)$pager_page_array[0],
      ((int)$pager_total[0]) - 1)
    );

  $variables['orders_list'] = theme('table', $headers, $rows, $attributes = array('class' => 'orders-list'), $caption = '');
  $variables['orders_total_counts'] = t('%count orders', array('%count' => $total_counts));

  // Add the pager to the output.
  $variables['pager'] = theme('pager', NULL, magento_my_orders_items_per_page(), 0, array(), 9);

}

/**
 * Process variables for magento_order_info.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $order: array of order data
 * - $account: user account object
 *
 * @see magento_order_info.tpl.php
 */
function template_preprocess_magento_order_info(&$variables) {
  $order = $variables['order'];

  $headers = array(
    t('Product Name'),
    t('SKU'),
    t('Price'),
    t('Qty'),
    t('Subtotal'),
  );
  $rows = array();

  foreach ($order['items'] as $item) {
    $rows[] = array(
      'name' => $item['name'],
      'sku' => $item['sku'],
      'price' => $item['price'],
      'qty' => $item['qty_ordered'],
      'subtotal' => theme('magento_product_format_price', $item['row_total']),
    );
  }

  //////////////////
  //add summary rows
  $rows[] = array(
    'data' => array(
      'label' => array('data' => t('Subtotal'), 'colspan' => '4', 'align' => 'right'),
      'subtotal' => theme('magento_product_format_price', $order['subtotal']),
    ),
    'class' => 'subtotal',
  );
  $rows[] = array(
    'data' => array(
      'label' => array('data' => t('Shipping & Handling'), 'colspan' => '4', 'align' => 'right'),
      'shipping_handling' => theme('magento_product_format_price', $order['shipping_incl_tax']),
    ),
    'class' => 'shipping_handling',
  );
  if (!empty($order['discount_amount'])&& (floatval($order['discount_amount']) > 0.001) ) {
    $rows[] = array(
      'data' => array(
        'label' => array('data' => t('Discount'), 'colspan' => '4', 'align' => 'right'),
        'discount' => theme('magento_product_format_price', $order['discount_amount']),
      ),
      'class' => 'discount',
    );
  }
  $rows[] = array(
    'data' => array(
      'label' => array('data' => t('Grand total'), 'colspan' => '4', 'align' => 'right'),
      'grand_total' => theme('magento_product_format_price', $order['grand_total']),
    ),
    'class' => 'grand_total',
  );

  //payment_method
  $payment_method = empty($order['payment']['method']) ? '' : $order['payment']['method'];
  $payment_method = function_exists($payment_method  . '_register_payment_system') ? call_user_func($payment_method  . '_register_payment_system') : $payment_method;

  $variables['order_date'] = format_date(strtotime($order['created_at']));
  $variables['order_shipping_method'] = $order['shipping_description'];
  $variables['order_payment_method'] = $payment_method;
  $variables['order_shipping_address'] = theme('magento_my_orders_address', $order['shipping_address']);
  $variables['order_billing_address'] = theme('magento_my_orders_address', $order['billing_address']);
  $variables['items_ordered'] = theme('table', $headers, $rows, $attributes = array('class' => 'items-ordered'), $caption = t('Items Ordered'));

}

/**
 * Process variables for magento_my_orders_address.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $address: array of address data
 *
 * @see magento_my_orders_address.tpl.php
 */
function template_preprocess_magento_my_orders_address(&$variables) {
  module_load_include('inc', 'magento_customers', 'magento_customer.addresses');

  $address = $variables['address'];

  $countries = magento_customer_address_country_list();

  $address_data = array();
  $address_data['fio'] = $address['firstname'] . (empty($address['middlename']) ? ' ' : ' ' . $address['middlename'] . ' ') . $address['lastname'];
  $address_data['street'] = trim($address['street']);
  $address_data['city'] = $address['city'] . (empty($address['region']) ? '' : ', ' . $address['region'] ) . ' ' . $address['postcode'];
  $address_data['country'] = empty($countries[$address['country_id']]) ? '' : $countries[$address['country_id']];
  if (!empty($address['telephone'])) {
    $address_data['telephone'] = $address['telephone'];
  }

  $variables['address_block'] = join(',<br/>', $address_data);
}

/**
 * Get list of Customer's Orders in Magento
 *
 * @param $customer_id
 *   Customer ID
 *
 * @return
 *   Array of orders data
 */
function magento_api_get_orders_list($customer_id) {
  // hook before
  magento_invoke('magento_api', 'before get_orders_list', $customer_id);

  // Use general functions
  $result = _magento_api_call('order.list', array(array('customer_id' => $customer_id)));
  //$result = _magento_api_call('order.list', array());//array('customer_id' => $customer_id)

  // hook after
  magento_invoke('magento_api', 'after get_orders_list', $result);
  return $result;
}

/**
 * Get Order information from Magento
 *
 * @param $customer_id
 *   Customer ID
 *
 * @return
 *   Array of orders data
 */
function magento_api_get_order_info($order_id) {
  // hook before
  magento_invoke('magento_api', 'before get_order_info', $customer_id);

  // Use general functions
  $result = _magento_api_call('order.info', array($order_id));

  // hook after
  magento_invoke('magento_api', 'after get_order_info', $result);
  return $result;
}

/**
 * `magento_no_orders` theme implementation
 */
function theme_magento_no_orders() {
  drupal_set_message(t("You have no past orders"));
  return '';
}
