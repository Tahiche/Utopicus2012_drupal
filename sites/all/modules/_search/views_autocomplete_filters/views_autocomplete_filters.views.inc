<?php

/**
 * @file views_autocomplete_filters.views.inc
 *
 * Adds Views3 support
 *
 * Views3 adds the concept of exposed forms to the mix.  In addition, elements injected into a Views
 * dialog is no longer saved along with the Views form information (see the unpack_options() and
 * options_definition() methods of the views_object object).
 *
 */

/**
 * Implementation of hook_views_plugins()
 */
function views_autocomplete_filters_views_plugins() {
  return array(
    'exposed_form' => array(
      'views_autocomplete_filters' => array(
        'title' => t('Views Autocomplete Filters'),
        'help' => t('Provides autocomplete for exposed filters'),
        'handler' => 'views_autocomplete_filters_exposed_form_plugin',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'parent' => 'basic',
      ),
    ),
  );
}
