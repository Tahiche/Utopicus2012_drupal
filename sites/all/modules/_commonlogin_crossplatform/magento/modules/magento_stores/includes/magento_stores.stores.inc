<?php
// $Id$
/**
 * @file
 * stores
 */
require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';

/**
 *
 */
function magento_stores_stores_delete_batch_create(&$params) {
  // remove main nodes
  foreach (array(MAGENTO_STORES_CONTENT_TYPE_STORE, MAGENTO_STORES_CONTENT_TYPE_GROUP, MAGENTO_STORES_CONTENT_TYPE_WEBSITE) as $type) {
    $nodes = db_query("SELECT nid FROM {node} WHERE type = '%s'", $type);
    while ($nid = db_fetch_array($nodes)) {
      magento_node_delete($nid['nid']);
    }
  }
  $params['finished'] = 1;
  $params['message'] = t('Websites, groups and stores have been removed.');
}

/**
 *
 */
function magento_stores_stores_fetch_batch_create(&$params) {
  if (0 == (float)$params['finished']) {
    $params['steps']   = magento_stores_array_count($params['tree']);
    $params['current'] = 0;
  }

  // NOTE: We won't have here too many shops and websites, so this operation is quite rare.
  //       That's why we can do this in a loop inside every iteration
  $params['current'] += $iteration = 1;
  $params['finished'] = (float) $params['current'] / $params['steps'];

  foreach ($params['tree'] as $website) {
    if ($iteration++ == $params['current']) {
      magento_stores_website_update($website);
      $params['message'] = t("Website '%website' has been created.", array('%website' => $website['name']));
      return;
    }
    foreach ($website[MAGENTO_STORES_GROUPS] as $group) {
      if ($iteration++ == $params['current']) {
        magento_stores_group_update($group);
        $params['message'] = t("Group '%group' has been created.", array('%group' => $group['name']));
        return;
      }
      foreach ($group[MAGENTO_STORES_STORES] as $store) {
        if ($iteration++ == $params['current']) {
          magento_stores_store_update($store);
          $params['message'] = t("Store '%store' has been created.", array('%store' => $store['name']));
          return;
        }
      }
    }
  }

  $params['finished'] = 1;
}

/**
 * Get Stores Tree for synchronization.
 */
function magento_stores_stores_fetch_batch_get(&$params) {
  if (0 == $params['finished']) {
    $params['tree'] = magento_api_get_store_tree();
  }
}