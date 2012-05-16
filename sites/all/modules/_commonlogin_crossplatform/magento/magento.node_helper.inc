<?php
// $Id$

/**
 * @file
 * Magento Node helper functions
 */

/**
 * Implement hook helper, this function should be helpful for all magento modules offering hooks
 */
function magento_invoke($hook, $event, &$arg1 = NULL, &$arg2 = NULL, &$arg3 = NULL) {
  $break = FALSE;

  foreach (module_implements($hook) as $module) {
    //@HACK: call_user_func with passed by reference
    $args = array($event, $break, &$arg1, &$arg2, &$arg3);
    $unused_count = 5 - func_num_args();
    if ($unused_count > 0) {
      while ($unused_count > 0) {
        array_pop($args);
        $unused_count--;
      }
    }
    elseif ($unused_count < 0 ) {
      $all_args = func_get_args();
      $args = array_merge($args, array_slice($all_args, 5));
    }
    $function = $module .'_'. $hook;
    $break |= call_user_func_array($function, $args);
  }

  return $break;
}

/**
 * Non recursive version of Quick Sort
 */
function magento_helper_quickSort(&$array) {
  // rearrange array for sorting
  $new_array = array();
  foreach ($array as $item) {
    $new_array[] = $item;
  }
  $array = $new_array;

  $cur = 1;
  $stack[1]['l'] = 0;
  $stack[1]['r'] = count($array) - 1;

  do {
    $l = $stack[$cur]['l'];
    $r = $stack[$cur]['r'];
    $cur--;
      do {
        $i = $l;
        $j = $r;
        $tmp = $array[(int)( ($l+$r)/2 )]['description'];

        // partion the array in two parts.
        // left from $tmp are with smaller values,
        // right from $tmp are with bigger ones
        do {
          while ( 0 > strcmp($array[$i]['description'], $tmp) )
          $i++;

          while ( 0 < strcmp($array[$j]['description'], $tmp) )
          $j--;

          // swap elements from the two sides
          if ( $i <= $j ) {
            $w = $array[$i];
            $array[$i] = $array[$j];
            $array[$j] = $w;
            $i++;
            $j--;
          }
        } while ( $i <= $j );

        if ( $i < $r ) {
          $cur++;
          $stack[$cur]['l'] = $i;
          $stack[$cur]['r'] = $r;
        }
        $r = $j;
      } while ( $l < $r );
  } while ( $cur != 0 );
}

/**
 * Create new CCK content type or modify existing
 * @param $content_type
 *  string ContentType type
 * @param $name
 *  string ContentType name
 * @param $description
 *  string ContentType description
 *
 * @return string msg
 */
function magento_add_content_type($content_type, $name, $description = '') {
  // hook before
  magento_invoke('magento', 'before add_content_type', $content_type, $name, $description);

  $type              = new stdClass();
  $type->type        = $content_type;
  $type->orig_type   = $type->type;
  $type->name        = $name;
  $type->description = $description;
  $type->module      = 'magento';
  $type->title_label = 'Name';
  $type->help        = '';
  $type->has_title   = TRUE;
  $type->has_body    = FALSE;
  $type->custom      = FALSE;
  $type->modified    = TRUE;
  $type->locked      = TRUE;
  $type->min_word_count = 0;

  $status = node_type_save($type);
  node_types_rebuild();

  $t_args = array('%name' => $type->name);
  if ($status == SAVED_UPDATED) {
    $msg = t('The content type %name has been updated.', $t_args);
  }
  elseif ($status == SAVED_NEW) {
    $msg = t('The content type %name has been added.', $t_args);
  }

  // hook after
  magento_invoke('magento', 'after add_content_type', $msg);
  return $msg;
}

/**
 * Remove content type
 */
function magento_remove_content_type($content_type) {
  // hook before
  magento_invoke('magento', 'before remove_content_type', $content_type);

  $result = NULL;
  /* Check content type for existence and get it's name */
  $node_types = node_get_types('types', NULL, TRUE);

  foreach ($node_types as $type => $info) {
    if ($content_type == $type) {
      node_type_delete($content_type);
      node_types_rebuild();

      $t_args = array('%name' => $info->name);
      drupal_set_message(t('The content type %name has been deleted.', $t_args));
      break;
    }
  }

  // hook after
  magento_invoke('magento', 'after remove_content_type', $result);
  return $result;
}

