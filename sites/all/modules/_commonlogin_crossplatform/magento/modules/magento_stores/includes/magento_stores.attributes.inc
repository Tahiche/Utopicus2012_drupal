<?php
// $Id$
/**
 * @file
 * attributes
 */
require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';

/**
 *
 */
function magento_stores_attributes_delete_batch_create(&$params) {
  // list all content types
  $result = db_query("DELETE FROM {". MAGENTO_STORES_TABLE_LABELS . "}");
  $result = db_query("DELETE FROM {". MAGENTO_STORES_TABLE_ATTRIBUTES . "}");

  $params['finished'] = 1;
  $params['message'] = t('Attributes have been removed.');
}


/**
 *
 */
function magento_stores_attributes_fetch_batch_create(&$params) {
  $params['finished'] = 1;

  if (variable_get('magento_stores_log', 0)) {
    watchdog('magento_stores', '', '', WATCHDOG_NOTICE);
  }

  if (!isset($params['data']['attributes']) || !is_array($params['data']['attributes']) || 0 == count($params['data']['attributes'])) {
    watchdog('magento_stores', '', '', WATCHDOG_NOTICE);
  }
  else foreach ($params['data']['attributes'] as $attribute) {
    $attribute_id = $attribute['attribute_id'];
    $query = "SELECT * FROM {". MAGENTO_STORES_TABLE_ATTRIBUTES ."} WHERE attribute_id = '%s'";
    $local_attribute = db_fetch_array(db_query($query, $attribute_id));
    if (!$local_attribute) {
       // insert missing attribute
      drupal_write_record(MAGENTO_STORES_TABLE_ATTRIBUTES, $attribute);
      $local_attribute = db_fetch_array(db_query($query, $attribute_id));
    }

    if ($local_attribute) {
      // compare the fields - may be we don't really need to update anything
      $need_update = FALSE;
      foreach ($attribute as $name => $data) {
        if (!isset($local_attribute[$name]) || $local_attribute[$name] != $data) {
          $need_update  = TRUE;
          break;
        }
      }
      if ($need_update) {
        drupal_write_record(MAGENTO_STORES_TABLE_ATTRIBUTES, $attribute, 'attribute_id');
      }

      //----------------------------------
      // check labels for this ATTRIBUTE
      //----------------------------------
      $query = db_query("SELECT * FROM {". MAGENTO_STORES_TABLE_LABELS ."} WHERE object_id = '%s' AND type = '". MAGENTO_STORES_TYPE_ATTRIBUTE ."'", $attribute_id);

      // we don't expect here too many stores - so easy solution for compare
      while ($local_label = db_fetch_array($query)) {
        // if there is no such label - remote it
        $delete = TRUE;

        foreach ($attribute['labels'] as $key => $label) {
          if ($local_label['store'] == $label['store']) {
            if ($local_label['label'] != $label['label']) {
              $local_label['label'] = $label['label'];
              drupal_write_record(MAGENTO_STORES_TABLE_LABELS, $local_label, array('object_id', 'store', 'type'));
            }
            unset($attribute['labels'][$key]);
            $delete = FALSE;
            break;
          }
        }

        if ($delete) {
          db_query("DELETE FROM {". MAGENTO_STORES_TABLE_LABELS ."} WHERE object_id = '%s' AND store = %d AND type = '". MAGENTO_STORES_TYPE_ATTRIBUTE ."'", $attribute_id, $local_label['store']);
        }
      }

      // insert all of the missing labels
      foreach ($attribute['labels'] as $label) {
        $label['type']      = MAGENTO_STORES_TYPE_ATTRIBUTE;
        $label['object_id'] = $attribute_id;
        drupal_write_record(MAGENTO_STORES_TABLE_LABELS, $label);
      }

      //-------------------------------------------
      // check labels for this ATTRIBUTE's OPTIONS
      //-------------------------------------------
      if (isset($attribute['options']) && is_array($attribute['options'])) {
        $options = array();

        foreach ($attribute['options'] as $key => $option) {
          // NOTE: we can't handle array values
          if (!is_array($option['value'])) {
            $options[] = $option['value'];
            $query = db_query("SELECT * FROM {". MAGENTO_STORES_TABLE_LABELS ."} WHERE object_id = '%s' AND parent_id = '%s' AND type = '". MAGENTO_STORES_TYPE_OPTION ."'", $option['value'], $attribute_id);
            // we expect to get several rows - one row for each store
            while ($local_label = db_fetch_array($query)) {
              // if there is no such label - remote it
              $delete = TRUE;

              foreach ($option['label'] as $index => $store) {
                if ($local_label['store'] == $store['store']) {
                  if ($local_label['label'] != $store['label']) {
                    $local_label['label']  = $store['label'];
                    drupal_write_record(MAGENTO_STORES_TABLE_LABELS, $local_label, array('object_id', 'store', 'type'));
                  }
                  unset($attribute['options'][$key]['label'][$index]);
                  $delete = FALSE;
                  break;
                }
              }

              if ($delete) {
                db_query("DELETE FROM {". MAGENTO_STORES_TABLE_LABELS ."} WHERE object_id = %d AND parent_id = '%s' AND store = %d AND type = '". MAGENTO_STORES_TYPE_OPTION ."'", $local_label['object_id'], $attribute_id, $local_label['store']);
              }
            }
          }
        }

        // insert all of the missing labels
        foreach ($attribute['options'] as $option) {
          // NOTE: we can't handle array values
          if (!is_array($option['value'])) {
            foreach ($option['label'] as $label) {
              $row = array_merge($label, array('type' => MAGENTO_STORES_TYPE_OPTION, 'object_id' => $option['value'], 'parent_id' => $attribute_id));
              drupal_write_record(MAGENTO_STORES_TABLE_LABELS, $row);
            }
          }
        }

        /* update field only if it is alread exist */
        // TODO: REmove.
        //$res = magento_field_instance_update("field_{$attribute['attribute_code']}", array('allowed_values' => implode("\n", $options)));
        $php_values = '
          if (function_exists("_magento_store_option_values")) {
            $result = _magento_store_option_values($field["field_name"]);
          }
          else {
            $result = array();
            drupal_set_message(t("Field %field cant access allowed values, contact site administrator",
              array("%field" => $field["field_name"])),
              "error"
            );
          }
          return $result;';

        $res = magento_field_instance_update("field_{$attribute['attribute_code']}", array('allowed_values_php' => $php_values));
      } // e.o. if + foreach
    }
  } // eo foreach

  $params['message'] = t("Attributes' info has been updated");
}

/**
 *
 */
function magento_stores_attributes_fetch_batch_get(&$params) {
  if (0 == $params['finished']) {
    $params['data']['attributes'] = magento_api_get_attributes();
  }
}

