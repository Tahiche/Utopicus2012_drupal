<?php
// $Id$
/**
 * @file
 * magento_stores_settings form
 */
function magento_stores_settings() {
  $form = array();

  /*
  $form['magento_cache'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Cache'),
    '#collapsible' => FALSE,
    '#weight'      => 0,
  );
  */

  $form['magento_stores'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General'),
    '#collapsible' => FALSE,
    '#weight'      => 0,
  );

  /*
  $form['magento_cache']['magento_cache_settings'] = array('#type' => 'checkboxes',
    '#title' => t('Magento Caching strategy'),
    '#description'   => t('Cache Product Nodes results found by Magento ID'),
    '#default_value' => variable_get('magento_cache_settings', array('selects')),
    '#options' => array(
      'selects'   => t('SELECT queries'),
    ),
    '#description' => t(''),
  );
  */

  $form['magento_stores']['magento_stores_allow_bundles'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Allow bundles for'),
    '#description'   => t('Enter each attribute set Id on a separate line'),
    '#default_value' => variable_get('magento_stores_allow_bundles', '')
  );

  $form['magento_stores']['magento_stores_allow_configurables'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Allow back links to configurables for'),
    '#description'   => t('Enter each attribute set Id on a separate line'),
    '#default_value' => variable_get('magento_stores_allow_configurables', '')
  );

  $form['magento_stores']['magento_stores_allow_custom_options'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Allow Custom Options for'),
    '#description'   => t('Enter each attribute set Id on a separate line'),
    '#default_value' => variable_get('magento_stores_allow_custom_options', '')
  );

  $form = system_settings_form($form);
  return $form;
}
