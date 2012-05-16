<?php
// $Id$

/**
 * @file
 * Magento structure views filter
 */

class magento_structure_universal_handler_filter extends views_handler_filter {

  function get_current_structure_nid() {
    throw new Exception('This class cannot be used directly');
  }

  function ui_name($short = FALSE) {
    throw new Exception('This class cannot be used directly');
  }

  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return array(
      '=' => t('Is equal to'),
      '!=' => t('Is not equal to'),
    );
  }


  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 30,
      '#default_value' => is_numeric($this->value) ? $this->value : 'auto',
      '#description' => t('Leave non number for current auto detected, or enter nid')
    );
  }

  /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . $this->operator . " %d", (is_numeric($this->value) ? $this->value : $this->get_current_structure_nid()));
  }

}
