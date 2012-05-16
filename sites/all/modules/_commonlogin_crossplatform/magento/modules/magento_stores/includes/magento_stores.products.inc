<?php
// $Id$
/**
 * @file
 * products
 */

//require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';
module_load_include('strings', 'magento_stores', 'magento_stores');
define('ATTRIBUTES_SYNC_EXECUTED', 2);
/**
 * Update single product in all stores
 */
function magento_stores_products_update_batch_create(&$params) {
  if (is_array($params['data'])) foreach ($params['data'] as $data) {
    magento_stores_product_update($data['product'], $data['store']);
    $params['message'] = t("STORE '%store': Node '%title' has been updated", array('%title' => $data['product']['name'], '%store' => $data['store']));
  }

  $params['finished'] = 1;
}


/**
 * Remove surplus products in one store
 */
function __deprecated_magento_stores_products_remove_surplus_batch_create(&$params) {
  // get products in Magento
  $magento_products = array();
  $products = magento_api_get_products_ids_to_update($params['store_id'], '');

  if (!is_array($products)) {
    $params['finished'] = 1;
    return;
  }

  foreach ($products as $product) {
    $magento_products[] = $product['id'];
  }

  // get products in Drupal
  $query = <<<QUERY
SELECT
  f.nid, f.field_magento_id_value
FROM
  {node} n
    LEFT JOIN {content_field_magento_id} f ON f.nid = n.nid
    LEFT JOIN {content_field_level_id}   s ON s.nid = n.nid
WHERE
  s.field_level_id_value = %d     AND
  n.type LIKE 'magento_product_%' AND
  n.type NOT LIKE '%_website'     AND
  n.type NOT LIKE '%_global'
QUERY;

  $drupal_products = array();
  $res = db_query($query, $params['store_id']);
  while ($mid = db_fetch_array($res)) {
    $drupal_products[$mid['nid']] = $mid['field_magento_id_value'];
  }

  // find absent products
  $remove = array_diff($drupal_products, $magento_products);
  foreach ($remove as $magento_id) {
    magento_stores_product_delete($magento_id, $params['store_id']);
  }

  $params['finished'] = 1;
}

/**
 * Find and sync missed products in one store
 */
function magento_stores_products_sync_missed_batch_create(&$params) {
  // initialize if required
  if (empty($params['limit'])) {
    $params['limit'] = variable_get('magento_products_for_autosynch', ONE_STEP_PRODUCTS_AMOUNT_CRON);
    $params['offset'] = 0;
    $res = magento_api_get_count_products_to_update($params['store_id'], '');
    if (isset($res['count'])) {
      $params['total'] = $res['count'];
    }
  }

  if ($params['offset'] > ($params['total'] + $params['limit'])) {
    $params['finished'] = 1;
    return;
  }

  // get products in Magento
  $magento_products = array();
  $products = magento_api_get_products_ids_to_update($params['store_id'], '', $params['limit'], $params['offset']);

  if (!is_array($products) || empty($products[0])) {
    $params['finished'] = 1;
    return;
  }

  foreach ($products as $product) {
    $magento_products[] = $product['id'];
  }

  // get products in Drupal
  $query = <<<QUERY
SELECT
  f.nid, f.field_magento_id_value
FROM
  {node} n
    LEFT JOIN {content_field_magento_id} f ON f.nid = n.nid
    LEFT JOIN {content_field_level_id}   s ON s.nid = n.nid
WHERE
  s.field_level_id_value = %d     AND
  n.type LIKE 'magento_product_%' AND
  n.type NOT LIKE '%_website'     AND
  n.type NOT LIKE '%_global'      AND
  f.field_magento_id_value IN ( %s )
ORDER BY f.field_magento_id_value asc
QUERY;

  $drupal_products = array();
  $res = db_query($query, $params['store_id'], join(', ' , $magento_products));
  while ($mid = db_fetch_array($res)) {
    $drupal_products[$mid['nid']] = $mid['field_magento_id_value'];
  }

  // find absent products
  $absent = array_diff($magento_products, $drupal_products);

  if (!empty($absent)) {
    $products = magento_api_get_products($params['store_id'], $absent);
    if (is_array($products)) {
      foreach ($products as $product) {
        if (empty($product['name'])) {
          continue;
        }

        if (magento_stores_product_update($product, $params['store_id'])) {
          //$params['message'] = t("STORE '%store': Node '%title' has been updated", array('%title' => $product['name'], '%store' => $params['store']));
        }
      }
    }
  }

  $params['message'] = "Doing limit " . $params['limit'] . " and offset " . $params['offset'];

  $params['offset'] += $params['limit'];
  $params['finished'] = empty($params['total'])? 0.5 : ($params['offset']/$params['total']);
  //$params['finished'] = 0.5;
}

