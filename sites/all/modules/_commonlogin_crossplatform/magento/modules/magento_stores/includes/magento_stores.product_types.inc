<?php
// $Id$
/**
 * @file
 * product types
 */
require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';

/**
 *
 */
function magento_stores_product_types_delete_batch_create(&$params) {
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    if (0 == strncmp($vocabulary->name, MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES, drupal_strlen(MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES))) {
      // Yes, this is our vocabulary :) - let's remove it
      taxonomy_del_vocabulary($vocabulary->vid);

      // we have to rebuild Taxonomy menu
      $params['message'] = t("Vocabulary '%name' has been removed.", array('%name' => MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES));
      menu_rebuild();
      break;
    }
  }
  $params['finished'] = 1;
}


/**
 *
 */
function magento_stores_product_types_fetch_batch_create(&$params) {
  // One step batch
  $params['finished'] = 1;

  foreach ($params['data']['product_types'] as &$param) {
    $param['description'] = $param['type'];
    unset($param['type']);

    $param['name'] = $param['label'];
    unset($param['label']);
  }

  magento_update_vocabulary(MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES, $params['data']['product_types'], FALSE);

  // update vocabulary - assign node_types
  magento_alter_vocabulary_assign_content_types(magento_get_vocabulary(MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES), magento_stores_get_product_content_types());
}

/**
 *
 */
function magento_stores_product_types_fetch_batch_get(&$params) {
  if (0 == $params['finished']) {
    $params['data']['product_types'] = magento_api_get_product_types();
  }
}

