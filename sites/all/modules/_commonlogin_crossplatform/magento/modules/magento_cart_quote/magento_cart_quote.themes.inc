<?php
// $Id$
/**
 * @file
 * Implements theme templates PREPROCESS hooks
 */
/////////////////// PREPROCESS FUNCTIONS ///////////////////////////////
/**
 * Process variables for magento_cart_quote_view.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see magento_cart_quote_view.tpl.php
 */
function template_preprocess_magento_cart_quote_view_form(&$variables) {
  //drupal_add_js(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.js', 'module');
  $form = & $variables['form'];
  $cart = magento_cart_get();

  $variables['cart_errors'] = empty($cart['products']) || empty($cart['quote']['errors']) ? '' : theme('item_list', $cart['quote']['errors'], $title = '', 'ul', array('class' => 'cart-errors'));

  if (!isset($cart['products']) || 0 == count($cart['products'])) {
    $variables['cart_items'] = array();
    $variables['cart_header'] = array();
    $variables['products_list'] = theme('magento_cart_empty');
    return;
  }

  $header = array(
    'image'        => array('data' => '', 'class' => 'product-image'),
    'product_name' => array('data' => t('Product Name'), 'class' => 'product-name'),
    'unit_price'   => array('data' => t('Unit Price'), 'class' => 'product-unit-price'),
    'qty'          => array('data' => t('Qty'), 'class' => 'product-qty'),
    'subtotal'     => array('data' => t('Subtotal'), 'class' => 'product-subtotal'),
  );

  $rows= array();
  foreach ($cart['products'] as $key => $product) {
    $product_node = _magento_product_load_product($key); //$product['nid']
    if (!isset($form["magento_cart_quote_product_$key"])) {
      continue;
    }

    $product_image = theme('magento_product_image', $product_node->field_thumbnail[0]['value'], array('class' => 'magento-product-tiny-image'));
    $product_options = theme('magento_product_options', $product, $product_node);
    $product_row = array(
      'image' => l($product_image, 'node/'. $product_node->nid, array('html' => TRUE, 'attributes' => array('class' => 'product-image'))),
      'product_name' => array(
        'data' => l($product_node->title, 'node/'. $product_node->nid) .
                    (empty($product['errors'])? '':'<div class="cart-product-row-error">' .
                    theme('item_list', $product['errors']) . '</div>') .
                    $product_options,
        'class' => 'product-name'
      ),
      'unit_price' => theme('magento_product_price', $product_node),
      'qty'   => array(
        'data' => drupal_render($form["magento_cart_quote_product_$key"]) . drupal_render($form["magento_cart_quote_remove_$key"]),
        'class' => 'product-qty'
      ),
      'subtotal' => theme('magento_product_price', $product_node, intval($product['qty'])),
    );

    $rows[] = $product_row;

  }

  $subtotal_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Subtotal:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['subtotal']),
  );
  $rows[] = $subtotal_row;

  if (!empty($cart['quote']['discount_amount'])) {
    $discount_row = array(
      array('data' => '', 'colspan' => 3),
      'qty'   => array('data' => t('Discount (!cc):', array('!cc' => $cart['coupon']['code'])), 'class' => 'cart-subtotal-label'),
      'subtotal' => theme('magento_product_format_price', $cart['quote']['discount_amount']),
    );
    $rows[] = $discount_row;
  }

  $total_row =  array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => '<strong>' . t('Grand Total:') . '</strong>', 'class' => 'cart-grand-total-label'),
    'subtotal' => '<strong>'. theme('magento_product_format_price', $cart['grand_total']) .'</strong>',
  );
  $rows[] = $total_row;

  $this_step = magento_cart_get_step();
  $variables['cart_navigation'] = theme('magento_cart_nav', $this_step['step_id']);
  $variables['cart_items'] = $rows;
  $variables['cart_header'] = $header;
  $variables['products_list'] = theme('table', $header, $rows, array('class' => 'magento-cart-quote-table'));
}

/**
 * Process variables for magento_cart_quote_delivery.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see magento_cart_quote_delivery.tpl.php
 */