/**
 * Remove surplus products in one store
 */
function magento_stores_products_remove_surplus_batch_create(&$params) {
  // initialize if required
  if (empty($params['limit'])) {
    $params['magento_products'] = array();
    $params['limit'] = variable_get('magento_products_for_autosynch', ONE_STEP_PRODUCTS_AMOUNT_CRON);
    $params['offset'] = 0;
    $res = magento_api_get_count_products_to_update($params['store_id'], '');
    if (isset($res['count'])) {
      $params['total'] = $res['count'];
    }
  }

  if ($params['offset'] > ($params['total'] + $params['limit'])) {
    if ($params['finished'] < 1) {
      _magento_stores_products_remove_surplus($params);
    }
    $params['finished'] = 1;
    return;
  }

  // get products in Magento
  $products = magento_api_get_products_ids_to_update($params['store_id'], '', $params['limit'], $params['offset']);

  if (!is_array($products)) {
    $params['finished'] = 1;
    return;
  }

  foreach ($products as $product) {
    $params['magento_products'][] = $product['id'];
  }

  $params['message'] = "Doing limit " . $params['limit'] . " and offset " . $params['offset'];
  $params['offset'] += $params['limit'];
  $params['finished'] = empty($params['total'])? 0.5 : ($params['offset']/$params['total']);
  if ($params['finished'] >= 1) {
    _magento_stores_products_remove_surplus($params);
  }
}

/**
 * Delete all products from Drupal
 */
function magento_stores_products_delete_batch_create(&$params) {
  if (empty($params['deleted'])) {
    $params['deleted'] = 0;
  }

  if (isset($params['productId']) && NULL !== $params['productId']) {
    magento_stores_product_delete($params['productId']);
  }
  else {
    // list all content types
    $nodes = db_query("SELECT nid FROM {node} WHERE type LIKE '%s%%'", MAGENTO_STORES_CONTENT_TYPE_PRODUCT);

    $limit = variable_get('magento_delete_products_batch_limit', 1000);
    $count_deleted = 0;
    while ($nid = db_fetch_array($nodes)) {
      // update autosynch
      if ($limit > $count_deleted++) {
        node_delete($nid);
      }
      else {
        $params['deleted'] += $limit;
        $params['finished'] = 0.5;
        $params['message'] = t('%deleted nodes have been removed.', array('%deleted' => $params['deleted']));
        return;
      }
    }
  }

  $params['finished'] = 1;
  $params['message'] = t('Products have been removed.');
}


/**
 * Remove single product from all stores
 *   if $the_store_id is set, then product will be removed for this single store only, otherwise -
 *   it will be removed for all stores
 */
