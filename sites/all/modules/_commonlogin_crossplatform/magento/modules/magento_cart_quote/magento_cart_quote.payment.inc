<?php
// $Id$
/**
 * @file
 * Implements payment hooks, cart payment forms, payment helpers
 */

//Used watchdog msg LOG FORMAT:
// CART {operation}: uid: {uid}, {dump}

/**
 * Order states
 *
    const STATE_NEW             = 'new';
    const STATE_PENDING_PAYMENT = 'pending_payment';
    const STATE_PROCESSING      = 'processing';
    const STATE_COMPLETE        = 'complete';
    const STATE_CLOSED          = 'closed';
    const STATE_CANCELED        = 'canceled';
    const STATE_HOLDED          = 'holded';
 */
    define('MAGENTO_CART_ORDER_STATE_NEW',              'new');
    define('MAGENTO_CART_ORDER_STATE_PENDING_PAYMENT',  'pending_payment');
    define('MAGENTO_CART_ORDER_STATE_PROCESSING',       'processing');
    define('MAGENTO_CART_ORDER_STATE_COMPLETE',         'complete');
    define('MAGENTO_CART_ORDER_STATE_CLOSED',           'closed');
    define('MAGENTO_CART_ORDER_STATE_CANCELED',         'canceled');
    define('MAGENTO_CART_ORDER_STATE_HOLDED',           'holded');

/////////////////// DRUPAL HOOKS ///////////////////////////////////////
/**
 * Implementation of hook_domain_conf_form_alter ( where domain_conf_form is form_id )
 * Adds option to select avaiable payment systems for each domain
 */
function magento_cart_quote_form_domain_conf_form_alter(&$form) {
  $form['paymethods'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment system'),
    '#collapsible' => TRUE,
  );

  $domain_id = empty($form['domain_id']['#value']) ? 0 : intval($form['domain_id']['#value']);

  $form['paymethods']['avaiable_payment_methods'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled payment system'),
    '#required' => FALSE,
    '#options' => _magento_cart_quote_payment_registered_methods(),
    '#default_value' => _magento_cart_quote_payment_enabled_methods($domain_id),
    '#description' => t('Only selected payment systems will be available for this domain.')
  );

  $form['#submit'][]='magento_cart_quote_form_domain_conf_form_submit';
}

/**
 * Implementation of hook_domain_conf_form_alter ( where domain_conf_form is form_id )
 */
function magento_cart_quote_form_domain_conf_form_submit(&$form, &$form_state) {
  if (empty($form_state['values']['avaiable_payment_methods'])) {
    return;
  }

  $enabled_paymethods = array();
  foreach ($form_state['values']['avaiable_payment_methods'] as $paymethod => $enabled) {
    if ($enabled) {
      $enabled_paymethods[]=$paymethod;
    }
  }

  $domain_id = intval($form_state['values']['domain_id']);

  variable_set('enabled_paymethods_' . $domain_id, serialize($enabled_paymethods));
}

/////////////////// SETTINGS FORM //////////////////////////////////////
/**
 * Generate settings form
 */
function magento_cart_quote_payment_settings_form(&$form_state) {
  $form = array();

  magento_cart_quote_form_domain_conf_form_alter($form);

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );
  return $form;
}

/**
 * Submit function for settings form
 */
function magento_cart_quote_payment_settings_form_submit($form, &$form_state) {
  magento_cart_quote_form_domain_conf_form_submit($form, $form_state);
  drupal_set_message(t('Configuration saved.'));
}

/////////////////// CART PAYMENT PAGE //////////////////////////////////
/**
 * Payment page
 */