function template_preprocess_magento_cart_quote_delivery_form(&$variables) {
  drupal_add_js(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.js', 'module');
  $form = & $variables['form'];
  $cart = magento_cart_get();

  $variables['cart_errors'] = empty($cart['quote']['errors']) ? '' : theme('item_list', $cart['quote']['errors'], $title = '', 'ul', array('class' => 'cart-errors'));

  /*
  if (!isset($cart['products']) || 0 == count($cart['products'])) {
    $variables['cart_navigation'] = '';
    $variables['cart_items'] = array();
    $variables['cart_header'] = array();
    $variables['products_list'] = theme('magento_cart_empty');
    return;
  }

  $header = array(
    'image'        => array('data' => '', 'class' => 'product-image'),
    'product_name' => array('data' => t('Product Name'), 'class' => 'product-name'),
    'unit_price'   => array('data' => t('Unit Price'), 'class' => 'product-unit-price'),
    'qty'          => array('data' => t('Qty'), 'class' => 'product-qty'),
    'subtotal'     => array('data' => t('Subtotal'), 'class' => 'product-subtotal'),
  );

  $rows= array();
  foreach ($cart['products'] as $key => $product) {
    $product_node = _magento_product_load_product($product['nid']);//$product['nid']
    if (!isset($form['products'][$product_node->nid])) {
      continue;
    }

    $product_image = theme('magento_product_image', $product_node->field_thumbnail[0]['value'], array('class' => 'magento-product-tiny-image'));
    $product_row = array(
      'image' => l($product_image, 'node/'. $product_node->nid, array('html' => TRUE, 'attributes' => array('class' => 'product-image'))),
      'product_name' => array(
        'data' => l($product_node->title, 'node/'. $product_node->nid) . (empty($product['errors'])? '':'<div class="cart-product-row-error">' . theme('item_list', $product['errors']) . '</div>'),
        'class' => 'product-name'
      ),
      'unit_price' => theme('magento_product_price', $product_node),
      'qty'   => $cart['products'][$product_node->nid]['qty'],
      'subtotal' => theme('magento_product_price', $product_node, intval($product['qty'])),
    );

    $rows[] = $product_row;
  }

  $subtotal_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Subtotal:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['subtotal']),
  );
  $shipping_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Shipping:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['quote']['shipping_amount']),
  );
  $total_row =  array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => '<strong>' . t('Grand Total:') . '</strong>', 'class' => 'cart-grand-total-label'),
    'subtotal' => '<strong>'. theme('magento_product_format_price', $cart['grand_total']) .'</strong>',
  );
  $rows[] = $subtotal_row;
  $rows[] = $shipping_row;
  $rows[] = $total_row;*/

  drupal_render($form['products']);
  $this_step = magento_cart_get_step();
  $variables['cart_navigation'] = theme('magento_cart_nav', $this_step['step_id']);
  /*
  $variables['cart_items'] = $rows;
  $variables['cart_header'] = $header;
  $variables['products_list'] = theme('table', $header, $rows, array('class' => 'magento-cart-quote-table'));
  */

}

/**
 * Process variables for magento_cart_quote_payment.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see magento_cart_quote_payment.tpl.php
 */
function template_preprocess_magento_cart_quote_payment_form(&$variables) {
  drupal_add_js(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.js', 'module');
  $form = & $variables['form'];
  $cart = magento_cart_get();

  $variables['cart_errors'] = empty($cart['quote']['errors']) ? '' : theme('item_list', $cart['quote']['errors'], $title = '', 'ul', array('class' => 'cart-errors'));

  if (!isset($cart['products']) || 0 == count($cart['products'])) {
    $variables['cart_navigation'] = '';
    $variables['cart_items'] = array();
    $variables['cart_header'] = array();
    $variables['products_list'] = theme('magento_cart_empty');
    return;
  }

  $header = array(
    'image'        => array('data' => '', 'class' => 'product-image'),
    'product_name' => array('data' => t('Product Name'), 'class' => 'product-name'),
    'unit_price'   => array('data' => t('Unit Price'), 'class' => 'product-unit-price'),
    'qty'          => array('data' => t('Qty'), 'class' => 'product-qty'),
    'subtotal'     => array('data' => t('Subtotal'), 'class' => 'product-subtotal'),
  );

  $rows= array();

  foreach ($cart['products'] as $key => $product) {

    $product_node = _magento_product_load_product($product['nid']);

    $product_image = theme('magento_product_image', $product_node->field_thumbnail[0]['value'], array('class' => 'magento-product-tiny-image'));
    $product_options = theme('magento_product_options', $product, $product_node);
    $product_row = array(
      'image' => l($product_image, 'node/'. $product_node->nid, array('html' => TRUE, 'attributes' => array('class' => 'product-image'))),
      'product_name' => array(
        'data' => l($product_node->title, 'node/'. $product_node->nid) . (empty($product['errors'])? '':'<div class="cart-product-row-error">' . theme('item_list', $product['errors']) . '</div>') . $product_options,
        'class' => 'product-name'
      ),
      'unit_price' => theme('magento_product_price', $product_node),
      'qty'   => $cart['products'][$product_node->nid]['qty'],
      'subtotal' => theme('magento_product_price', $product_node, intval($product['qty'])),
    );

    $rows[] = $product_row;

  }

  $subtotal_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Subtotal:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['subtotal']),
  );
  $rows[]=$subtotal_row;

  if (!empty($cart['quote']['discount_amount'])) {
    $discount_row = array(
      array('data' => '', 'colspan' => 3),
      'qty'   => array('data' => t('Discount (!cc):', array('!cc' => $cart['coupon']['code'])), 'class' => 'cart-subtotal-label'),
      'subtotal' => theme('magento_product_format_price', $cart['quote']['discount_amount']),
    );
    $rows[] = $discount_row;
  }

  $shipping_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Shipping amount:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => array(
      'data' => theme_magento_product_format_price($cart['quote']['shipping_amount']),
      'id' => 'magento_cart_quote_shipping_method',
    )
  );
  $rows[] = $shipping_row;

  $total_row =  array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => '<strong>' . t('Grand Total:') . '</strong>', 'class' => 'cart-grand-total-label'),
    'subtotal' => array(
      'data' => '<strong>'. theme_magento_product_format_price($cart['grand_total']) .'</strong>',
    )
  );

  $rows[]=$total_row;

  $this_step = magento_cart_get_step();
  $variables['cart_navigation'] = theme('magento_cart_nav', $this_step['step_id']);
  $variables['cart_items'] = $rows;
  $variables['cart_header'] = $header;
  $variables['products_list'] = theme('table', $header, $rows, array('class' => 'magento-cart-quote-table'));





  // Payment options //////////////////////////////////
  $variables['payment_methods'] = array();
  $variables['payment_methods_msg'] = '';
  /*
  if (empty($form['magento_cart_quote_payment_methods'])) {
    $variables['payment_methods_msg'] = t('No paymethod available from selected options.');
  }
  */

  if (!empty($form['magento_cart_quote_payment_methods'])) {
    foreach ($form['magento_cart_quote_payment_methods'] as $paymethod => $method) {
      if (strpos($paymethod, '#') === 0) {
        continue;
      }
      $variables['payment_methods']['paymethods'][$paymethod]=$method;
    }
  }
}