function _magento_stores_product_delete($the_product_id, $the_store_id = NULL) {
  $types = _magento_stores_get_product_content_types(FALSE, TRUE);
  $nids  = array();

  // Then we should find out - is this the only store for this product?
  $nodes = magento_check_node($the_product_id);
  foreach ($nodes as $item) {
    if (isset($types[$item['type']]) && isset($item['field_level_id_value'])) {
      $nids[$item['field_level_id_value']] = $item['nid'];
    }
  }

  // nodes require update
  $linked_nids = array();

  $global_nid   = NULL;
  $website_nids = array();

  foreach ($nids as $store => $nid) {
    if (!$the_store_id || $store == $the_store_id) {
      $node = node_load($nid);

      if (!$global_nid) {
        // create full tree
        if (isset($node->field__parent__[0]['nid'])) {
          $website_nid = $node->field__parent__[0]['nid'];
          $website = node_load($website_nid);

          if (isset($website->field__parent__[0]['nid'])) {
            $global_nid = $website->field__parent__[0]['nid'];
          }
        }
      }

      // find website_node
      // $website_node = node_load($node->field__parent__[0]['nid']);

      // before deleting any node - we should remove all the links

      // website-level node
      magento_node_delete($node->field__parent__[0]['nid']);

      // store-level node
      magento_node_delete($nid);
  }
  }

  // global- level nodes
  if ((!$the_store_id || 1 == count($nids) && isset($nids[$the_store_id])) && $global_nid) {
    magento_node_delete($global_nid);
  }
}


/**
 * Update single product in one store
 */
