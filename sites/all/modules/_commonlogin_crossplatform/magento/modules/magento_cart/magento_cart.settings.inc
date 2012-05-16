<?php
// $Id$
/**
 * @file
 * magento_cart_settings form
 */

function magento_cart_settings() {
  $form = array();

  $form['magento_cart'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General'),
    '#collapsible' => FALSE,
    '#weight'      => 0,
  );

  $form['magento_cart']['magento_cart_ttl'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Time To Live (in hours)'),
    '#description'   => t('For how many hours after the user\' logout the cart should be valid'),
    '#default_value' => variable_get('magento_cart_ttl', 48)
  );

  $form['magento_cart']['magento_cart_content_selector'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Content selector'),
    '#description'   => t("jQuery selector to define the page's content area on this theme. (Used to display cart messages from AJAX response on all pages )."),
    '#default_value' => variable_get('magento_cart_content_selector', '#content-area')
  );

  $form = system_settings_form($form);
  return $form;
}