/**
 * Process variables for magento_cart_quote_payment_succeded.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see magento_cart_quote_payment_succeded.tpl.php
 */
function template_preprocess_magento_cart_quote_payment_succeeded_form(&$variables) {
  //drupal_add_js(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.js', 'module');
  $form = & $variables['form'];
  $order = _magento_cart_payment_quote(arg(2), TRUE);
  $cart = $order->cart;
  $user= user_load($order->uid);


  $variables['cart_errors'] = empty($cart['quote']['errors']) ? '' : theme('item_list', $cart['quote']['errors'], $title = '', 'ul', array('class' => 'cart-errors'));

  if (!isset($cart['products']) || 0 == count($cart['products'])) {
    $variables['cart_navigation'] = '';
    $variables['cart_items'] = array();
    $variables['cart_header'] = array();
    $variables['products_list'] = theme('magento_cart_empty');
    return;
  }

  $header = array(
    'image'        => array('data' => '', 'class' => 'product-image'),
    'product_name' => array('data' => t('Product Name'), 'class' => 'product-name'),
    'unit_price'   => array('data' => t('Unit Price'), 'class' => 'product-unit-price'),
    'qty'          => array('data' => t('Qty'), 'class' => 'product-qty'),
    'subtotal'     => array('data' => t('Subtotal'), 'class' => 'product-subtotal'),
  );

  $rows= array();

  foreach ($cart['products'] as $key => $product) {

    $product_node = _magento_product_load_product($product['nid']);

    $product_image = theme('magento_product_image', $product_node->field_thumbnail[0]['value'], array('class' => 'magento-product-tiny-image'));
    $product_options = theme('magento_product_options', $product, $product_node);
    $product_row = array(
      'image' => l($product_image, 'node/'. $product_node->nid, array('html' => TRUE, 'attributes' => array('class' => 'product-image'))),
      'product_name' => array(
        'data' => l($product_node->title, 'node/'. $product_node->nid) . (empty($product['errors'])? '':'<div class="cart-product-row-error">' . theme('item_list', $product['errors']) . '</div>') . $product_options,
        'class' => 'product-name'
      ),
      'unit_price' => theme('magento_product_price', $product_node),
      'qty'   => $cart['products'][$product_node->nid]['qty'],
      'subtotal' => theme('magento_product_price', $product_node, intval($product['qty'])),
    );

    $rows[] = $product_row;

  }

  $subtotal_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Subtotal:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['subtotal']),
  );
  $rows[]=$subtotal_row;

  if (!empty($cart['quote']['discount_amount'])) {
    $discount_row = array(
      array('data' => '', 'colspan' => 3),
      'qty'   => array('data' => t('Discount (!cc):', array('!cc' => $cart['coupon']['code'])), 'class' => 'cart-subtotal-label'),
      'subtotal' => theme('magento_product_format_price', $cart['quote']['discount_amount']),
    );
    $rows[] = $discount_row;
  }

  $shiping_methods = magento_cart_quote_shipping_methods($cart);
  $this_shipping_method = $shiping_methods[$cart['shipping']['method']];
  $shipping_row = array(
    array('data' => '', 'colspan' => 3),
    'qty' => array('data' => t('Shipping (!carrier - !method):',
        array(
          '!carrier' => $this_shipping_method['raw_carrier'],
          '!method' => $this_shipping_method['raw_method'],
        )), 'class' => 'cart-subtotal-label'),
    'subtotal' => array(
      'data' => theme_magento_product_format_price($cart['quote']['shipping_amount']),
      'id' => 'magento_cart_quote_shipping_method',
    )
  );
  $rows[] = $shipping_row;

  $total_row =  array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => '<strong>' . t('Grand Total:') . '</strong>', 'class' => 'cart-grand-total-label'),
    'subtotal' => array(
      'data' => '<strong>'. theme_magento_product_format_price($cart['grand_total']) .'</strong>',
    )
  );
  $rows[]=$total_row;

  $this_step = magento_cart_get_step();
  $variables['cart_navigation'] = theme('magento_cart_nav', $this_step['step_id']);
  $variables['cart_items'] = $rows;
  $variables['cart_header'] = $header;
  $variables['products_list'] = theme('table', $header, $rows, array('class' => 'magento-cart-quote-table'));


  $variables['shipping_address'] = magento_cart_quote_prepare_address($cart['shipping']['address']);
  $variables['billing_address']  = magento_cart_quote_prepare_address($cart['billing']['address']);
  $variables['order'] = $order;

}

