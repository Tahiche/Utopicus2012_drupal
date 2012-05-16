<?php
// $Id$

/**
 * @file
 *   Block callbacks.
 */

/**
 * Currency selection block builder.
 *
 * Will build block only if current store actually allow any currency selection.
 */
function magento_currency_block_select() {

  // Add js to page.
  drupal_add_js(drupal_get_path('module', 'magento_currency') .'/magento_currency.form_select.js');

  // Get store and other info.
  $mid = array_shift(_magento_structure_current_store());

  $options = array();
  $base = magento_currency_get_base($mid);
  $allowed = magento_currency_get_allowed($mid);

  foreach ($allowed as $cur_code) {
    $info = magento_currency_get_info($cur_code);
    $options[$cur_code] = t('!currency_name - !currency_code',
      array('!currency_name' => $info['name'], '!currency_code' => $cur_code)
    );
  }

  $current = !empty($_SESSION['magento_currency_current'][$mid])
    ? $_SESSION['magento_currency_current'][$mid]
    : $base;

  // Only fill block if selection available.
  if (count($options) > 1) {
    $subject = t('Select Your Currency');
    $content = drupal_get_form('magento_currency_form_select', $options, $current);
  }
  else {
    $subject = '';
    $content = '';
  }

  return array(
    'subject' => $subject,
    'content' => $content,
  );
}