<?php

/**
 * Implementation of hook_default_view_views().
 */
function views_examples_views_default_views() {
  $views = array();
  $files = drupal_system_listing('\.vex', 'modules', 'name', 0);
  foreach ($files as $file_name => $file) {
    $field_name = 'views_examples_'.$file_name;
    if (variable_get($field_name, FALSE) && $file->name && $file->filename) {
      require_once($file->filename);
      $views[$file->name] = $view;
    }
  }
  return $views;
}
