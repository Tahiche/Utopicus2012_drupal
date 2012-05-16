<?php
// $Id$
/**
 * @file
 * magento_stores_synchronize_form
 */
/**
 * Theme the terms overview as a sortable list of terms.
 *
 * @ingroup themeable
 * @see taxonomy_overview_terms()
 */
function theme_magento_stores_content($form) {
  drupal_add_css(drupal_get_path('module', 'magento_stores') .'/magento_stores.css');

  $output .= drupal_render($form);

  return $output;
}

/**
 * Show remote and local Stores
 */
function magento_stores_content(&$form_state) {
  return magento_stores_synchronize_form($form_state);
}

/**
 * Synchronize form
 */
function magento_stores_synchronize_form(&$form_state) {
  $form = array();

  $header = array(
    'object' => array('field' => 'object',  'data' => t('Website/Store/View')),
    'node'   => array('field' => 'node',    'data' => t('Node')),
    'status' => array('field' => 'status',  'data' => t('Status')),
  );

  return $form;
}

function magento_stores_synchronize_form_submit($form, &$form_state) {
  // restore recently received tree
  $tree = $_SESSION[MAGENTO_STORES_WEBSITES_TREE];
  $items = &$form_state['values']['nodes'];

  foreach ($items as $node => $value) {
    if ($value == 0) {
      unset($items[$node]);
    }
  }

  // Cleanup the tree for synchronization
  // NOTE: isset() is considered to be faster then in_array()
  foreach ($tree as $website_name => &$website) {
    $store_found = FALSE;
    if (isset($items[$website['id']])) {
      unset($items[$website['id']]);
      $website['sync_type'] = MAGENTO_STORES_CONTENT_TYPE_WEBSITE;
    }
    foreach ($website['stores'] as $store_name => &$store) {
      $store_view_found = FALSE;
      if (isset($items[$store['id']])) {
        unset($items[$store['id']]);
        $store['sync_type'] = MAGENTO_STORES_CONTENT_TYPE_GROUP;
      }
      foreach ($store[MAGENTO_STORES_STORES] as $store_view_name => &$store_view) {
        if (isset($items[$store_view['id']])) {
          unset($items[$store_view['id']]);
          $store_view['sync_type'] = MAGENTO_STORES_CONTENT_TYPE_STORE;
          $store_view_found = TRUE;
        }
        else {
          // we don't care about this store view any more
          unset($store[MAGENTO_STORES_STORES][$store_view_name]);
        }
      }
      // If we have at least one store view - we have to sync the store first
      if ($store_view_found) {
        $store['sync_type'] = MAGENTO_STORES_CONTENT_TYPE_GROUP;
        $store_found = TRUE;
      }
      elseif (!isset($store['sync_type'])) {
        // we don't care about this store any more
        unset($website['stores'][$store_name]);
      }
    }
    // If we have at least one store - we have to sync the website first
    if ($store_found) {
      $website['sync_type'] = MAGENTO_STORES_CONTENT_TYPE_WEBSITE;
    }
    elseif (!isset($website['sync_type'])) {
      // we don't care about this website any more
      unset($tree[$website_name]);
    }
  }

  // Now we can resync the objects
  if (magento_stores_synchronize($tree)) {
    drupal_set_message(t('All objects were successfully resynchronized'), 'ok');
  }
  else {
    drupal_set_message(t('There were several errors during synchronization.'), 'error');
  }
}

/**
 * @deprecated
 */
/*
function magento_stores_get_row($type, $magento_id, &$object) {
  static $i = 0;

  $nodes = magento_check_node($magento_id);
  $nid = $nodes[0]['nid'];

  $object['id'] = ++$i;
  return array(
    '#weight' => $i,
    'object'  => "<div class='$type'>" . check_plain($object['name']) . '</div>',
    'node'    => ($nid) ? l(node_load($nid)->title, "node/$nid") : '-',
    'status'  => 'OK'
  );
}
 */