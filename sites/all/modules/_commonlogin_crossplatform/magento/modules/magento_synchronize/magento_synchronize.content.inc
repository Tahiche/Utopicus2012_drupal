<?php
// $Id$
/**
 * @file
 * pages
 */
/**
 * `Magento Synchronization Console` page
 */
function magento_synchronize_page() {
  $info =  magento_stores_get_info(array(
              MAGENTO_STORES_WEBSITES,
              MAGENTO_STORES_GROUPS,
              MAGENTO_STORES_STORES,
              MAGENTO_STORES_ATTRIBUTES,
              MAGENTO_STORES_CATEGORIES,
              MAGENTO_STORES_MAGENTO_PRODUCT_TYPES,
              MAGENTO_STORES_PRODUCT_TYPES,
              MAGENTO_STORES_PRODUCTS)
  );

        $rows = array();
  foreach ($info as $key => $item) {
    $rows[] = array(
      $item['name'],
      $item['local_count'],
      isset($item['remote_count']) ? $item['remote_count'] : t('unknown'),
      (MAGENTO_STORES_PRODUCTS == $key) ? magento_synchronize_get_products_for_autosynch_count() : '-'
    );
  }

  return drupal_get_form('magento_synchronize_content_form', $rows);
}

/**
 *
 */
function magento_synchronize_content_form(&$form_state, $rows) {
  $form = array();

  // Check the settings to enable some functionality
  $magento_stores = module_exists('magento_stores');

/**************************************
 * GENERAL status
 * *************************************/
  // general status form
  $form['status'] = array('#type' => 'fieldset', '#collapsible' => TRUE, '#collapsed' => FALSE,
    '#title' => t('Status'),
  );

  // Information
    $header = array(
    array('data' => t('Objects'),        'width' => '61%'),
    array('data' => t('Local count'),    'width' => '13%'),
    array('data' => t('Magento count'),  'width' => '13%'),
    array('data' => t('For cron synch'), 'width' => '13%'),
  );

  $output = theme('table', $header, $rows);
  $form['status']['data'] = array(
    '#value' => $output
  );

  $form['status']['cleanup'] = array('#type' => 'submit',
    '#value'    => t('Cleanup All'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-status-cleanup',
  );

  $form['status']['synchronize'] = array('#type' => 'submit',
    '#value'    => t('Cleanup & Full ReSync'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-status-synchronize',
  );

  // COMMENT: this functionality should be enabled only if we have Magento Stores Module
  if (!$magento_stores) {
    $form['magento_stores_info'] = array(
      '#value' => '<div class="messages warning">' . t("'Magento Stores' module is not installed") . '</div>',
    );
  }
/**************************************
 * STORES
 * *************************************/
  $form['stores'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Stores'),
    '#collapsed' => !$magento_stores,
  );

  $form['stores']['cleanup'] = array('#type' => 'submit',
    '#value'       => t('Cleanup Stores'),
    '#description' => t('Some description here'),
    '#disabled'    => !$magento_stores,
    '#id' => 'edit-stores-cleanup',
  );

  $form['stores']['synchronize'] = array('#type' => 'submit',
    '#value'       => t('Synchronize Stores'),
    '#description' => t('Some description here'),
    '#disabled'    => !$magento_stores,
    '#id' => 'edit-stores-synchronize',
  );

/**************************************
 * TAXONOMIES
 * *************************************/
  $form['taxonomy'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Taxonomies'),
    '#collapsed' => !$magento_stores,
  );

  $form['taxonomy']['cleanup'] = array('#type' => 'submit',
    '#value'    => t('Cleanup Taxonomies'),
    '#disabled' => FALSE,
    '#id' => 'edit-taxonomy-cleanup',
  );

  $form['taxonomy']['synchronize'] = array('#type' => 'submit',
    '#value'    => t('Synchronize Taxonomies'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-taxonomy-synchronize',
  );

/**************************************
 * PRODUCTS
 * *************************************/
  $form['products'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Products'),
    '#collapsed' => !$magento_stores,
  );

  // Product Types
  $form['products']['product_types'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Product Types'),
    '#collapsed' => !$magento_stores,
  );
  $form['products']['product_types']['cleanup'] = array('#type' => 'submit',
    '#value'    => t('Cleanup Product Types'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-product-types-cleanup',
  );

  $form['products']['product_types']['synchronize'] = array('#type' => 'submit',
    '#value'    => t('Synchronize Product Types'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-product-types-synchronize',
  );

  // Attributes
  $form['products']['attributes'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Attributes'),
    '#collapsed' => !$magento_stores,
  );
  $form['products']['attributes']['cleanup'] = array('#type' => 'submit',
    '#value'    => t('Cleanup Attributes'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-attributes-cleanup',
  );

  $form['products']['attributes']['synchronize'] = array('#type' => 'submit',
    '#value'    => t('Synchronize Attributes'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-attributes-synchronize',
  );

  // Products
  $form['products']['product'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Products'),
    '#collapsed' => !$magento_stores,
  );

  $form['products']['product']['cleanup'] = array('#type' => 'submit',
    '#value'    => t('Cleanup Products'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-products-cleanup',
  );

  $form['products']['product']['synchronize'] = array('#type' => 'submit',
    '#value'    => t('Synchronize Products'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-products-synchronize',
  );

  $form['products']['product']['reset_synchronize_time'] = array('#type' => 'submit',
    '#value'    => t('Reset Last Synchronization Time'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-products-reset-synchronize-time',
  );

  $form['products']['product']['remove_surplus_products'] = array('#type' => 'submit',
    '#value'    => t('Remove Surplus Products'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-remove-surplus-products',
  );

  //sync-missed-products
  $form['products']['product']['sync-missed-products'] = array('#type' => 'submit',
    '#value'    => t('Sync Missed Products'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-sync-missed-products',
  );
/**************************************
 * DEBUG
 * *************************************/
  $form['debug'] = array('#type' => 'fieldset', '#collapsible' => TRUE,
    '#title'     => t('Debug'),
    '#collapsed' => !$magento_stores,
  );

  $form['debug']['cleanup_broken_nodes'] = array('#type' => 'submit',
    '#value'    => t('Remove Nodes without Content Type'),
    '#disabled' => !$magento_stores,
    '#id' => 'edit-cleanup-broken-nodes',
  );

  return $form;
}

function magento_synchronize_content_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $batch = array();

  switch ($form_state['clicked_button']['#id']) {
    /* STOREs, GROUPs, WEBSITEs */
    case 'edit-stores-cleanup':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_STORES, MAGENTO_STORES_OPERATION_DELETE));
      // Batch info
      $info = array(
        'title'        => t('Cleanup Stores'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    case 'edit-stores-synchronize':
      // * Fetch stores
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_STORES, MAGENTO_STORES_OPERATION_FETCH));

      // Batch info
      $info = array(
        'title'        => t('Stores ReSynchronization'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    /* ATTRIBUTEs, PRODUCT TYPEs*/
    case 'edit-attributes-cleanup':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_ATTRIBUTES, MAGENTO_STORES_OPERATION_DELETE));

      // Batch info
      $info = array(
        'title'        => t('Cleanup Attributes'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    case 'edit-attributes-synchronize':
      // * Fetch attributes
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_ATTRIBUTES, MAGENTO_STORES_OPERATION_FETCH));

      // Batch info
      $info = array(
        'title'        => t('Attributes ReSynchronization'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    /* TAXONOMIES */
    case 'edit-taxonomy-cleanup':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_CATEGORIES,            MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_MAGENTO_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_DELETE));

      // Batch info
      $info = array(
        'title'        => t('Cleanup Taxonomies'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    case 'edit-taxonomy-synchronize':
      // * Create product types' vocabulary
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_MAGENTO_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_FETCH));

      // * Create vocabulary for categories
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_CATEGORIES, MAGENTO_STORES_OPERATION_FETCH));

      // Batch info
      $info = array(
        'title'        => t('Taxonomies ReSynchronization'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;
    case 'edit-product-types-cleanup':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_DELETE));

      // Batch info
      $info = array(
        'title'        => t('Cleanup Product Types'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;
    case 'edit-product-types-synchronize':
      // * Create product types
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_FETCH));

      // Batch info
      $info = array(
        'title'        => t('ReSynchronization of Product Types'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    case 'edit-status-cleanup':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS,              MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCT_TYPES,         MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_ATTRIBUTES,            MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_CATEGORIES,            MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_MAGENTO_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_STORES,                MAGENTO_STORES_OPERATION_DELETE));

      // Batch info
      $info = array(
        'title'        => t('Full Cleanup'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    case 'edit-status-synchronize':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS,              MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCT_TYPES,         MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_ATTRIBUTES,            MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_CATEGORIES,            MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_MAGENTO_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_DELETE));
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_STORES,                MAGENTO_STORES_OPERATION_DELETE));

      // * Fetch stores
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_STORES, MAGENTO_STORES_OPERATION_FETCH));

      // * Fetch attributes
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_ATTRIBUTES, MAGENTO_STORES_OPERATION_FETCH));

      // * Create product types' vocabulary
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_MAGENTO_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_FETCH));

      // * Create vocabulary for categories
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_CATEGORIES, MAGENTO_STORES_OPERATION_FETCH));

      // * Create products
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCT_TYPES, MAGENTO_STORES_OPERATION_FETCH));

      // * Get products
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS, MAGENTO_STORES_OPERATION_FETCH));

      // Batch info
      $info = array(
        'title'        => t('Full ReSynchronization'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;
    case 'edit-products-cleanup':
      magento_synchronize_reset_last_synch_time();
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS, MAGENTO_STORES_OPERATION_DELETE));

      // Batch info
      $info = array(
        'title'        => t('Cleanup products'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;
    case 'edit-products-synchronize':
      // * Create products
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS, MAGENTO_STORES_OPERATION_FETCH));

      // Batch info
      $info = array(
        'title'        => t('ReSynchronization of products'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    case 'edit-products-reset-synchronize-time':
      magento_synchronize_reset_last_synch_time();
      break;

    /* Remove products existing in Drupal, but absent in Magento */
    case 'edit-remove-surplus-products':
      $batch  = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS, MAGENTO_STORES_OPERATION_REMOVE_SURPLUS));

      // Batch info
      $info = array(
        'title'        => t('Check & Remove surplus products'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    /* Sync missed products existing in Magento, but absent in Drupal */
    case 'edit-sync-missed-products':
      $batch  = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_PRODUCTS, MAGENTO_STORES_OPERATION_SYNC_MISSED));

      // Batch info
      $info = array(
        'title'        => t('Check & Sync missed products'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

    // DEBUG
    case 'edit-cleanup-broken-nodes':
      $batch = array_merge($batch, magento_stores_create_batch(MAGENTO_STORES_BROKEN_NODES, MAGENTO_STORES_OPERATION_DELETE));

      // Batch info
      $info = array(
        'title'        => t('Remove Nodes without Content Type'),
        'init_message' => t('Start processing')
      );

      // Submit batch
      magento_batch_process($info, $batch);
      break;

  }
}
