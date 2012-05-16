<?php
// $Id: vppr.admin.inc,v 1.1 2011/01/27 15:02:41 boobaa Exp $

/**
 * @file
 * Vocabulary Permissions Per Role - administration UI
 */

function vppr_form_admin_settings_vppr() {
  $form = array();
  $vocabs = taxonomy_get_vocabularies();
  $roles = user_roles(TRUE); // List of roles without 'anonymous'.
  $perms = variable_get('vppr_perms', array(array()));
  foreach ($vocabs as $vocab) {
    $form['vppr_vocabs'][$vocab->vid] = array(
      '#value' => $vocab->name,
    );
    foreach ($roles as $rid => $role) {
      $form['vppr_perms'][$vocab->vid][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => $perms[$vocab->vid][$rid],
      );
    }
    $form['vppr_perms']['#tree'] = TRUE;
  }
  foreach ($roles as $rid => $role) {
    $form['vppr_roles'][$rid] = array(
      '#value' => $role,
    );
  }
  $form['help'] = array(
    '#type' => 'item',
    '#description' => t('Tick a box above to allow editing that vocabulary for that role.'),
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Theme helper for vppr_form_admin_settings_vppr().
 */
function theme_vppr_form_admin_settings_vppr($form) {
  $header = array(FALSE);
  foreach (element_children($form['vppr_roles']) as $rid) {
    $header[$rid] = drupal_render($form['vppr_roles'][$rid]);
  }
  $rows = array();
  foreach (element_children($form['vppr_vocabs']) as $vid) {
    $row = array(drupal_render($form['vppr_vocabs'][$vid]));
    foreach (element_children($form['vppr_perms'][$vid]) as $rid) {
      $row[$rid] = drupal_render($form['vppr_perms'][$vid][$rid]);
    }
    $rows[$vid] = $row;
  }
  $buttons = drupal_render($form['buttons']);
  return theme('table', $header, $rows) . drupal_render($form) . $buttons;
}

/**
 * Store VPPR perms to the DB.
 */
function vppr_form_admin_settings_vppr_submit($form, &$form_state) {
  $perms = array();
  foreach ($form_state['values']['vppr_perms'] as $vid => $values) {
    $values = array_filter($values);
    if (!empty($values)) {
      $perms[$vid] = $values;
    }
  }
  variable_set('vppr_perms', $perms);
}

