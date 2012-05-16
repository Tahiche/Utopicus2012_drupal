<?php
// $Id$
/**
 * @file
 * remove broken nodes
 */
require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';

/**
 * Delete nodes without content type from Drupal
 */
function magento_stores_broken_nodes_delete_batch_create(&$params) {
  // list all nodes without content types
  $nodes = db_query("SELECT nid FROM {node} WHERE type IS NULL OR type = '' OR type = 'magento_product_64website' OR type = 'magento_product_64global'");
  while ($nid = db_fetch_array($nodes)) {
    magento_node_delete($nid);
  }

  $params['finished'] = 1;
  $params['message'] = t('Broken nodes have been removed.');
}