/**
 * Helper function used to Update CCK field instanse data
 *
 * @param $field
 *  string CCK field name
 * @param $data
 *  array CCK field data
 * @param $merge
 *  boolean If TRUE merge current CCK field data with new $data, otherwise replace current CCK field data with new $data.
 *
 * @return
 */
function magento_field_instance_update($field, $data, $merge = TRUE) {
  $fields = content_fields();
  if (isset($fields[$field])) {
    if ($merge) {
      magento_merge_arrays($fields[$field], $data);
    }
    else {
      foreach ($data as $key => $value) {
        $fields[$field][$key] = $value;
      }
    }
    include_once(drupal_get_path('module', 'content') .'/includes/content.crud.inc');
    return content_field_instance_update($fields[$field], TRUE);
  }
  return NULL;
}

/**
 * Helper function used to Add new CCK field
 *
 * @param $cointent_type
 *  string ContentType type
 * @param $field_name
 *  string Field name
 * @param $label
 *  string Field label
 * @param $type
 *  string Field type
 * @param $widget_type
 *  string Field CCK widget type
 * @param $rebuild
 *  $rebuild TRUE to clear content type caches and rebuild menu (default).
 *  FALSE allows the caller to process several fields at a time quickly,
 *  but then the caller is reponsible to clear content type caches and rebuild menu as soon as all fields have been processed.
 * @param $multiple
 *  TRUE if Field has multiple values. FALSE otherwise.
 *
 */
function magento_add_cck_field($content_type, $field_name, $label, $type, $widget_type, $rebuild = FALSE, $multiple = FALSE) {
  // hook before
  magento_invoke('magento', 'before add_cck_field', $content_type, $field_name, $label, $type, $widget_type, $rebuild);

  include_once(drupal_get_path('module', 'content') .'/includes/content.crud.inc');

  $field = content_fields($field_name, $content_type);
  if (empty($field)) {
    $field = array();
    $field['label']       = $label;
    if ('optionwidgets_onoff' == $widget_type) {
      $field['description']       = $label;
    }
    $field['field_name']  = $field_name;
    $field['type_name']   = $content_type;
    $field['widget_type'] = $widget_type;
    $field['type']        = $type;
    $field['parent']      = '';
    $field['multiple']    = $multiple;
    $field['hidden_name'] = '_add_new_field';

    content_field_instance_create($field, $rebuild);

    $msg = t('CCK field "%name" has been added to "%type".', array('%name' => $label, '%type' => $content_type));
  }
  else {
    $msg = t('Content type "%type" already contains CCK field "%name".', array('%name' => $label, '%type' => $content_type));
  }

  // hook after
  magento_invoke('magento', 'after get_products_list', $msg);
  return $msg;
}

/**
 * Remove CCK field
 */
function magento_remove_cck_field($content_type, $field_name) {
  // hook before
  magento_invoke('magento', 'before remove_cck_field', $content_type, $field_name);

  include_once(drupal_get_path('module', 'content') .'/includes/content.crud.inc');
  $result = content_field_instance_delete($field_name, $content_type);

  // hook after
  magento_invoke('magento', 'after remove_cck_field', $result);
  return $result;
}

/**
 * Clear Cache wrapper
 */
function magento_clear_type_cache($param) {
  content_clear_type_cache($param);
  menu_rebuild();
}

/**
 * Save the node under the correct user
 */
function magento_node_save($node) {
  static $user_id = 0;

  if (!$user_id) {
    $user = user_load(array('name' => MAGENTO_USER));
    $user_id = $user->uid;
  }

  $node->uid = $user_id;
  node_save($node);
}

/**
 * Delete node wrapper
 * - Same as node_delete() but without access_permission check.
 */
function magento_node_delete($nid) {
  //node_delete($node);

  // Clear the cache before the load, so if multiple nodes are deleted, the
  // memory will not fill up with nodes (possibly) already removed.
  $node = node_load($nid, NULL, TRUE);


  db_query('DELETE FROM {node} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {node_revisions} WHERE nid = %d', $node->nid);

  // Call the node-specific callback (if any):
  node_invoke($node, 'delete');
  node_invoke_nodeapi($node, 'delete');

  // Clear the page and block caches.
  cache_clear_all();

  // Remove this node from the search index if needed.
  if (function_exists('search_wipe')) {
    search_wipe($node->nid, 'node');
  }
  watchdog('content', '@type: deleted %title.', array('@type' => $node->type, '%title' => $node->title));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => node_get_types('name', $node), '%title' => $node->title)));
}

/**
 * Try to find the product Node with desired ID in Magento.
 * Helper function used to found the node(s) with specified $magento_id and $level_id and (optionally) $type
 *
 * @return array
 */
