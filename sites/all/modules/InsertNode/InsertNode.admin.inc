<?php
// $Id: InsertNode.admin.inc,v 1.1.2.1 2010/03/22 03:00:12 alexiswilke Exp $
// Author: Alexis Wilke

/**
 * @file InsertNode.admin.inc
 *
 * Defines the InsertNode settings.
 */

/**
 * Implementation of filter operation 'settings'
 */
function _InsertNode_settings($format) {
  $form['InsertNode'] = array(
    '#type' => 'fieldset',
    '#title' => 'Insert node',
    '#description' => 'Additional options for the Insert Node filter.',
    '#collapsible' => TRUE,
  );

  $form['InsertNode']['InsertNode_override_' . $format] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow field overrides'),
    '#description' => t('Offer the [node:123 override=...;] capability. By default this is turned off since it allows the users to transform the node outside of the filter. Turn on at your own risk (i.e. view this flag as a permission flag.)'),
    '#default_value' => variable_get('InsertNode_override_' . $format, 0),
  );

  return $form;
}

// vim: ts=2 sw=2 et syntax=php
