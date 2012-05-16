<?php
// $Id$
/**
 * @file
 * attribute sets
 */
require_once drupal_get_path('module', 'magento_stores') . '/magento_stores.strings';

/**
 *
 */
function magento_stores_attribute_sets_delete_batch_create(&$params) {
  if (0 == $params['finished']) {
    $types = node_get_types();
    $length = drupal_strlen(MAGENTO_STORES_CONTENT_TYPE_PRODUCT);

    foreach ($types as $type => $data) {
      if (MAGENTO_STORES_CONTENT_TYPE_PRODUCT == drupal_substr($type, 0, $length)) {
        $ending = drupal_substr($type, $length);
        if (ctype_digit($ending)) {
          $params['types'][$type][$type] = $data->name;
        }
        else {
          $params['types'][drupal_substr($type, 0, strrpos($type, '_'))][$type] = $data->name;
        }
        $params['total'] += 1;
      }
    }

    $params['current'] = 0;
  }

  $params['current'] += 1;

  // list all product types
  if (is_array($params['types'])) {
    foreach ($params['types'] as $main_type => $data) {
    foreach ($data as $type => $name) {
      if ($main_type != $type || 1 == count($data)) {
        magento_stores_product_type_delete($type, $main_type);
        unset($params['types'][$main_type][$type]);

        $params['message'] = t("Content Type '%name' has been removed.", array('%name' => $name));

        if ($main_type == $type) {
          unset($params['types'][$main_type]);
        }
        break;
      }
    }
    break;
    }
  }

  $params['finished'] = (0 == count($params['types'])) ? 1 : ($params['current'] / $params['total']);
}

/**
 *
 */
function magento_stores_attribute_sets_fetch_batch_create(&$params) {
  if (0 == $params['finished']) {
    $params['total']   = 18 + count($params['data']['attributes']); // HARDCODED number of steps
    $params['current'] = 0;
  }

  $params['message']  = magento_stores_product_type_update($params['data'], $params['current'] += 1, $params['total']);
  $params['finished'] = (float) $params['current'] / (int)$params['total'];
}

/**
 *
 */
function magento_stores_attribute_sets_fetch_batch_get(&$params) {
  if (!$params['finished']) {
    $params['data']['attributes'] = magento_api_get_attribute_set_attributes($params['data'][MAGENTO_API_ATTRIBUTESET_ID]);
  }
}

/**
 * WARNING: hardcoded number of steps
 */
