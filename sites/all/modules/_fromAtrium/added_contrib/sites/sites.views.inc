<?php

// $Id: sites.views.inc,v 1.1.2.3 2010/03/31 20:05:36 brynbellomy Exp $

/**
 * Implementation of hook_views_api().
 */
function sites_views_api() {
  return array(
    'api' => 2,
    'path' => drupal_get_path('module', 'sites'),
  );
}


/**
 * Implementation of hook_views_handlers().
 */
function sites_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'sites') .'/plugins',
    ),
    'handlers' => array(
      'sites_handler_filter_site_current' => array(
        'parent' => 'views_handler_filter',
       ),
      'sites_handler_filter_site' => array(
       'parent' => 'views_handler_filter_many_to_one',
      ),
    ),
  );
}

function sites_views_data() {
  $data = array();
  $data['site_node']['table']['group']  = t('Sites');

  $data['site_node']['table']['join'] = array(
    'site_node' => array(
      'left_field' => 'sid',
      'field' => 'sid',
    ),
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // sid field
  $data['site_node']['current_site_sid'] = array(
    'title' => t('Current site'),
    'help' => t('Filter node published in current site.'),
    'filter' => array(
      'title' => t('Current site'),
      'handler' => 'sites_handler_filter_site_current',
      'numeric' => TRUE,
      'allow empty' => TRUE,
    ),
  );

  $data['site_node']['sid'] = array(
    'title' => t('Sites'),
    'help' => t('Filter node published on selected sites.'),
    'filter' => array(
      'handler' => 'sites_handler_filter_site',
    ),
  );

  return $data;
}