/**
 * Process variables for magento_cart_quote_review_form.tpl.php.
 */
function template_preprocess_magento_cart_quote_review_form(&$variables) {

  drupal_add_js(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.js', 'module');
  $form = & $variables['form'];
  $cart = magento_cart_get();

  $variables['cart_errors'] = empty($cart['quote']['errors']) ? '' : theme('item_list', $cart['quote']['errors'], $title = '', 'ul', array('class' => 'cart-errors'));

  if (!isset($cart['products']) || 0 == count($cart['products'])) {
    $variables['cart_navigation'] = '';
    $variables['cart_items'] = array();
    $variables['cart_header'] = array();
    $variables['products_list'] = theme('magento_cart_empty');
    return;
  }

  $header = array(
    'image'        => array('data' => '', 'class' => 'product-image'),
    'product_name' => array('data' => t('Product Name'), 'class' => 'product-name'),
    'unit_price'   => array('data' => t('Unit Price'), 'class' => 'product-unit-price'),
    'qty'          => array('data' => t('Qty'), 'class' => 'product-qty'),
    'subtotal'     => array('data' => t('Subtotal'), 'class' => 'product-subtotal'),
  );

  $rows= array();

  foreach ($cart['products'] as $key => $product) {

    $product_node = _magento_product_load_product($product['nid']);

    $product_image = theme('magento_product_image', $product_node->field_thumbnail[0]['value'], array('class' => 'magento-product-tiny-image'));
    $product_options = theme('magento_product_options', $product, $product_node);
    $product_row = array(
      'image' => l($product_image, 'node/'. $product_node->nid, array('html' => TRUE, 'attributes' => array('class' => 'product-image'))),
      'product_name' => array(
        'data' => l($product_node->title, 'node/'. $product_node->nid) . (empty($product['errors'])? '':'<div class="cart-product-row-error">' . theme('item_list', $product['errors']) . '</div>') . $product_options,
        'class' => 'product-name'
      ),
      'unit_price' => theme('magento_product_price', $product_node),
      'qty'   => $cart['products'][$product_node->nid]['qty'],
      'subtotal' => theme('magento_product_price', $product_node, intval($product['qty'])),
    );

    $rows[] = $product_row;

  }

  $subtotal_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Subtotal:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['subtotal']),
  );
  $rows[]=$subtotal_row;

  if (!empty($cart['quote']['discount_amount'])) {
    $discount_row = array(
      array('data' => '', 'colspan' => 3),
      'qty'   => array('data' => t('Discount (!cc):', array('!cc' => $cart['coupon']['code'])), 'class' => 'cart-subtotal-label'),
      'subtotal' => theme('magento_product_format_price', $cart['quote']['discount_amount']),
    );
    $rows[] = $discount_row;
  }


  $shiping_methods = magento_cart_quote_shipping_methods();
  $this_shipping_method = $shiping_methods[$cart['shipping']['method']];
  $shipping_row = array(
    array('data' => '', 'colspan' => 3),
    'qty' => array('data' => t('Shipping (!carrier - !method):',
        array(
          '!carrier' => $this_shipping_method['raw_carrier'],
          '!method' => $this_shipping_method['raw_method'],
        )), 'class' => 'cart-subtotal-label'),
    'subtotal' => array(
      'data' => theme_magento_product_format_price($cart['quote']['shipping_amount']),
      'id' => 'magento_cart_quote_shipping_method',
    )
  );
  $rows[] = $shipping_row;

  $total_row =  array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => '<strong>' . t('Grand Total:') . '</strong>', 'class' => 'cart-grand-total-label'),
    'subtotal' => array(
      'data' => '<strong>'. theme_magento_product_format_price($cart['grand_total']) .'</strong>',
    )
  );

  $rows[]=$total_row;

  $this_step = magento_cart_get_step();
  $variables['cart_navigation'] = theme('magento_cart_nav', $this_step['step_id']);
  $variables['cart_items'] = $rows;
  $variables['cart_header'] = $header;
  $variables['products_list'] = theme('table', $header, $rows, array('class' => 'magento-cart-quote-table'));

  $variables['billing_address']  = magento_cart_quote_prepare_address($cart['billing']['address']);
  $variables['shipping_address'] = magento_cart_quote_prepare_address($cart['shipping']['address']);

  $paymethods = magento_cart_quote_payment_available_methods();
  $this_paymethod_title = $paymethods[$cart['custom_data']['payment_module']];
  $variables['payment_method'] = array(
    'title' => $this_paymethod_title,
    'description' => '',
  );

}