function _magento_stores_product_update($product, $the_store_id) {
  static $attributes_sync_required;

  //$updated = FALSE;
  $supported_types = array('simple', 'virtual', 'configurable', 'bundle');

  $start = magento_microtime_float();
  if (!$product || !is_array($product) || !(in_array($product['type'], $supported_types))) {
    return;
  }

  $nodes = array('store' => magento_get_node(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $product['set'], $product['product_id'], $product['name'], $the_store_id));

  if (!$nodes['store']) {
    return;
  }

      $node_store = &$nodes['store'];

      $taxonomies  = array();
      $need_update = array();

      /* check and update Store Node linked to this product */
      // 1. Translate StoreId into Node
      $store = magento_get_node(MAGENTO_STORES_CONTENT_TYPE_STORE, $the_store_id);

      // 2. Compare current value with the new one + Check store group & website
      if ($node_store->field__store__[0]['nid'] != $store->nid || !$node_store->field__store_group__[0]['nid'] || !$node_store->field__website__[0]['nid']) {
        $node_store->field__store__[0]['nid'] = $store->nid;

        $store_group = node_load($store->field__store_group__[0]['nid']);
        $node_store->field__store_group__[0]['nid'] = $store_group->nid;
        $node_store->field__website__[0]['nid'] = $store_group->field__website__[0]['nid'];
        $need_update['store'] = TRUE;
      }

      /* compare local attribute set and available data */
      // Let's create a light copy of the product
      $product_light = array();
      foreach ($product as $key => $data) {
        if (!is_array($data)) {
          $product_light[$key] = 0;
        }
      }

      $query = db_query('SELECT '. implode(",", array(MAGENTO_ATTRIBUTE_CODE, MAGENTO_STORES_TYPE)) .' FROM {'. MAGENTO_STORES_TABLE_ATTRIBUTE_SETS .'} WHERE '. MAGENTO_STORES_TYPE ." = '%s' OR ". MAGENTO_STORES_TYPE ." = '%s_global' OR ". MAGENTO_STORES_TYPE ." = '%s_website'", $nodes['store']->type, $nodes['store']->type, $nodes['store']->type);

      while ($attribute = db_fetch_array($query)) {
        $product_light[$attribute[MAGENTO_ATTRIBUTE_CODE]] = (isset($product_light[$attribute[MAGENTO_ATTRIBUTE_CODE]])) ? 1 : $attribute[MAGENTO_STORES_TYPE];
      }

      // if we have attributes which do not exist any more in the product - then remove it
      foreach ($product_light as $attribute => $data) {
        switch ($data) {
          // new attribute
          case '0':
          case '1':
            break;

            // removed attribute
          default:
            magento_remove_cck_field($data, 'field_'. $attribute);
            $query = db_query('DELETE FROM {'. MAGENTO_STORES_TABLE_ATTRIBUTE_SETS .'} WHERE '. MAGENTO_STORES_TYPE ." = '%s' AND ". MAGENTO_ATTRIBUTE_CODE ." = '%s'", $data, $attribute);
            break;
        }
      }

      // Media gallery
      if (isset($product[MAGENTO_API_MEDIA_GALLERY]) && is_array($product[MAGENTO_API_MEDIA_GALLERY])) {
        $gallery = &$product[MAGENTO_API_MEDIA_GALLERY];

        // Save all images
        $gallery = serialize($gallery);
        if (empty($nodes['store']->field_image_gallery[0]['value']) || ($nodes['store']->field_image_gallery[0]['value'] != $gallery)) {
          $nodes['store']->field_image_gallery[0]['value'] = $gallery;
          $need_update['store'] = TRUE;
        }
      }

      // CUSTOM_OPTIONS
      if (magento_stores_is_custom_options_allowed($product['set']) && isset($product['options']) && is_array($product['options'])) {
        $custom_options = &$product['options'];

        // Save CUSTOM_OPTIONS
        $custom_options = serialize($custom_options);
        if (empty($nodes['store']->field_custom_options_data[0]['value']) || ($nodes['store']->field_custom_options_data[0]['value'] != $custom_options)) {
          $nodes['store']->field_custom_options_data[0]['value'] = $custom_options;
          $need_update['store'] = TRUE;
        }
      }

      // if this node has no website -> create one
      if (empty($nodes['store']->field__parent__[0]['nid'])) {
    // determine Website id
        $store_group  = node_load($store->field__store_group__[0]['nid']);
        $website_node = node_load($store_group->field__website__[0]['nid']);

        $nodes['website'] = magento_get_node(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $product['set'] .'_website', $product['product_id'], $product['name'], $website_node->field_magento_id[0]['value']);
        $nodes['store']->field__parent__ = array(array('nid' => $nodes['website']->nid));
        $need_update['store'] = TRUE;
      }
      else {
        $nodes['website'] = node_load($nodes['store']->field__parent__[0]['nid']);
      }

      // if this node has no global -> create one
      if (empty($nodes['website']->field__parent__[0]['nid'])) {
        $nodes['global'] = magento_get_node(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $product['set'] .'_global', $product['product_id'], $product['name']);
        $nodes['website']->field__parent__ = array(array('nid' => $nodes['global']->nid));
        $need_update['website'] = TRUE;
      }
      else {
        $nodes['global'] = node_load($nodes['website']->field__parent__[0]['nid']);
      }

      foreach ($product as $key => $value) {
        $cck_field = 'field_'. $key;
        switch ($key) {
          // predefined attributes
          case 'bundle_data':
            if (magento_stores_is_bundle($product['set'])) {
              $remote_products = array();
              $drupal_bundle_product_nodes = array();
              $bundle_product_nodes = array();

              // get existing nodes
              foreach ($nodes['store']->field__bundle_links__ as $drupal_bundle_product) {
                $drupal_bundle_product_nodes[] = $drupal_bundle_product['nid'];
              }

              if (isset($value['selected_products']) && is_array($value['selected_products'])) foreach ($value['selected_products'] as $selected_products) {
                if (is_array($selected_products)) foreach ($selected_products as $selected_product) {
                  $remote_products[$selected_product['product_id']] = $selected_product['set'];
                }
              }

              // get all the linked Drupal nodes
              foreach ($remote_products as $remote_product => $set) {
                $product_node = magento_get_node(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $set, $remote_product, NULL, $the_store_id);
                if ($product_node) {
                  $bundle_product_nodes[] = $product_node->nid;
                }
              }

              // compare with existing values
              $items_to_remove = array_diff($drupal_bundle_product_nodes, $bundle_product_nodes);
              $items_to_add    = array_diff($bundle_product_nodes, $drupal_bundle_product_nodes);

              if (0 < count($items_to_remove) || 0 < count($items_to_add)) {
                $array = array();
                foreach ($bundle_product_nodes as $bundle_node) {
                  $array[] = array('nid' => $bundle_node);
                }
                $nodes['store']->field__bundle_links__ = $array;
                $need_update['store'] = TRUE;
              }
            }
            break;
          // predefined attributes
          case MAGENTO_STORES_LINKED_PRODUCTS:
            if (is_array($value)) foreach ($value as $linked_product_type => $products) if (is_array($products) && 0 < count($products)) {
              $linked_product_nodes        = array();
              $drupal_linked_product_nodes = array();

              // THE name of the linked_product_type_links field by `linked_product_type`
              $field__linked_product_type_links__ = _magento_stores_get_linked_products_content_field_name($linked_product_type);

              foreach ($nodes['store']->$field__linked_product_type_links__ as $drupal_linked_product) {
                $drupal_linked_product_nodes[] = $drupal_linked_product['nid'];
              }

              //$products = magento_store_resort_linked_by_position($products);

              // get all the linked Drupal nodes
              foreach ($products as $linked_product) {
                if (empty($linked_product)) {
                  continue;
                }

                $linked_node = magento_get_node(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $linked_product['set'], $linked_product['product_id'], $linked_product['name'], $the_store_id);
                if ($linked_node) {
                  $linked_product_nodes[] = $linked_node->nid;
                }
              }

              // compare with existing values
              foreach ($linked_product_nodes as $l_key => $linked_node) {
                $array[] = array('nid' => $linked_node);
                if ( $linked_node != $drupal_linked_product_nodes[$l_key] ) {
                  $need_update['store'] = TRUE;
                }
              }

              if (empty($need_update['store']) && (count($drupal_linked_product_nodes) != count($linked_product_nodes))) {
                $need_update['store'] = TRUE;
              }

              $nodes['store']->$field__linked_product_type_links__ = $array;

              /*
              $items_to_remove = array_diff($drupal_linked_product_nodes, $linked_product_nodes);
              $items_to_add    = array_diff($linked_product_nodes, $drupal_linked_product_nodes);

              if (0 < count($items_to_remove) || 0 < count($items_to_add)) {
                $array = array();
                foreach ($linked_product_nodes as $linked_node) {
                  $array[] = array('nid' => $linked_node);
                }
                $nodes['store']->$field__linked_product_type_links__ = $array;
                $need_update['store'] = TRUE;
              }
              */
            }
            break;

          // TODO: I don't like this:
          case 'type':
          case 'updated_at':
            break;

          case 'status':
            // convert into Drupal format (1 = enabled, 2 = disabled)
            if (1 != $value) {
              $value = 0;
            }

      /* We should publish/unpublish only the STORE node */
            if ($nodes['store']->status != $value) {
              $nodes['store']->status = $value;
              $need_update['store'] = TRUE;
            }

      /* Other nodes should be always published */
            if ($nodes['website']->status != 1) {
              $nodes['website']->status = 1;
              $need_update['website'] = TRUE;
            }

            if ($nodes['global']->status != 1) {
              $nodes['global']->status = 1;
              $need_update['global'] = TRUE;
            }

            break;

          case 'name':
            if (empty($nodes['website']->title) && $nodes['website']->title != $value) {
              $nodes['website']->title = $value;
              $need_update['website'] = TRUE;
            }
            if (empty($nodes['global']->title) && $nodes['global']->title != $value) {
              $nodes['global']->title = $value;
              $need_update['global'] = TRUE;
            }
            if ($nodes['store']->title != $value) {
              $nodes['store']->title = $value;
              $need_update['store'] = TRUE;
            }
            // WARNING! no break!

          default:
            if (!is_array($value)) {
              $attributes = magento_stores_product_get_attributes();
              $scope = isset($attributes[$key]) ? $attributes[$key][MAGENTO_API_ATTRIBUTE_SCOPE] : '';

              ////////////////////////////////////////////////////////////////////////////////////////
              /// Check if it is required to sync attributes
              $is_select_option = isset($attributes[$key]) && $attributes[$key]['frontend_input'] == 'select' && $attributes[$key]['backend_type'] == 'int';
              if (!empty($value) && empty($attributes_sync_required) && $is_select_option) {
                // also check for backlist_attributes
                if (_magento_stores_get_attribute_options_label($the_store_id, $value) === FALSE) {
                  $attributes_sync_required = TRUE;
                }
              }
              /// end Check
              ////////////////////////////////////////////////////////////////////////////////////////

              if (empty($scope)) {
                switch ($key) {
                  case 'type_id':
                    static $product_types_vocabulary = NULL;

                    if (!$product_types_vocabulary) {
                      $product_types_vocabulary = magento_get_vocabulary(MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES);
                    }

                    $taxonomies[$product_types_vocabulary->vid][$value] = NULL;
                    break;
                }
              }
              else {
                if (!isset($nodes[$scope]->$cck_field)) {
                  $nodes[$scope]->$cck_field = array(array());
                }
                if ($attributes[$key]['cck_fill_function']($nodes[$scope]->{$cck_field}, $value)) {
                $need_update[$scope] = TRUE;
                }
              }
            }
            else {
              if ($key == 'categories') {
                static $product_categories_vocabulary = array();

                if (!isset($product_categories_vocabulary[$the_store_id])) {
                  $product_categories_vocabulary[$the_store_id] = magento_get_vocabulary(MAGENTO_STORES_VOCABULARY_CATEGORIES ." ({$the_store_id})");
                }

                foreach ($value as $category) {
                  $taxonomies[$product_categories_vocabulary[$the_store_id]->vid][$category] = NULL;
                }
              }
            }

        }
      }

      if ($product['type'] == 'configurable') {
        if (isset($product['configurable_data'])) {
          if (isset($product['configurable_data']['products'][0]) && is_array($product['configurable_data']['products'][0])) {
            $exclude = array();
            foreach ($product['configurable_data']['products'][0] as $child_product_id) {

              // check for product existance
              $child_node  = array();
          $child_nodes = magento_check_node($child_product_id, $the_store_id);

          foreach ($child_nodes as $child_item) {
            if (0 == strncmp($child_item['type'], MAGENTO_STORES_CONTENT_TYPE_PRODUCT, drupal_strlen(MAGENTO_STORES_CONTENT_TYPE_PRODUCT))) {
              $child_node = $child_item;
              break;
            }
          }

          // if no node found - get it from - but we don't expect to get here
          if (!isset($child_node['nid'])) {

            $tmp_products = magento_api_get_products($the_store_id, array($child_product_id));
            if (is_array($tmp_products)) foreach ($tmp_products as $tmp_product) {
              magento_stores_product_update($tmp_product, $the_store_id);
            }

            // search one more time
            $child_nodes = magento_check_node($child_product_id, $the_store_id);

            foreach ($child_nodes as $child_item) {
              if (0 == strncmp($child_item['type'], MAGENTO_STORES_CONTENT_TYPE_PRODUCT, drupal_strlen(MAGENTO_STORES_CONTENT_TYPE_PRODUCT))) {
                $child_node = $child_item;
                break;
              }
            }
          }

          // if we still have no child product - this is definitely some error
          if (isset($child_node['nid'])) {

            // load this node
            $child = magento_get_node($child_node['type'], $child_product_id, '', $the_store_id);

            // get attribute set id
            $child_attribute_set_id = drupal_substr($child->type, drupal_strlen(MAGENTO_STORES_CONTENT_TYPE_PRODUCT));
            if (magento_stores_is_configurable($child_attribute_set_id)) {
              // backlinking is enabled
              if ($child->field__configurable_links__[0]['nid'] != $nodes['store']->nid) {
                $child->field__configurable_links__ = array(array('nid' => $nodes['store']->nid));
                magento_node_save($child);
              }

              $exclude[] = $child->nid;
            }
          }
        }


          // remove from all nodes other then selected
          $allowed_configurables = magento_stores_allowed_configurables();
          if (!empty($allowed_configurables[0])) {
            $sql = 'SELECT nid FROM {content_field__configurable_links__} WHERE field__configurable_links___nid = %d';
            if (0 < count($exclude)) {
              $sql .= ' AND nid NOT IN (' . implode(",", $exclude) . ')';
            }

            $query = db_query($sql, $nodes['store']->nid);

            while ($remove_from_node = db_fetch_array($query)) {
              $new_links      = array();
              $node_to_remove = node_load($remove_from_node['nid']);

              if (is_array($node_to_remove->field__configurable_links__)) foreach ($node_to_remove->field__configurable_links__ as $configurable_link) {
                if ($configurable_link['nid'] != $nodes['store']->nid) {
                  $new_links[] = $configurable_link;
                  break;
                }
              }

              $node_to_remove->field__configurable_links__ = $new_links;

              node_save($node_to_remove);
            }
          }

          }//end of : if (isset($product['configurable_data']['products'][0]) && is_array($product['configurable_data']['products'][0]))

          if (isset($product['configurable_data']['full_data'])) {
            foreach ($product['configurable_data']['full_data']['attributes'] as $attr_key => $attr) {
              foreach ($attr['options'] as $opt_key => $opt) {
                $prod_nids = array();
                foreach ($opt['products'] as $prod_key => $prod) {
                  //TODO: here we load or create new child node with same content type with configurable parent
                  // maybe this can be wrong for contribute.
                  $type = MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $product['set'];
                  $prod_nid = magento_get_node($type, $prod, '', $the_store_id);
                  $prod_nids[$prod_key] = $prod_nid->nid;
                }
                $product['configurable_data']['full_data']['attributes'][$attr_key]['options'][$opt_key]['products_nids'] = $prod_nids;
              }
            }
          }
          $nodes['store']->field_type_specific_data[0]['value'] = serialize($product['configurable_data']);
          $need_update['store'] = TRUE;
        }
      }

      // Compare taxonomies
      $local_taxonomies  = array();
      if (isset($nodes['store']->taxonomy) && is_array($nodes['store']->taxonomy)) foreach ($nodes['store']->taxonomy as $taxonomy_item) {
        $local_taxonomies[$taxonomy_item->vid][$taxonomy_item->description] = $taxonomy_item->tid;
      }

      $update_taxonomies = !(count($taxonomies) == count($local_taxonomies));
      foreach ($taxonomies as $vid => $terms) {
        $local_count = 0;
        if (isset($local_taxonomies[$vid])) {
          $local_count = count($local_taxonomies[$vid]);
        }

        $update_taxonomies |= !(count($terms) == $local_count);
        foreach ($terms as $id => $term) {
          if (isset($local_taxonomies[$vid][$id])) {
            $taxonomies[$vid][$id] = $local_taxonomies[$vid][$id];
          }
          else {
            $update_taxonomies |= TRUE;
          }
        }
      }

      if ($update_taxonomies) {
        foreach ($taxonomies as $vid => $terms) {
          foreach ($terms as $id => $term) {
            if (0 < $term) {
                $taxonomies_new[] = $term;
            }
            else {
                // let's find this term's id
          $tid = db_result(db_query("SELECT tid FROM {term_data} WHERE vid = %d AND description = '%s'", $vid, $id));
          if ($tid) {
                  $taxonomies_new[] = $tid;
                  }
                  else {
                    // we might need to update TAXONOMIES
                  }
            }
          }
        }

      $nodes['store']->taxonomy = $taxonomies_new;

      // WARNING! We do not call here taxonomy_node_save - because it is done in taxonomy_nodeapi
      //   taxonomy_node_save($nodes['store'], $taxonomies_new);

      if (!isset($need_update['store']) || !$need_update['store']) {
        /* WARNING! Code from node_save() function
        *   we call all the hooks like the node was updated
        */

        // Call the node specific callback (if any).
        node_invoke($nodes['store'], 'update');
        node_invoke_nodeapi($nodes['store'], 'update');
      }
    }

      foreach ($need_update as $scope => $value) {
        if ($value) {
          //$updated = TRUE;
          magento_node_save($nodes[$scope]);
        }
      }
    //}
  //}

  if (!empty($attributes_sync_required)&& $attributes_sync_required!==ATTRIBUTES_SYNC_EXECUTED) {
    ob_start(); // run attributes sync in background without output
    magento_stores_synchronize_attributes();
    $attributes_sync_required = ATTRIBUTES_SYNC_EXECUTED;
    ob_end_clean();
  }

  return TRUE;
}

