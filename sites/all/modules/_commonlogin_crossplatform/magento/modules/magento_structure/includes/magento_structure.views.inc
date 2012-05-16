<?php
// $Id$

/**
 * @file
 * Implementaions of views hooks
 */

/**
 * Implementation of hook_views_handlers().
 */
function magento_structure_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'magento_structure') . '/includes',
    ),
    'handlers' => array(
      'magento_structure_universal_handler_filter' => array(
        'parent' => 'views_handler_filter'
      ),
      'magento_structure_website_handler_filter' => array(
        'parent' => 'magento_structure_universal_handler_filter'
      ),
      'magento_structure_store_group_handler_filter' => array(
        'parent' => 'magento_structure_universal_handler_filter'
      ),
      'magento_structure_store_handler_filter' => array(
        'parent' => 'magento_structure_universal_handler_filter'
      )
    )
  );
}

/**
 * Implementation of hook_views_data().
 */
function magento_structure_views_data() {
  $product_types = magento_stores_get_product_content_types($all = FALSE, $with_names = FALSE);

  $types = array('website' => t('Website'), 'store_group' => t('Store Group'), 'store' => t('Store'));
  $data = array();
  foreach ($types as $type => $name) {
    $field = "field__{$type}__";
    // a Fix for `Magento Structure Store` filter
    $filter_table = "content_$field";
    if ($type=='store' && count($product_types) == 1) {
      $filter_table = "content_type_" . $product_types[0];
    }

    $data["magento_structure_$field"] = array(
      'table' => array(
        'title' => $name,
        'help' => t('reference to structure'),
        'group' => t('Magento structure'),
        'join' => array(
          'node' => array(
            'table' => $filter_table,
            'left_field' => 'vid',
            'field' => 'vid'
          ),
          'node_revisions' => array(
            'table' => $filter_table,
            'left_field' => 'vid',
            'field' => 'vid'
          )
        )
      ),
      $field . '_nid' => array(
        'filter' => array(
          'field' => $field . '_nid',
          'table' => $filter_table,
          'content_field_name' => $field,
          'handler' => "magento_structure_{$type}_handler_filter",
        )
      )
    );
  }

  return $data;
}
