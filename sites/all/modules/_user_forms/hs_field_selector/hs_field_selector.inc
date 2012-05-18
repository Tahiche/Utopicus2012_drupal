<?php
/**
 * @file
 * Hierarchical Select Field Selector form element code.
 */

/**
 * HS implementation to select CCK content types and their fields.
 */
function hs_field_selector_form_element() { 
  $form = array(
    '#type' => 'hierarchical_select',
    '#title' => t('Select field'),
    '#size' => 1,
    '#description' => t('Select field from the list of content types'), 
    '#config' => array(
      'module' => 'hs_field_selector',
      // Callback function names for filtering node types and fields.
      'params' => array('nodetype_filter' => '', 'field_filter' => ''),
      'save_lineage'    => 1,
      'enforce_deepest' => 1,
      'entity_count'    => 0,
      'resizable'       => 1,
      'level_labels' => array(
        'status' => 1,
        'labels' => array(
          0 => t('Content type'),
        ), 
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => '',
        'limit'    => 0,
        'reset_hs' => 1,
      ),
      'editability' => array(
        'status'           => 0,
        'item_types'       => array(),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 2,
      ),
      // These settings cannot be configured through the UI: they can only be
      // overridden through code.
      'animation_delay'    => 400,
      'exclusive_lineages' => array(),
      'render_flat_select' => 0,
    ),
  );
  return $form;
}

/**
 * Returns an array with the names of all parameters that are necessary for
 * this implementation to work.
 */
function hs_field_selector_hierarchical_select_params() {
  return array('nodetype_filter','field_filter');
}

/**
 * Returns the root level of the hierarchy: an array of (item, label) pairs.
 * The $dropbox parameter is optional and can even ommitted, as it's only
 * necessary if you need the dropbox to influence your hierarchy.
 */
function hs_field_selector_hierarchical_select_root_level($params, $dropbox = FALSE) {
  $type_map = array();
  $root_items = array();
  $types = content_types();
  foreach ($types as $id => $type) {
    // Type should have fields
    if (is_array($type['fields']) && !empty($type['fields'])) {
      // Filter node types using callback function.
      if (!empty($params['nodetype_filter'])) {
        if ($params['nodetype_filter']($type)) {
          $root_items[$id] = $type['name'];  
        }
      }
      else {
        $root_items[$id] = $type['name']; 
      }
    }
  }
  // Default HS option for none is "none". To avoid collisions, we implement
  // our own "none" option as empty string.
  $root_items[''] = t('none');
  return array_map('check_plain', $root_items);
}

/**
 * Gets the children of $parent ($parent is an item in the hierarchy) and
 * returns them: an array of (item, label) pairs, or the empty array if the
 * given $parent has no children.
 * The $dropbox parameter is optional and can even ommitted, as it's only
 * necessary if you need the dropbox to influence your hierarchy.
 */
function hs_field_selector_hierarchical_select_children($parent, $params, $dropbox = FALSE) {
  if ($parent === -1) {
    return array();
  }
  $types = content_types();
  if (!is_array($types[$parent]) || !is_array($types[$parent]['fields'])) {
    return array();
  }
  $children = array();
  foreach ($types[$parent]['fields'] as $id => $field) {
    if (!empty($params['field_filter'])) {
      // Filter fields using callback function.
      if ($params['field_filter']($field, $parent)) {
        $children[$id] = $field['widget']['label'] .' ('. $field['type'] .')';  
      }
    }
    else {
      $children[$id] = $field['widget']['label'] .' ('. $field['type'] .')';
    }
  }
  return array_map('check_plain', $children);
}

/**
 * Validates an item, returns TRUE if valid, FALSE if invalid.
 *
 * @param $item
 * @param $params
 * @return unknown_type
 */
function hs_field_selector_hierarchical_select_valid_item($item, $params) {
  // TODO: Item validation is probably unneeded: hs_field_selector 
  // implementation can just validate resulting nodetype-field combinations 
  // during form submission and not care about single items. 
  return TRUE;
}

/**
 * Return metadata about this implementation.
 * This information is used to generate the implementations overview at
 * admin/settings/hierarchical_select/implementations.
 *
 * @return unknown_type
 */
function hs_field_selector_hierarchical_select_implementation_info() {
  return array(
    'hierarchy type' => t('CCK content types and fields'),
    'entity type'    => t('Content Field'),
  );
}

