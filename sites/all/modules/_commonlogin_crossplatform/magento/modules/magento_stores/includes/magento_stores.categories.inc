<?php
// $Id$
/**
 * @file
 * categories
 */
require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';


/**
 * Update single product in all stores
 */
function magento_stores_categories_update_batch_create(&$params) {
  if (!isset($params['current'])) {
    $params['current'] = 0;
    foreach ($params['data'] as $data) {
      $params['stores'][] = $data['store'];
    }
  }

  if (!isset($params['data'][$params['current']])) {
    $params['finished'] = 1;
  }
  else {
    $params['categories'] = &$params['data'][$params['current']]['tree'];
    magento_stores_categories_fetch_batch_create($params);
  }
}

/**
 *
 */
function magento_stores_categories_delete_batch_create(&$params) {
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    if (0 == strncmp($vocabulary->name, MAGENTO_STORES_VOCABULARY_CATEGORIES, drupal_strlen(MAGENTO_STORES_VOCABULARY_CATEGORIES))) {
      // Yes, this is our vocabulary :) - let's remove it
      taxonomy_del_vocabulary($vocabulary->vid);
      $params['message'] = t('Magento Categories have been removed.');
      // we have to rebuild Taxonomy menu
      menu_rebuild();
    }
  }

  $params['finished'] = 1;
}

/**
 *
 */
function magento_stores_categories_get($root, &$ready) {
  if (is_array($root)) {
    $ready[] = array(
      'description' => $root['category_id'],
      'name'        => $root['name'],
      'parent'      => $root['parent_id'],
      'weight'      => empty($root['position']) ? 0 : $root['position'],
    );
    if (is_array($root['children'])) {
      foreach ($root['children'] as $child) {
        magento_stores_categories_get($child, $ready);
      }
    }
  }
}

/**
 *
 */
function magento_stores_categories_fetch_batch_create(&$params) {
  if (empty($params['cats'])) {
    $params['terms'] = 0;
    $params['cats']  = array();
    magento_stores_categories_get($params['categories'], $params['cats']);
  }

  $vocabulary_name = MAGENTO_STORES_VOCABULARY_CATEGORIES ." ({$params['stores'][$params['current']]})";

  $processed = magento_update_vocabulary($vocabulary_name, $params['cats'], TRUE, 50);
  $params['message'] = t("Processing vocabulary '%name': %terms terms updated.", array('%name' => MAGENTO_STORES_VOCABULARY_CATEGORIES ." ({$params['stores'][$params['current']]})", '%terms' => $params['terms'] += $processed));

  if (0 == $processed) {
    $params['cats']     = array();

    // Check again if there are still no Magento Stores
    if (!empty($params['stores'])) {
      $params['finished'] = (float) (++$params['current']) / (int)count($params['stores']);
    }
    else {
      // Do nothing - There are no Magento Stores in Drupal yet
      $params['finished'] = 1;
    }

    $params['message']  = t("Vocabulary '%name': processed.", array('%name' => $vocabulary_name));

    // update vocabulary - assign node_types
    $vocabulary = magento_get_vocabulary($vocabulary_name);
    magento_alter_vocabulary_assign_content_types($vocabulary, magento_stores_get_product_content_types());
  }
}

/**
 *
 */
function magento_stores_categories_fetch_batch_get(&$params) {
  if (!$params['current']) {
    $params['current'] = 0;
  }

  // If Magento Stores had been empty when the batch was created try to fill them now
  if (empty($params['stores'])) {
    // Get stores list
    $local_stores = magento_stores_get_stores();
    foreach ($local_stores as $store) {
      $params['stores'][] = $store['field_magento_id_value'];
    }
  }

  $params['categories'] = magento_api_get_categories($params['stores'][$params['current']]);
}