function magento_check_node($magento_id, $level_id = 0, $type = NULL) {
  // do we know level Id ?
  if (0 < $level_id) {
    $query = <<<QUERY
SELECT
 n.nid, n.type, s.field_level_id_value
FROM
 {content_field_magento_id} f LEFT JOIN
 {content_field_level_id} s ON s.nid = f.nid LEFT JOIN
 {node} n ON n.nid = f.nid
WHERE f.field_magento_id_value = %d AND s.field_level_id_value = %d
QUERY;
    $params = array($magento_id, $level_id);
  }
  elseif (db_table_exists('content_field_level_id')) {
    $query = <<<QUERY
SELECT
 n.nid, n.type, s.field_level_id_value
FROM
 {content_field_magento_id} f LEFT JOIN
 {content_field_level_id} s ON s.nid = f.nid LEFT JOIN
 {node} n ON n.nid = f.nid
WHERE f.field_magento_id_value = %d
QUERY;
    $params = array($magento_id);
  }
  else {
    $query = <<<QUERY
SELECT
 n.nid, n.type
FROM
 {content_field_magento_id} f LEFT JOIN
 {node} n ON n.nid = f.nid
WHERE f.field_magento_id_value = %d
QUERY;
    $params = array($magento_id);
  }

  if (!empty($type)) {
    $query .= " AND n.type = '%s'";
    $params[] = $type;
  }

  $result = array();
  $res = db_query($query, $params);
  while ($node = db_fetch_array($res)) {
    $result[] = $node;
  }

  return $result;
}
/**
 * @depreacted
 * Try to find the product Node with desired ID in Magento
 *
 */
function __depreacated_magento_check_node($magento_id, $level_id = 0) {
  $result = NULL;
  $cid    = "magento_cache:{$magento_id}";
  $cached = array();

  // We do not often change this info, so caching SELECTs might greatly improve site's performance
  $use_cache = variable_get('magento_cache_settings', array());
  if (isset($use_cache['selects']) && $use_cache['selects']) {
    // Get data from the cache
    $cached_obj = cache_get($cid);
    if (isset($cached_obj->data)) {
      $cached = $cached_obj->data;
    }

    if (!empty($cached["check_node_{$level_id}"])) {
      $result = $cached["check_node_{$level_id}"];
    }
  }

  if (!$result) {
    // do we know level Id ?
    if (0 < $level_id) {
      $query = <<<QUERY
SELECT
   n.nid, n.type, s.field_level_id_value
FROM
   {content_field_magento_id} f LEFT JOIN
   {content_field_level_id} s ON s.nid = f.nid LEFT JOIN
   {node} n ON n.nid = f.nid
WHERE f.field_magento_id_value = %d AND s.field_level_id_value = %d
QUERY;
    }
    elseif (db_table_exists('content_field_level_id')) {
      $query = <<<QUERY
SELECT
   n.nid, n.type, s.field_level_id_value
FROM
   {content_field_magento_id} f LEFT JOIN
   {content_field_level_id} s ON s.nid = f.nid LEFT JOIN
   {node} n ON n.nid = f.nid
WHERE f.field_magento_id_value = %d
QUERY;
    }
    else {
      $query = <<<QUERY
SELECT
   n.nid, n.type
FROM
   {content_field_magento_id} f LEFT JOIN
   {node} n ON n.nid = f.nid
WHERE f.field_magento_id_value = %d
QUERY;
    }

    $result = array();
    $res = db_query($query, $magento_id, $level_id);
    while ($node = db_fetch_array($res)) {
      $result[] = $node;
    }

    if (isset($use_cache['selects'])) {
      $cached["check_node_{$level_id}"] = $result;
      cache_set($cid, $cached);
    }
  }

  return $result;
}

/**
 * Find existing node or create a new one (the product Node with specified ID in Magento).
 * @param $type
 *  string product type
 * @param $magento_id
 *  string Magento ID
 * @param $title
 *  string Node title
 * @param $level_id
 *
 * @return Node object
 */
