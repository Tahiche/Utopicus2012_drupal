<?php

// $Id: sites.context.inc,v 1.1.2.2 2010/04/08 18:23:09 brynbellomy Exp $

/**
 * CTools plugin API hook for Context
 */
function sites_context_plugins() {
  $plugins = array();
  $plugins['context_condition_site'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'sites') .'/plugins',
      'file' => 'context_condition_site.inc',
      'class' => 'context_condition_site',
      'parent' => 'context_condition',
    ),
  );

  return $plugins;
}

/**
 * Registry hook for conditions & reactions.
 */
function sites_context_registry() {
  return array(
    'conditions' => array(
      'site' => array(
        'title' => t('Site'),
        'plugin' => 'context_condition_site',
      ),
    ),
  );
}