/**
 * Process variables for magento_cart_quote_nav.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $state - current cart state
 *
 * @see magento_cart_quote_nav.tpl.php
 */
function template_preprocess_magento_cart_nav(&$variables) {
  global $user;
  $current_state = $variables['state'];
  $class = ($current_state == 'payment-succeeded') ? 'off' : 'on';
  $cart_states = magento_cart_get_steps();

  foreach ($cart_states as $state => &$item) {
    if (!empty($item['hidden'])) {
      unset($cart_states[$state]);
      continue;
    }

    $item['class'] = $class . ' ' . $state;

    if ($state == $current_state) {
      $item['class'] = 'current on ' . $state;
      $class = 'off';

      // Set title according to current highlighted step.
      drupal_set_title($item['title']);
    }

    if ($item['class'] != 'off' . ' ' . $state) {
      $item['path'] = magento_cart_get_step_path($state);
    }
  }

  $variables['cart_states'] = $cart_states;
}

/**
 * Process variables for magento_cart_quote_delivery.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see magento_cart_quote_delivery.tpl.php
 */
function template_preprocess_magento_cart_quote_shipping_method_form(&$variables) {
  drupal_add_js(drupal_get_path('module', 'magento_cart_quote') .'/magento_cart_quote.js', 'module');
  $form = & $variables['form'];
  $cart = magento_cart_get();

  $variables['cart_errors'] = empty($cart['quote']['errors']) ? '' : theme('item_list', $cart['quote']['errors'], $title = '', 'ul', array('class' => 'cart-errors'));

  if (!isset($cart['products']) || 0 == count($cart['products'])) {
    $variables['cart_navigation'] = '';
    $variables['cart_items'] = array();
    $variables['cart_header'] = array();
    $variables['products_list'] = theme('magento_cart_empty');
    return;
  }

  $header = array(
    'image'        => array('data' => '', 'class' => 'product-image'),
    'product_name' => array('data' => t('Product Name'), 'class' => 'product-name'),
    'unit_price'   => array('data' => t('Unit Price'), 'class' => 'product-unit-price'),
    'qty'          => array('data' => t('Qty'), 'class' => 'product-qty'),
    'subtotal'     => array('data' => t('Subtotal'), 'class' => 'product-subtotal'),
  );

  $rows= array();
  foreach ($cart['products'] as $key => $product) {
    $product_node = _magento_product_load_product($product['nid']);//$product['nid']
    if (!isset($form['products'][$key])) {
      continue;
    }

    $product_image = theme('magento_product_image', $product_node->field_thumbnail[0]['value'], array('class' => 'magento-product-tiny-image'));
    $product_options = theme('magento_product_options', $product, $product_node);
    $product_row = array(
      'image' => l($product_image, 'node/'. $product_node->nid, array('html' => TRUE, 'attributes' => array('class' => 'product-image'))),
      'product_name' => array(
        'data' => l($product_node->title, 'node/'. $product_node->nid) . (empty($product['errors'])? '':'<div class="cart-product-row-error">' . theme('item_list', $product['errors']) . '</div>') . $product_options,
        'class' => 'product-name'
      ),
      'unit_price' => theme('magento_product_price', $product_node),
      'qty'   => $cart['products'][$product_node->nid]['qty'],
      'subtotal' => theme('magento_product_price', $product_node, intval($product['qty'])),
    );

    $rows[] = $product_row;
  }

  $subtotal_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Subtotal:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => theme('magento_product_format_price', $cart['subtotal']),
  );
  $rows[] = $subtotal_row;

  if (!empty($cart['quote']['discount_amount'])) {
    $discount_row = array(
      array('data' => '', 'colspan' => 3),
      'qty'   => array('data' => t('Discount (!cc):', array('!cc' => $cart['coupon']['code'])), 'class' => 'cart-subtotal-label'),
      'subtotal' => theme('magento_product_format_price', $cart['quote']['discount_amount']),
    );
    $rows[] = $discount_row;
  }

  $shipping_row = array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => t('Shipping:'), 'class' => 'cart-subtotal-label'),
    'subtotal' => array(
      'data' => $form['#shiping_method']['price'],
      'id' => 'magento_cart_quote_shipping_method',
    )
  );
  drupal_add_js(array('magento_cart_quote' => array('shipping_methods' => $form['#shiping_methods'])), 'setting');

  $total_row =  array(
    array('data' => '', 'colspan' => 3),
    'qty'   => array('data' => '<strong>' . t('Grand Total:') . '</strong>', 'class' => 'cart-grand-total-label'),
    'subtotal' => array(
      'data' => '<strong id="magento_cart_quote_grand_total">'. $form['#shiping_method']['grand_total'] .'</strong>',
    )
  );
  $rows[] = $shipping_row;
  $rows[] = $total_row;

  drupal_render($form['products']);

  $this_step = magento_cart_get_step();
  $variables['cart_navigation'] = theme('magento_cart_nav', $this_step['step_id']);
  $variables['cart_items'] = $rows;
  $variables['cart_header'] = $header;
  $variables['products_list'] = theme('table', $header, $rows, array('class' => 'magento-cart-quote-table'));

}