function magento_get_node($type, $magento_id, $title = '', $level_id = 0) {
  static $cached_nodes = array();

  if (isset($cached_nodes[$level_id][$magento_id][$type])) {
    return $cached_nodes[$level_id][$magento_id][$type];
  }

  $nid   = NULL;
  $node  = new StdClass;

  //$nodes = magento_check_node($magento_id, $level_id);
  $nodes = magento_check_node($magento_id, $level_id, $type);

  foreach ($nodes as $item) {
    if ($item['type'] == $type) {
      $nid = $item['nid'];
      break;
    }
  }

  if (NULL == $nid) {
    // create new node
    $node->title = $title;

  // initially unpublished
    $node->status = 0;

    // hidden DB sync fields
    $node->field_magento_id = array(array('value' => $magento_id));
    if ($level_id) {
      $node->field_level_id = array(array('value' => $level_id));
    }

    // created by Editor
    $node->type     = $type;
    $node->revision = 0;
    $node->uid      = 1; // TODO: admin ?

    // New node will be automatically loaded into $node
    magento_node_save($node);
  }
  else {
    $node = node_load($nid);
  }

  $cached_nodes[$level_id][$magento_id][$type] = $node;

  return $node;
}

/**
 * Assign(link) Content Types to Taxonomy vocabulary
 * @param $vocabulary
 *  Taxonomy vocabulary data array
 * @param $content_types
 *  array of CCK Content Types that should be linked to the $vocabulary
 *
 */
function magento_alter_vocabulary_assign_content_types($vocabulary, $content_types) {
  if ($vocabulary && is_array($content_types)) {
    $save       = FALSE;
    $vocabulary = get_object_vars($vocabulary);
    foreach ($content_types as $type_name) {
      if (!isset($vocabulary['nodes'][$type_name])) {
        $vocabulary['nodes'][$type_name] = $type_name;
        $save = TRUE;
      }
    }

    if ($save) {
      taxonomy_save_vocabulary($vocabulary);
    }
  }
}

/**
 * Load existing vocabulary or create a new one
 */
function magento_get_vocabulary($name, $vid = NULL, $multiple = NULL, $reset = FALSE) {
  // get (or create vocabulary)
  if (NULL == $vid) {
    // Let's check for this vocabulary
    $vid = db_result(db_query("SELECT vid FROM {vocabulary} WHERE vid = %d", $vid));
  }
  if (NULL == $vid || 0 >= $vid) {
    // If there is no vocabulary with this ID - let's try to load the one with this name
    $vid = db_result(db_query("SELECT vid FROM {vocabulary} WHERE name = '%s'", $name));
  }
  if (NULL == $vid || 0 >= $vid) {
    // Let's create a new vocabulary
    taxonomy_save_vocabulary(
      $vocabulary = array(
        'name'      => $name,
        'relations' => 1,
        'multiple'  => $multiple
      )
    );
    $vid = db_result(db_query("SELECT vid FROM {vocabulary} WHERE name = '%s'", $name));
  }

  $vocabulary = taxonomy_vocabulary_load($vid, $reset);
  if ($vocabulary && (!isset($vocabulary->multiple) || ($multiple !== NULL && $vocabulary->multiple != $multiple))) {
    $vocabulary->multiple = $multiple;
    $vocabulary_array = (array) $vocabulary;
    taxonomy_save_vocabulary($vocabulary_array);
    cache_clear_all();
  }
  return $vocabulary;
}

/**
 * Update vocabulary with specified name
 * This function is used to create/update terms in Taxonomy vocabulary
 * @param $name
 *  string Taxonomy vocabulary name
 * @param $remote
 *  array with terms data
 * @param $multiple
 * @param $process_terms
 * @param $flush_caches
 *
 */
