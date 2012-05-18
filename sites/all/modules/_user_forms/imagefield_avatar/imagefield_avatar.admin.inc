<?php

/**
 * @file
 * ImageField Avatar admin pages code.
 */

/**
 * Private version of hook_form_alter().
 */
function _imagefield_avatar_form_alter(&$form, &$form_state, $form_id) {
  switch ($form_id) {
    case 'content_field_edit_form':
      // Update default user picture settings according to imagefield default
      // image settings.
      $avatar_field = variable_get('imagefield_avatar_field', '');
      $avatar_type = variable_get('imagefield_avatar_content_type', '');
      if ($form['#field']['field_name'] == $avatar_field && $form['#field']['type_name'] == $avatar_type) {
        $form['#submit'][] = '_imagefield_avatar_content_field_edit_form_submit';
      }
      break;

    case 'user_profile_form':
      unset($form['picture']);
      break;

    case 'user_admin_settings':
      module_load_include('inc', 'hs_field_selector');
      $form['#submit'][] = '_imagefield_avatar_user_admin_settings_submit';
      $form['#validate'][] = '_imagefield_avatar_user_admin_settings_validate';
      $form['pictures']['settings']['user_picture_imagefield'] = hs_field_selector_form_element();
      $form['pictures']['settings']['user_picture_imagefield']['#weight'] = -10;
      $form['pictures']['settings']['user_picture_imagefield']['#required'] = TRUE;
      $form['pictures']['settings']['user_picture_imagefield']['#title'] = t('Picture field');
      $form['pictures']['settings']['user_picture_imagefield']['#description'] = t('Please select imagefield/filefield from a content type to serve as user picture (avatar). If the content type list is empty you need to add an imagefield to some content type first.');
      $form['pictures']['settings']['user_picture_imagefield']['#default_value'] = array(
        0 => variable_get('imagefield_avatar_content_type', ''),
        1 => variable_get('imagefield_avatar_field', ''),
      );
      $form['pictures']['settings']['user_picture_imagefield']['#config']['special_items'] = array(
        '' => array('none', 'exclusive')
      );
      $form['pictures']['settings']['user_picture_imagefield']['#config']['level_labels']['status'] = 0;
      $form['pictures']['settings']['user_picture_imagefield']['#config']['params'] = array(
        'field_filter' => '_imagefield_avatar_field_filter',
        'nodetype_filter' => '_imagefield_avatar_nodetype_filter',
      );
      $form['pictures']['settings']['user_picture_path']['#type'] = 'hidden';
      $form['pictures']['settings']['user_picture_default']['#type'] = 'hidden';
      $form['pictures']['settings']['user_picture_dimensions']['#type'] = 'hidden';
      $form['pictures']['settings']['user_picture_file_size']['#type'] = 'hidden';
      $form['pictures']['settings']['user_picture_guidelines']['#type'] = 'hidden';
      break;
  }
}


function _imagefield_avatar_user_admin_settings_submit($form, &$form_state) {
  $type = $form_state['values']['user_picture_imagefield'][0];
  $field = $form_state['values']['user_picture_imagefield'][1];
  variable_set('imagefield_avatar_content_type', $type);
  variable_set('imagefield_avatar_field', $field);
  // Update default user picture setting.
  if (!empty($type)) {
    $types = content_types();
    $widget = $types[$type]['fields'][$field]['widget'];
  }
  else {
    $widget = array();
  }
  _imagefield_avatar_sync_default_images($widget);
}

function _imagefield_avatar_user_admin_settings_validate ($form, &$form_state) {
  if (!_nodetype_field_valid($form_state['values']['user_picture_imagefield'][0], $form_state['values']['user_picture_imagefield'][1])) {
    form_set_error('user_picture_imagefield', t('Illegal selection detected.'));
  }
}

function _imagefield_avatar_content_field_edit_form_submit($form, &$form_state) {
  _imagefield_avatar_sync_default_images($form_state['values']);
}

function _imagefield_avatar_sync_default_images($widget) {
  if (isset($widget['use_default_image']) &&
      $widget['use_default_image'] == 1 &&
      is_array($widget['default_image']) && 
      !empty($widget['default_image']['filepath'])) {
    variable_set('user_picture_default', $widget['default_image']['filepath']);
  }
  else {
    variable_set('user_picture_default', '');
  }
}
