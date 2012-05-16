<?php
// $Id$
/**
 * @file
 * magento_synchronize_settings form
 */
function magento_synchronize_settings() {
  $form = array();

  $form['magento_synchronize'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General'),
    '#collapsible' => FALSE,
    '#weight'      => 0,
  );

  $form['magento_synchronize']['magento_products_for_autosynch'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Items for one cron synch'),
    '#default_value' => variable_get('magento_products_for_autosynch',   ONE_STEP_PRODUCTS_AMOUNT_CRON)
  );

  $form['magento_synchronize']['magento_products_for_batchsynch'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Items for one batch synch'),
    '#default_value' => variable_get('magento_products_for_batchsynch', ONE_STEP_PRODUCTS_AMOUNT)
  );

  $form['magento_synchronize']['magento_cron_task_timeout'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Cron task timout'),
    '#default_value' => variable_get('magento_cron_task_timeout', LOCK_TIMEOUT_CRON)
  );

  $form = system_settings_form($form);
  return $form;
}