function magento_update_vocabulary($name, &$remote, $multiple = NULL, $process_terms = -1, $flush_caches = FALSE) {
  static $vocabularies = array();

  if ($flush_caches) {
    $vocabularies = array();
    $flush_caches = FALSE;
  }

  $processed  = 0;
  $vocabulary = magento_get_vocabulary($name, NULL, $multiple);

  // we do not use taxonomy functions to update vocabulary because they clean cache each time the node gets modified
  if ($vocabulary) {
    if (!isset($vocabularies[$vocabulary->vid])) {
      $local = array();
      /* get drupal terms without hierarchy */
      $result = db_query("SELECT t.*, h.parent FROM {term_data} t LEFT JOIN {term_hierarchy} h ON t.tid = h.tid WHERE t.vid = %d ORDER BY t.description ASC", $vocabulary->vid);
      while ($term = db_fetch_array($result)) {
        $local[] = $term;
      }
      $vocabularies[$vocabulary->vid] = $local;
    }

    $local = &$vocabularies[$vocabulary->vid];

    // Sort $remote by 'description'
    magento_helper_quickSort($remote);

    if (0 == count($remote) && 0 < count($local)) {
      // Just remove all the items
      foreach ($terms as $term => $data) {
        if (0 != $process_terms) {
          $processed++;
          taxonomy_del_term($data['tid']);
          $flush_caches = TRUE;
          unset($local[$term]);
          if (0 < $process_terms) {
              --$process_terms;
          }
        }
        else break; // stop processing
      }
    }
    else {
      // Check for new items
      $current_remote = 0;
      $current_local  = 0;

      while (isset($remote[$current_remote]) || isset($local[$current_local])) {
        if (isset($local[$current_local])) {
          $local_term = $local[$current_local];
          if (empty($local_term['description']) || !isset($remote[$current_remote]) || (0 > strcmp($local_term['description'], $remote[$current_remote]['description']))) {
            if (0 != $process_terms) {
              // remove unused term
              taxonomy_del_term($local_term['tid']);
              $flush_caches = TRUE;
              $current_local++;
              $processed++;

              if (0 < $process_terms) {
                --$process_terms;
              }
            }
            else break; // stop processing
          }
          elseif ($local_term['description'] == $remote[$current_remote]['description']) {
            // item exists

            $local_term_updated = FALSE;

            // Check for possibly updated name
            if (0 != strcmp($local_term['name'], $remote[$current_remote]['name'])) {
              $local_term['name'] = $remote[$current_remote]['name'];
              $local_term_updated = TRUE;
            }
            // Check for possibly updated weight
            if (isset($remote[$current_remote]['weight']) && ($local_term['weight'] != $remote[$current_remote]['weight']) ) {
              $local_term['weight'] = intval($remote[$current_remote]['weight']);
              $local_term_updated = TRUE;
            }
            if ($local_term_updated) {
              taxonomy_save_term($local_term);
            }

            $current_remote++;
            $current_local++;
          }
          else {
            if (0 != $process_terms) {
              // add new item
              $term = $remote[$current_remote];
              $term['vid'] = $vocabulary->vid;
              unset($term['parent']);
              taxonomy_save_term($term);
              $flush_caches = TRUE;

              $current_remote++;
              $processed++;

              if (0 < $process_terms) {
                --$process_terms;
              }
            }
            else break; // stop processing
          }
        }
        else {
            if (0 != $process_terms) {
              // we should add new item
              $term = $remote[$current_remote];
              $term['vid'] = $vocabulary->vid;
              unset($term['parent']);
              taxonomy_save_term($term);
              $flush_caches = TRUE;

              $current_remote++;
              $processed++;

              if (0 < $process_terms) {
                --$process_terms;
              }
            }
            else break; // stop processing
        }
      }
    }

    if (0 != $process_terms) {
      /* reread drupal terms without hierarchy */
      $local_new = array();
      $result = db_query("SELECT t.*, h.parent FROM {term_data} t LEFT JOIN {term_hierarchy} h ON t.tid = h.tid WHERE t.vid = %d ORDER BY t.description ASC", $vocabulary->vid);
      while ($term = db_fetch_array($result)) {
        $local_new[$term['description']] = $term;
      }

      // Now $remote and $local_new data should contain same amount of items, - let's update data and parents
      foreach ($remote as $remote_item) {
        $local_term = (isset($local_new[$remote_item['description']])) ? $local_new[$remote_item['description']] : array();

    // Do we need to update this term (different parents) ?
    if (0 != (int)$remote_item['parent'] || 0 != (int)$local_term['parent']) {
          $parent = ($remote_item['parent'] && isset($local_new[$remote_item['parent']]['tid'])) ? $local_new[$remote_item['parent']]['tid'] : 0;

          if ($parent != $local_term['parent']) { // Yes parents are different
            if (0 != $process_terms) {
              $term = array_merge($local_term, $remote_item);
              if (!empty($term['parent'])) {
                $term['parent'] = (isset($local_new[$term['parent']])) ? $local_new[$term['parent']]['tid'] : NULL;
              }

              taxonomy_save_term($term);
              $processed++;

              if (0 < $process_terms) {
                --$process_terms;
              }
            }
            else break; // stop processing
          }
    }
      }
    }
  }

  // $flush_caches == TRUE here means - remove only this vocabulary
  if ($flush_caches) {
    unset($vocabularies[$vocabulary->vid]);
  }

  return $processed;
}

/**
 * Update taxonomies for the selected node
 */
