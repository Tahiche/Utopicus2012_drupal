<?php

/**
 * @file
 * Provides infrequently used functions for views examples module.
 */

drupal_add_js(drupal_get_path('module', 'views_examples').'/views_examples.js', 'module');

/**
 * Admin settings form.
 */
function views_examples_settings_form(&$form_state) {
  // go through directories and figure out where the info files are so they can
  // be added to the list
  $files = drupal_system_listing('\.vex', 'modules', 'name', 0);

  // Set defaults for info
  $defaults = array(
    'dependencies' => array(),
    'missing' => array(),
    'description' => '',
    'extra' => array(),
  );

  foreach ($files as $file_name => $file) {
    $file->info = drupal_parse_info_file(dirname($file->filename) .'/'. $file->name .'.info');

    // skip vex files that don't provide info - these may be added via extra
    if (empty($file->info) && empty($files[$file_name]->info)) {
      continue;
    }

    // merge in defaults and save
    $files[$file_name]->info = $file->info + $defaults;

    // go through extra examples and add in - has to be same directory
    if (! empty($files[$file_name]->info['extra'])) {
      foreach ($files[$file_name]->info['extra'] as $extra) {
        $extra = trim($extra);
        if ($extra) {
          $matches = array();
          if (preg_match('/(.*)\/.*/', $files[$file_name]->filename, $matches)) {
            $vex_dir = $matches[1];
            $files[$extra] = new stdClass();
            $files[$extra]->filename = $vex_dir.'/'.$extra.'.vex';
            $files[$extra]->basename = $extra.'.vex';
            $files[$extra]->name = $extra;
            $files[$extra]->info = $files[$file_name]->info;
          }
        }
      }
    }
  }

  // do in separate loop so all extra examples are added
  foreach ($files as $file_name => $file) {
    // check if dependencies are satisfied
    if (! empty($files[$file_name]->info['dependencies'])) {
      $files[$file_name]->info['missing'] = array();
      foreach ($files[$file_name]->info['dependencies'] as $dep) {
        if (! module_exists($dep)) {
          $files[$file_name]->info['missing'][] = $dep;
        }
      }
    }
  }

  // fixme: not working
  //uasort($files, 'system_sort_modules_by_info_name');

  // pull package information from file list and start grouping files
  foreach ($files as $file) {
    if (!isset($file->info['package']) || !$file->info['package']) {
      $file->info['package'] = t('Other');
    }
    if ($file->info['name']) {
      $packages[$file->info['package']][$file->name] = $file->info;
    }
  }
  ksort($packages);

  $modules_list = drupal_system_listing('\.module$', 'modules', 'name', 0);
  $form = array();
  foreach ($packages as $package => $files) {
    $form[$package] = array(
        '#type' => 'fieldset',
        '#title' => $package,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        );
    asort($files);
    foreach ($files as $file_name => $file_info) {
      $group = $file_info['name'];
      $field_name = 'views_examples_'.$file_name;
      $dependency_desc = '';
      $dependencies = array();
      if (is_array($file_info['dependencies'])) {
        foreach ($file_info['dependencies'] as $dependency) {
          if (in_array($dependency, $file_info['missing'])) {
            if (in_array($dependency, array_keys($modules_list))) {
              $dependencies[] = t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => $dependency));
            }
            else {
              $dependencies[] = t('@module (<span class="admin-missing">missing</span>)', array('@module' => $dependency));
            }
          }
          else {
            $dependencies[] = t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => $dependency));
          }
        }
        if (! empty($dependencies)) {
          $dependency_desc = 
            '<div class="admin-dependencies">'.
            t('Depends on: !dependencies', array('!dependencies' => implode(', ', $dependencies))).
            '</div>';
        }
      }
      $attributes = array();
      if (! empty($file_info['missing'])) {
        $attributes = array('disabled' => 'disabled');
      }
      if (! empty($file_info['extra'])) {
        // check dependencies
        if (! isset($form[$package][$group])) {
          $form[$package][$group] = array(
              '#type' => 'fieldset',
              '#title' => $group,
              '#description' => $file_info['description'],
              '#collapsible' => TRUE,
              '#collapsed' => FALSE,
              );
          if ($dependency_desc) {
            $form[$package][$group]['#description'] .= $dependency_desc;
          }
        }
        // fixme: tack on name/description from views include file
        $form[$package][$group][$field_name] = array(
            '#type' => 'checkbox',
            '#title' => $file_name,
            '#default_value' => variable_get($field_name, FALSE),
            '#attributes' => $attributes,
            );
      }
      else if (isset($file_info['name'])) {
        $form[$package][$field_name] = array(
            '#type' => 'checkbox',
            '#title' => $file_info['name'],
            '#description' => $file_info['description'],
            '#default_value' => variable_get($field_name, FALSE),
            '#attributes' => $attributes,
            );
        if ($dependency_desc) {
          $form[$package][$field_name]['#description'] .= $dependency_desc;
        }
      }
    }
  }
  //print_r($files);
  return system_settings_form($form);
}