function _magento_stores_product_type_update($attribute_set, $current, $total) {
  $message = '';
  switch ($current) {
    case 1: // create global
      $message = magento_add_content_type(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_global',  "{$attribute_set[MAGENTO_API_ATTRIBUTESET_NAME]} (global)");
      break;

    case 2: // create website
      $message = magento_add_content_type(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_website', "{$attribute_set[MAGENTO_API_ATTRIBUTESET_NAME]} (website)");
      break;

    case 3: // add global reference field to website
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_website', "field__parent__", 'Parent global', 'nodereference', 'nodereference_autocomplete');
      magento_field_instance_update('field__parent__', array('referenceable_types' => array(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_global' => MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_global')));
      break;

    case 4: // create store
      $message = magento_add_content_type(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], $attribute_set[MAGENTO_API_ATTRIBUTESET_NAME]);
      break;

    case 5: // add website reference field to store
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field__parent__', 'Parent website node', 'nodereference', 'nodereference_autocomplete');
      magento_field_instance_update('field__parent__', array('referenceable_types' => array(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_website' => MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_website')));
      break;

    case 6: // remote object id (store)
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], "field_magento_id", 'Magento product id', 'number_integer', 'number');
      break;

    case 7: // remote object id (website)
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_website', "field_magento_id", 'Magento product id', 'number_integer', 'number');
      break;

    case 8: // remote object id (global)
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_global', "field_magento_id", 'Magento product id', 'number_integer', 'number');
      break;

    case 9: // remote Store id
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], "field_level_id", 'Magento Store id', 'number_integer', 'number');
      break;

    case 10: // remote Website id
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] .'_website', "field_level_id", 'Magento Website id', 'number_integer', 'number');
      break;

    case 11: // update 'Magento Categories' vocabularies by adding this content type
      $type_name     = MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID];

      $vocabularies = taxonomy_get_vocabularies();
      foreach ($vocabularies as $vocabulary) {
        if (0 == strncmp($vocabulary->name, MAGENTO_STORES_VOCABULARY_CATEGORIES, drupal_strlen(MAGENTO_STORES_VOCABULARY_CATEGORIES))) {
          // Yes, this is our vocabulary :) - let's add it
          $vocabulary = get_object_vars($vocabulary);
          if (!isset($vocabulary['nodes'][$type_name])) {
            $vocabulary['nodes'][$type_name] = $type_name;
            taxonomy_save_vocabulary($vocabulary);
          }
        }
      }
      $message = t("Updating vocabularies '%name'", array('%name' => MAGENTO_STORES_VOCABULARY_CATEGORIES));
      break;

    case 12: // update 'Magento Product Type' vocabulary by adding this content type
      $type_name = MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID];
      if ($vocabulary = magento_get_vocabulary(MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES, NULL, NULL, TRUE)) {
        $vocabulary = get_object_vars($vocabulary);

        if (!isset($vocabulary['nodes'][$type_name])) {
          $vocabulary['nodes'][$type_name] = $type_name;
          taxonomy_save_vocabulary($vocabulary);
        }
      }
      $message = t("Updating vocabulary '%name'", array('%name' => MAGENTO_STORES_VOCABULARY_PRODUCT_TYPES));
      break;

    case 13: // Store node reference
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field__store__',       'Store node',       'nodereference', 'nodereference_autocomplete');
      magento_field_instance_update('field__store__', array('referenceable_types' => array(MAGENTO_STORES_CONTENT_TYPE_STORE => MAGENTO_STORES_CONTENT_TYPE_STORE)));
      break;

    case 14: // Store Group node reference
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field__store_group__', 'Store Group node', 'nodereference', 'nodereference_autocomplete');
      magento_field_instance_update('field__store_group__', array('referenceable_types' => array(MAGENTO_STORES_CONTENT_TYPE_GROUP => MAGENTO_STORES_CONTENT_TYPE_GROUP)));
      break;

    case 15: // Website node reference
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field__website__',     'Website node',     'nodereference', 'nodereference_autocomplete');
      magento_field_instance_update('field__website__', array('referenceable_types' => array(MAGENTO_STORES_CONTENT_TYPE_WEBSITE => MAGENTO_STORES_CONTENT_TYPE_WEBSITE)));
      break;

    case 16: // Add node-references for linked products
      $info = magento_stores_get_info(MAGENTO_STORES_LINKED_PRODUCT_TYPES);
      if (isset($info[MAGENTO_STORES_LINKED_PRODUCT_TYPES]['remote_data'])) {
        // + parent
        $info[MAGENTO_STORES_LINKED_PRODUCT_TYPES]['remote_data'][] = array(
          'code' => MAGENTO_STORES_PARENT_PRODUCTS,
          'name' => 'Parent node',
        );

        // + bundles
        if (magento_stores_is_bundle($attribute_set[MAGENTO_API_ATTRIBUTESET_ID])) {
          $info[MAGENTO_STORES_LINKED_PRODUCT_TYPES]['remote_data'][] = array(
            'code' => 'bundle',
            'name' => 'Bundle nodes',
          );
        }

        // + backlinks for configurables
        if (magento_stores_is_configurable($attribute_set[MAGENTO_API_ATTRIBUTESET_ID])) {
          $info[MAGENTO_STORES_LINKED_PRODUCT_TYPES]['remote_data'][] = array(
            'code' => 'configurable',
            'name' => 'Configurable backlink nodes',
          );
        }

        // one CCK field for each
        foreach ($info[MAGENTO_STORES_LINKED_PRODUCT_TYPES]['remote_data'] as $linked) {
          $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], "field__{$linked['code']}_links__", $linked['name'], 'nodereference', 'nodereference_autocomplete', FALSE, TRUE);

          // get already existing product type nodes
          $referenceable_types = array();
          $types = magento_stores_get_product_content_types();
          foreach ($types as $type) {
            $referenceable_types[$type] = $type;
          }

          magento_field_instance_update("field__{$linked['code']}_links__", array('referenceable_types' => $referenceable_types));
        }
      }
      break;

    case 17: // Add field to store Product Type specific data (like configurable data)
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field_type_specific_data',     'Product Type specific data',     'text', 'text_textarea');
      // Add field to store CUSTOM_OPTIONS data
      if (magento_stores_is_custom_options_allowed($attribute_set[MAGENTO_API_ATTRIBUTESET_ID])) {
        $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field_custom_options_data',  'Custom Options data',            'text', 'text_textarea');
      }
      break;
    case 18: // Add field to store Product Media Gallery data (product images with their path, labels, position, visibility, etc.)
      $message = magento_add_cck_field(MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID], 'field_image_gallery',     'Images',     'text', 'text_textarea');
      break;

    default: // Create all required CCK Fields
      $attributes = magento_stores_product_get_attributes();

      $attribute  = $attribute_set['attributes'][$current - 19]; // HARDCODED number of steps

      // rename 'code' into => MAGENTO_ATTRIBUTE_CODE
      if (!isset($attribute[MAGENTO_ATTRIBUTE_CODE])) {
        $attribute[MAGENTO_ATTRIBUTE_CODE] = $attribute['code'];
        unset($attribute['code']);
      }

      if (isset($attributes[$attribute[MAGENTO_ATTRIBUTE_CODE]])) {
        $attribute['frontend_input'] = $attributes[$attribute[MAGENTO_ATTRIBUTE_CODE]]['frontend_input'];
        $attribute['backend_type']   = $attributes[$attribute[MAGENTO_ATTRIBUTE_CODE]]['backend_type'];

        // this attribute is well known - let's use the first available label as a name
        foreach ($attributes[$attribute[MAGENTO_ATTRIBUTE_CODE]]['labels'] as $label) {
          $attribute_name = $label;
          break;
        }
      }
      else {
        // We don't know this attribute - our attribute's list is out of date !
      }

      if (empty($attribute_name)) {
        // let's use CODE as a default name
        $attribute_name = $attribute[MAGENTO_ATTRIBUTE_CODE];
      }

      $cck = magento_type2cck($attribute['frontend_input'], isset($attribute['backend_type'])? $attribute['backend_type']: NULL );
      $scope = ($attribute[MAGENTO_API_ATTRIBUTE_SCOPE] != 'store') ? '_' . $attribute[MAGENTO_API_ATTRIBUTE_SCOPE] : '';
      $type = MAGENTO_STORES_CONTENT_TYPE_PRODUCT . $attribute_set[MAGENTO_API_ATTRIBUTESET_ID] . $scope;
      $message = $cck['cck_create_function'](
        $type,
        'field_' . $attribute[MAGENTO_ATTRIBUTE_CODE],
        $attribute_name,
        $cck['cck_type'],
        $cck['cck_widjet_type'],
        $current == $total
      );

      // Check for this attribute in our tables
      $attribute_exists_in_attribute_set = db_result(db_query('SELECT true FROM {'. MAGENTO_STORES_TABLE_ATTRIBUTE_SETS .'} WHERE '. MAGENTO_STORES_TYPE ." = '%s' AND ". MAGENTO_ATTRIBUTE_CODE ." = '%s'", $type, $attribute[MAGENTO_ATTRIBUTE_CODE]));
      if (!$attribute_exists_in_attribute_set) {
        // Add this attribute into our table
        $row = array(MAGENTO_STORES_TYPE => $type, MAGENTO_ATTRIBUTE_CODE => $attribute[MAGENTO_ATTRIBUTE_CODE]);
        drupal_write_record(MAGENTO_STORES_TABLE_ATTRIBUTE_SETS, $row);
      }
  }
  return $message;
}