function magento_cart_quote_magento_cart_payment() {
  drupal_add_css(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.css', 'module', 'all', FALSE);

  $cart = magento_cart_get();
  $output = drupal_get_form('magento_cart_quote_payment_form', $cart);

  return theme('magento_cart_quote_content_wrapper', $output);
}

/**
 * Payment form
 */
function magento_cart_quote_payment_form($form_state, $cart) {
  global $_domain;
  $domain_id = intval($_domain['domain_id']);

  // check that current quote is valid else goto delivery form
  /*
  if (empty($cart['quote']['id'])&&empty($form_state['ajaxupdate'])) {
    drupal_set_message(t("Quote is invalid. Can't process to payment.") . (empty($cart['errors'])?'':join('', $cart['errors'])));
    magento_cart_update_state('delivery');
    drupal_goto('cart/delivery');
    exit();
  }
  */

  $form = array();

  $paymethods = magento_cart_quote_payment_available_methods();
  $sel_paymethod = (empty($cart['custom_data']['payment_module']) ? '' : $cart['custom_data']['payment_module']);

  if (!empty($paymethods)) {
    $form['magento_cart_quote_payment_methods'] = array(
      '#type' => 'radios',
      '#options' => $paymethods,
      '#default_value' => $sel_paymethod,
    );

    _magento_cart_quote_payment_options($form);

    $content_wrapper = magento_cart_quote_content_wrapper_id();

    $form['payment_mode'] = array(
       '#type' => 'hidden',
       '#default_value' => $sel_paymethod,
       '#ahah' => array(
          'event' => 'change',
          'path'  => "cart/payment_update",
          'wrapper' => $content_wrapper,//'content-area',
          'method' => 'replace',
          'effect' => 'fade',
      ),
    );

    $form['magento_cart_quote_payment'] = array('#type' => 'submit',
      '#value' => t('Continue'),
    );
  }
  else {
    drupal_set_message(t('No payment methods available, try to reload this page. If problem persist please contact site Administrator.'), 'error');
    $form['payment_mode'] = array(
      '#value' => t('N/A')
    );
  }

  $form['_backlink'] = array(
    '#value' => magento_cart_get_back_link(),
  );
  $form['#mclc'] = magento_cart_last_changed();
  $form['mclc'] = array(
    '#type' => 'value',
    '#default_value' => magento_cart_last_changed()
  );
  $form['#validate'][] = 'magento_cart_quote_payment_form_validate';
  $form['#submit'][]   = 'magento_cart_quote_payment_form_submit';
  return $form;
}

/**
 * Payment Form Validation
 */
function magento_cart_quote_payment_form_validate(&$form, &$form_state) {
  global $user;
  global $_domain;


  // If we lost cart lock - drop error.
  if ($form_state['values']['mclc'] != magento_cart_last_changed()) {
    global $__magento_cart_lock_lost;
    $__magento_cart_lock_lost = TRUE;
    form_set_error('', t('Your Cart was modified outside of this checkout process, you can\'t continue.'));
    return FALSE;
  }

  $domain_id = intval($_domain['domain_id']);

  $cart = magento_cart_get();

  $payment_module = empty($form_state['values']['magento_cart_quote_payment_methods'])? FALSE : $form_state['values']['magento_cart_quote_payment_methods'];

  $cart['custom_data']['payment_module']  = $payment_module;
  magento_cart_save($cart);

  if (empty($payment_module) || empty($form['magento_cart_quote_payment_methods']['#options'][$payment_module])) {
    form_set_error('magento_cart_quote_payment_methods', t('Please, select a payment method'));
    return;
  }

  // Set order specific data
  $cart['custom_data']['payment_module']  = $payment_module;
  $cart['custom_data']['payment_mode']  = _magento_cart_quote_payment_method_name($payment_module);
  $cart['custom_data']['is_test_order'] = TRUE;

  // Add order settings(special fields) from payment modules
  if (module_hook($payment_module, "is_test_order")) {
    $cart['is_test_order'] = module_invoke($payment_module, "is_test_order");
  }

  $res = module_invoke($payment_module, 'payment_validate', $form, $form_state);

  //SET order extra PAYMENT data
  $cart['payment'] = array(
        'method'    => _magento_cart_quote_payment_method_name($payment_module),
        'po_number' => '', // doesn't matter at the moment
    );

  if (!empty($res['errors'])) {
    form_set_error($payment_module, '');
    return;
  }
  elseif (is_array($res)) {
    $cart['payment'] = array_merge($res, $cart['payment']);
  }

  // Save all info from payment step to the cart.
  $cart['payment_forms']['payment_form_state'] = $form_state;
  $cart['payment_forms']['payment_form'] = $form;

  $cart = magento_cart_update($cart, TRUE);

  if (!empty($cart['errors']) || empty($cart['quote']['id'])) {
    form_set_error('magento_cart_quote_payment_conditions', t("Can't create Quote"));
    return;
  }
}
/**
 * Payment Form Submit
 */
function magento_cart_quote_payment_form_submit($form, &$form_state) {
  global $user;

  // Go to next step.
  $next_step = magento_cart_get_step_next();
  magento_cart_goto_step($next_step);
  //$res = magento_cart_quote_payment_process($payment_module, MAGENTO_CART_ORDER_STATE_NEW, $form, $form_state);
}

/////////////////// CONFIRMATION PAGE //////////////////////////////////
/**
 * Confirmation page
 */
function magento_cart_quote_magento_cart_payment_succeeded() {
  drupal_add_css(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.css', 'module', 'all', FALSE);
  $output = drupal_get_form('magento_cart_quote_payment_succeeded_form');

  return theme('magento_cart_quote_content_wrapper', $output);
}
/**
 * Confirmation form
 */
function magento_cart_quote_payment_succeeded_form($form_state) {
  global $user;

  $form = array();

  $quote_id = arg(2);
  $quote = _magento_cart_payment_quote($quote_id, TRUE);

  if ($quote->uid !=  $user->uid) {
    //access denied
    drupal_access_denied();
    exit();
  }

  $payment_module = $quote->type;
  if (module_hook($payment_module, "confirmation_text")) {
    $form['confirmation_text'][$payment_module] = module_invoke($payment_module, "confirmation_text", $quote);
  }

  return $form;
}

/////////////////// PAYMENT API ////////////////////////////////////////
/**
 * PAYMENT API
 */
/*
 * Get Available payment methods
 * return array like array('payment_module_name' => 'payment_module_description',),
 * for example, array('paybox'             => 'Electronic Bank Card', 'paybycheck'         => 'Pay by Check', );
 */
function magento_cart_quote_payment_available_methods($new_cart = FALSE) {
  static $methods;
  if (!empty($methods) && empty($new_cart)) {
    return $methods;
  }

  global $_domain;

  $res=array();
  $paymethods = (array)_magento_cart_quote_payment_enabled_methods($_domain['domain_id']);
  $cart = empty($new_cart)? magento_cart_get() : $new_cart;

  foreach ($paymethods as $module) {
    if (module_hook($module, 'register_payment_system') && (!module_hook($module, 'check_visibility') || module_invoke($module, 'check_visibility', $cart)) ) {
      $res[$module] = module_invoke($module, 'register_payment_system');
    }
  }

  $methods = $res;
  return $methods;
}
/**
 * Add payment method specific options(fields) to the Payment form
 */
function _magento_cart_quote_payment_options(&$form) {
  $available_payments = magento_cart_quote_payment_available_methods();

  foreach ($available_payments as $payment_module => $descr) {
    $form['paymethods'][$payment_module] = module_invoke($payment_module, "payment_options");
  }
}
/**
 * Returns quote_id for identification current payment.
 * The quote_id should be used in all payment modules.
 *
 * @param $payment_module
 *   string Name of the Drupal's payment module that will process the payment
 * @param $status
 *   string Quote Status (MAGENTO_CART_ORDER_STATE_NEW, MAGENTO_CART_ORDER_STATE_PENDING_PAYMENT, etc ...)
 * @param $form
 *   Array with the form data built on the Payment step. (This data is passed to the payment module into hook_payment_process)
 * @param $form_state
 *   Array with the form_state data built on the Payment step. (This data is passed to the payment module into hook_payment_process)
 *
 * @return $quote_id
 *   ID for identification current payment in Drupal. ( It doesn't match to quote_id of the corresponding Quote in Magento)
 */
function magento_cart_quote_payment_process($payment_module, $status, $form, $form_state) {
  global $user;

  $cart = magento_cart_get();

  $quote = new stdClass();
  $quote->uid = $user->uid;
  $quote->status = $status;
  $quote->cart = serialize($cart);
  $quote->type = $payment_module;
  $quote->created_at = time();
  $quote->updated_at = time();

  //LOG action
  //CART {operation}: uid: {uid}, {dump}
  watchdog('magento_cart_quote', $message = 'CART payment_form_submit:  uid: %uid, %quote', $variables = array('%uid' => $user->uid, '%quote' => serialize($quote)), $severity = WATCHDOG_INFO, $link = NULL);

  //presave current transaction
  drupal_write_record('cart_quotes', $quote);

  // Mark customer first address (the billing address) as read_only - can't be changed any longer.
  //magento_customer_mark_billing_address($user->uid);

  //LOG action
  watchdog('magento_cart_quote', $message = 'CART called payment_process:   uid: %uid, %quote', $variables = array('%uid' => $user->uid, '%quote' => serialize($quote)), $severity = WATCHDOG_INFO, $link = NULL);

  return _magento_cart_quote_payment_process($payment_module, $quote->quote_id, $form, $form_state);
}

/**
 * Process payment via payment system
 */
function _magento_cart_quote_payment_process($payment_module, $quote_id, $form, $form_state) {
  return module_invoke($payment_module, 'payment_process', $quote_id, $form, $form_state);
}

/**
 * Returns Quota presaved when payment process starts
 * Should be used by payment modules in order to access quote data
 */
function _magento_cart_payment_quote($quote_id, $full_object = FALSE) {
  $query = "SELECT * FROM {cart_quotes} where quote_id = %d";
  $res = db_fetch_object(db_query($query, $quote_id));
  if (empty($res)) {
    //do some exception handling
    return;
  }

  $res->cart = unserialize($res->cart);

  if ($full_object) {
    return $res;
  }

  return $res->cart;
}

/**
 * Returns Quota identified by order_id (This means order must be precreated) presaved when payment process starts
 * Should only be used by payment modules IF due to process flow  the order is created before passing it to Payment Gateway
 * Should be used by payment modules in order to access quote data
 */
function _magento_cart_payment_quote_by_order_id($order_id, $full_object = FALSE) {
  $query = "SELECT * FROM {cart_quotes} where order_id = %d";
  $res = db_fetch_object(db_query($query, $order_id));
  if (empty($res)) {
    //do some exception handling
    return;
  }

  $res->cart = unserialize($res->cart);

  if ($full_object) {
    return $res;
  }

  return $res->cart;
}

/**
 * Updates Quota presaved when payment process starts
 */
function _magento_cart_quote_payment_quota_update($quote, $updated_data = array()) {
  if (empty($updated_data) || !is_array($updated_data)) {
    return;
  }

  unset($updated_data['quote_id']);

  foreach ($updated_data as $field => $value) {
    $quote->$field = $value;
  }

  $quote->updated_at = time();
  if (is_array($quote->cart)) {
    $quote->cart = serialize($quote->cart);
  }

  $res = drupal_write_record('cart_quotes', $quote, array('quote_id'));
  $quote->cart = unserialize($quote->cart);

  return $res;
}

/**
 * CALLBACK hook used to create/update Order (order in magento)
 * @quote_id - ID of the quote in cart_quotes
 * @status -
 * This callback shoud be called by payment modules
 */
function magento_cart_quote_payment_callback($quote_id, $status = '') {
  $quote = _magento_cart_payment_quote($quote_id, TRUE);
  $user = user_load($quote->uid);
  //LOG action
  //CART {operation}: uid: {uid}, {dump}
  watchdog('magento_cart_quote', $message = 'CART payment_callback:  uid: %uid, [QUOTE_ID: %quoteid, STATUS: %status]', $variables = array('%uid' => $user->uid, '%quoteid' => $quote_id, '%status' => $status), $severity = WATCHDOG_INFO, $link = NULL);


  if (empty($quote->cart['quote']['id']) || empty($quote->cart['store_id'])) {
    //do some exception handling
    magento_cart_quote_payment_failed($quote_id, array(t('The Payment is invalid. Quote is empty.')));
    return;
  }

  if (empty($quote->order_id)) {
    ///////////////Creating order///////////////////
    magento_cart_quote_payment_order_create($quote);
  }

  if (empty($status)) {
    return $quote->order_id;
  }

  if (!in_array($status, _magento_cart_order_alowed_states())) {
    //do some exception handling
    magento_cart_quote_payment_failed($quote_id, array(t('Requested order status is invalid.')));
    return;
  }

  return magento_cart_quote_payment_order_update($quote, $status);
}

/**
 * Redirect to payment-succeded page
 */
function magento_cart_quote_payment_succeded($quote_id) {
  $quote = _magento_cart_payment_quote($quote_id, TRUE);
  $user = user_load($quote->uid);
  //LOG action
  //CART {operation}: uid: {uid}, {dump}
  watchdog('magento_cart_quote', $message = 'CART magento_cart_quote_payment_SUCCEDED:  uid: %uid, QUOTE_ID: %quoteid', $variables = array('%uid' => $user->uid, '%quoteid' => $quote_id), $severity = WATCHDOG_INFO, $link = l(t('Show The Order'), 'cart/payment-succeeded/' . (int)$quote_id));

  // Go to next step.
  $next_step = magento_cart_get_step_next();
  magento_cart_goto_step($next_step, (int)$quote_id);

  //magento_cart_goto_step('payment-succeeded', (int)$quote_id);
  //drupal_goto('cart/payment-succeeded/' . (int)$quote_id);
}

/**
 * Redirect to payment-failed page
 */
function magento_cart_quote_payment_failed($quote_id, $errors=array()) {
  $quote = _magento_cart_payment_quote($quote_id, TRUE);
  $user = empty($quote->uid)? FALSE : user_load($quote->uid);
  //LOG action
  //CART {operation}:  uid: {uid}, {dump}
  watchdog('magento_cart_quote', $message = 'CART magento_cart_quote_payment_failed:  uid: %uid, [QUOTE_ID: %quoteid, ERRORS: %errors]', $variables = array('%uid' => $user->uid, '%quoteid' => $quote_id, '%errors' => serialize($errors)), $severity = WATCHDOG_WARNING, $link = NULL);

  drupal_set_message(t('Payment failed.') . '<br/>' . join('<br/>', (array)$errors), 'error');
  //magento_cart_update_state('payment');
  //drupal_goto('cart/payment-failed/'.(int)$quote_id);
  //drupal_goto('cart/payment');
  magento_cart_goto_step('payment');
  exit();
}

/**
 * Return all avaialbe payment systems
 * NOTE! To register your module as payment system implement hook_register_payment_system() in your module.
 * The hook_register_payment_system should return payment system's friendly name.
 */
function _magento_cart_quote_payment_registered_methods($without_extra_info=FALSE) {
  $res=array();
  foreach (module_implements('register_payment_system') as $module) {
    $func = $module . '_register_payment_system';
    $res[$module] = $func() . ($without_extra_info? '' : ' <span class="description">(module `' . $module . '`)</span>');
  }
  return $res;
}

/**
 * Get enabled payment methods for the @domain.
 */
function _magento_cart_quote_payment_enabled_methods($domain_id = 0) {
  return unserialize(variable_get('enabled_paymethods_'. intval($domain_id), serialize(array('checkmo')) ));
}


/**
 * Create Order in Magento
 */
function magento_cart_quote_create_order($partial_cart = NULL, $cart = NULL) {
  $result = magento_cart_order_update($partial_cart);

  if ($result && !empty($cart['products'])) {
    $products = array();
    //$cart = magento_cart_get();

    foreach ($cart['products'] as $product => $data) {
      $products[] = array($product, $data['qty']);
    }

    module_invoke_all('create_order_succeeded', $products);
  }
  return $result;
}

/**
 * Create Order from the Quote
 */
function magento_cart_quote_payment_order_create($quote) {

  // merge warranties data in to quote data
  $quote_data = $quote->cart['quote'];
  foreach ($quote->cart['products'] as $product_nid => $product) {
    if (!empty($product['warranty']) && !empty($product['data']['selected_warranty_sku'])) {
      $quote_data['products'][$product['id']]['data']['selected_warranty_sku'] = $product['data']['selected_warranty_sku'];
    }
  }

  $partial_cart =  array(
        'store_id' => $quote->cart['store_id'],
        'quote' => $quote_data,//$quote->cart['quote'],
    );

  // Creates ORDER
  $order = magento_cart_quote_create_order($partial_cart, $quote->cart);
  if (empty($order['order_id'])) {
    //do some exception handling
    magento_cart_quote_payment_failed($quote_id, array(t("Couldn't create Order in magento.")));
    return;
  }

  $quote->cart['order'] = $order;
  _magento_cart_quote_payment_quota_update($quote, array('order_id' => $order['increment_id']));

  // return ORDER increment id (ERP order_id)
  return $order['increment_id'];
}

/**
 * Update Order status in Magento
 */
function magento_cart_quote_payment_order_update($quote, $status) {
  if (empty($quote->cart['order'])) {
    return FALSE;
  }

  $order = $quote->cart['order'];
  $store_id = $quote->cart['store_id'];

  // call to magento
  $res = _magento_cart_update_order($order, $store_id, $status);
  if (empty($res)) {
    //do some exception handling
    magento_cart_quote_payment_failed($quote_id, array(t("Couldn't update Order Status in magento.")));
    return;
  }

  _magento_cart_quote_payment_quota_update($quote, array('status' => $status));

  return $quote->order_id;
}

/**
 * Prepayment update Quote to see all totals
 * #ahah callback
 */
function magento_cart_quote_payment_update() {
  global $user;

  $form_id = 'magento_cart_quote_payment_form';
  $form_state = array('storage' => NULL, 'submitted' => FALSE, 'process_input' => 1, 'ajaxupdate' => TRUE);
  $form_build_id = $_POST['form_build_id'];

  // Retrieve form from cache.
  $form = form_get_cache($form_build_id, $form_state);

  // If we loast cart lock - drop error, and return message.
  if ($form['#mclc'] != magento_cart_last_changed()) {
    form_set_error('', t('Your Cart was modified outside of this checkout process, you can\'t continue.'));
    $output = t('Maybe you have your cart open in another browser window?');
  }
  else { // Update cart and rebuild form.
    $cart = magento_cart_quote_payment_update_helper();
    $form_state['post'] = $form['#post'] = $_POST;
    $form = drupal_rebuild_form($form_id, $form_state, array($form_state, $cart), $form_build_id);

    $this_step = magento_cart_get_step();
    $form['#action'] = url(magento_cart_get_step_path($this_step));
    $output = drupal_render_form($form_id, $form);
  }

  $output = theme_status_messages() . $output;
  $output .= '<script>$("div#magento_cart_state").html(' . drupal_to_js(theme('magento_cart_state')) . ');</script>';

  print drupal_json(array('status' => TRUE, 'data' => $output));
}

function magento_cart_quote_payment_update_helper() {
  global $_domain;
  $domain_id = intval($_domain['domain_id']);

  $cart = magento_cart_get();

  // VALIDATE arguments
  //$cart['custom_data']['is_payment_delayed'] = empty($_REQUEST['is_payment_delayed']) ? 0 : 1;

  // validate @payment_mode
  if (!empty($_REQUEST['payment_mode'])) {
    $payment_mode       = $_REQUEST['payment_mode'];
    $cart['custom_data']['payment_module']     = $payment_mode;
    $paymethods = magento_cart_quote_payment_available_methods($cart);
    if (!in_array($payment_mode, array_keys($paymethods))) {
      $cart['errors'][] = t('Selected payment mothod is not allowed.');
    }
  }
  if (!_magento_cart_quote_payment_update_validate($cart)) {
    return $cart;
  }

  // Set order specific data
  $cart['custom_data']['payment_module']     = $payment_mode;
  $cart['custom_data']['payment_mode']       = _magento_cart_quote_payment_method_name($payment_mode);

  $cart = magento_cart_update($cart, TRUE);
  // Final rendering callback.
  return $cart;
}
/**
 *
 */
function _magento_cart_quote_payment_update_validate($cart) {
  if (!empty($cart['errors'])) {
    //$errors = drupal_to_js(strip_tags(join(' ',$cart['errors'] )));
    return FALSE;
  }
  //validated
  return TRUE;
}

/**
 * Print price with coins.
 */
function _magento_cart_quote_payment_product_price($price, $currency_symbol = '€' ) {
  _magento_cart_quote_product_price($price, $currency_symbol);
}

/**
 * Returns payment_method_name from the module if hook_payment_method_name is implemented or the module name otherwise.
 */
function _magento_cart_quote_payment_method_name($payment_module) {
  $res = $payment_module;
  if (module_hook($payment_module, "payment_method_name")) {
    $res = module_invoke($payment_module, "payment_method_name");
  }
  return $res;
}