/**
 * Update products
 *
 */
function magento_stores_products_fetch_batch_create(&$params) {
  if (0 == count($params['products'])) {
    $params['finished'] = 1;
    $params['message']  = t("There are no products for update.");
  }
  else {
    $products = magento_api_get_products($params['store_id'], $params['products']);
    if (is_array($products)) {
      if (0 == count($products)) {
        $params['finished'] = 1;
        return;
      }
      foreach ($products as $product) {
        $time_start = microtime(true);
        if (magento_stores_product_update($product, $params['store_id'])) {
          $params['message'] = t("STORE '%store': Node '%title' has been updated", array('%title' => $product['name'], '%store' => $params['store']));
        }
      }
    }
    $params['current'] += count($products);
    $params['finished'] = (float) $params['current'] / (int)$params['steps'];
  }
}

/**
 *
 * On start we don't know how many products we will get
 */
function magento_stores_products_fetch_batch_get(&$params) {
  if (!$params['finished']) {
    // try to get the number of products we will have
    $info = magento_stores_get_info(array(MAGENTO_STORES_PRODUCTS => array('store' => $params['store_id'])));
    if (is_array($info) && is_array($info[MAGENTO_STORES_PRODUCTS])) {
      $params['steps'] = $info[MAGENTO_STORES_PRODUCTS]['remote_count'];
    }

    $params['current'] = 0;
  }

  // get new portion of products
  $params['products'] = array();
  $data = magento_api_get_products_ids_to_update($params['store_id'], NULL, variable_get('magento_products_for_batchsynch', ONE_STEP_PRODUCTS_AMOUNT), $params['current']);
  if (is_array($data)) {
    foreach ($data as $product) {
      $params['products'][] = $product['id'];
    }
  }
}