function magento_update_node_taxonomies($node, $items) {
  $resync_vocabularies = TRUE;
  $taxonomies = array();

  static $vocabularies = array();

  foreach ($items as $item) {
    if (!isset($vocabularies[$item['name']])) {
      $vocabulary = magento_get_vocabulary($item['name']);
      $vocabularies[$item['name']] = $vocabulary->vid;
    }

    $tid = db_result(db_query("SELECT tid FROM {term_data} WHERE vid = %d AND description = '%s'", $vocabularies[$item['name']], $item['id']));
    if ($tid) {
      $taxonomies[] = $tid;
    }
  }

  taxonomy_node_save($node, $taxonomies);
}

/**
 * Helper function. Find item with specified $key and $value in assosiative $array.
 * (Used to update taxonomy terms data.)
 */
function &magento_tree_find_item($array, $key, $child_key, $value) {
  static $nothing = NULL;

  foreach ($array as $item) {
    if (isset($item[$key]) && $item[$key] == $value) {
      return $item;
    }
    elseif (isset($item[$child_key]) && is_array($item[$child_key])) {
      $result = &magento_tree_find_item($item[$child_key], $key, $child_key, $value);
      if ($result) {
        return $result;
      }
    }
  }

  return $nothing;
}

/**
 * Merge two arrays using KEY values
 */
function magento_tree_merge($base_array, $merge_key, $child_key, $parent_key, $merge_array) {
  foreach ($merge_array as $item) {
    $found = &magento_tree_find_item($base_array, $merge_key, $child_key, $item[$merge_key]);
    if (!$found) {
      if (isset($item[$parent_key]) && $parent = &magento_tree_find_item($base_array, $merge_key, $child_key, $item[$parent_key])) {
        $parent[$child_key][] = $item;
      }
      else {
        $base_array[] = $item;
      }
    }
    elseif (isset($item[$child_key]) && is_array($item[$child_key])) {
      $base_array = magento_tree_merge($base_array, $merge_key, $child_key, $parent_key, $item[$child_key]);
    }
  }

  return $base_array;
}

/**
 * Mark these types as module-related and active
 */
function magento_node_info() {
  $types = array();

  $result = db_query("SELECT type, name, module, description, title_label, has_body FROM {node_type} WHERE type LIKE 'magento_%%'");
  while ($item = db_fetch_array($result)) {
    $type = $item['type'];
    unset($item['type']);
    $types[$type] = $item;
  }

  return $types;
}

/**
 * Implementation of hook_form().
 */
function magento_form($node, $form_state) {
  $type = node_get_types('type', $node);
  $form = array();

  if ($type->has_title) {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type->title_label),
      '#required' => TRUE,
      '#default_value' => $node->title,
      '#maxlength' => 255,
      '#weight' => -5,
    );
  }

  if ($type->has_body) {
    $form['body_field'] = node_body_field($node, $type->body_label, $type->min_word_count);
  }

  return $form;
}

/**
 * node_access hook
 */
function magento_access($op, $node, $account) {
  if (isset($account->name) && $account->name == MAGENTO_USER) {
    return TRUE;
  }
  return NULL;
}

/**
 * Get Node Nid by value for fieldname
 *
 * @param $value
 *   Value to search
 * @param $fieldname
 *   CCK fieldname
 * @param $typename
 *   Content Type name or array with content types
 *   If $typename is array, then all content type values types must be same, and
 *   value type (decimal or string) will be solved for first content type
 *
 * @return
 *   Result if found of NULL
 */
function magento_get_nid_by_field_value($value, $fieldname, $typename) {
  $result = NULL;
  $type = NULL;
  if ($fieldname && $typename && $value) {
    $single_typename = is_array($typename) ? current($typename) : $typename;
    $info = content_database_info(content_fields($fieldname, $single_typename));

    if ($info['columns']['value']['type'] == 'char' || $info['columns']['value']['type'] == 'text') $type = "'%s'";
    if ($info['columns']['value']['type'] == 'int') $type = '%d';

    if (is_array($typename)) {
      $type_eq = " IN ('" . implode("','", $typename) . "')";
    }
    else {
      $type_eq = " = '" . $typename . "'";
    }

    if ($info['table'] && $type) {
      $sql = "
                      SELECT cck.nid FROM {" . $info['table'] . "} cck
                      LEFT JOIN {node} n ON cck.vid = n.vid
                      WHERE cck." . $info['columns']['value']['column'] . " = " . $type . " AND n.type " . $type_eq;

      $query = db_query($sql, $value);
      while ($data = db_fetch_array($query)) {
        $result[] = $data;
      }
    }
  }
  return $result;
}