/**
 * Actually remove surplus products.
 * Helper for magento_stores_products_remove_surplus_batch_create($params)
 *
 * @see magento_stores_products_remove_surplus_batch_create()
 */
function _magento_stores_products_remove_surplus(&$params) {
  if (!is_array($params['magento_products'])) {
    $params['finished'] = 1;
    return;
  }

  // get products in Drupal
  $query = <<<QUERY
SELECT
  f.nid, f.field_magento_id_value
FROM
  {node} n
    LEFT JOIN {content_field_magento_id} f ON f.nid = n.nid
    LEFT JOIN {content_field_level_id}   s ON s.nid = n.nid
WHERE
  s.field_level_id_value = %d     AND
  n.type LIKE 'magento_product_%' AND
  n.type NOT LIKE '%_website'     AND
  n.type NOT LIKE '%_global'
QUERY;

  $drupal_products = array();
  $res = db_query($query, $params['store_id']);
  while ($mid = db_fetch_array($res)) {
    $drupal_products[$mid['nid']] = $mid['field_magento_id_value'];
  }

  // find absent products
  $remove = array_diff($drupal_products, $params['magento_products']);
  foreach ($remove as $magento_id) {
    magento_stores_product_delete($magento_id, $params['store_id']);
  }

  $params['finished'] = 1;
}