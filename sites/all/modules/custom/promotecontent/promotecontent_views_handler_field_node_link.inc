<?php
// $Id: promotecontent_views_handler_field_node_link.inc,v 1.1.2.7 2010/07/01 19:52:14 ambiguator Exp $

/**
 * Field handler to present a link node publish.
 */
class promotecontent_views_handler_field_node_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['promote'] = 'promote';
    $this->additional_fields['uid'] = 'uid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['promote'] = 
    $options['unpromote'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['promote'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for promoting'),
      '#default_value' => $this->options['promote'],
    );
    $form['unpromote'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for unpromoting'),
      '#default_value' => $this->options['unpromote'],
    );
  }


  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // ensure user has access to edit this node.
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->type = $values->{$this->aliases['type']};
    $node->promote = $values->{$this->aliases['promote']};
    $node->uid = $values->{$this->aliases['uid']};

    $options = array('query' => drupal_get_destination());

    if ($node->promote && _promotecontent_unpublish_access($node))
      $op = 'unpromote';
	
	
	
    if (!$node->promote && _promotecontent_publish_access($node))
      $op = 'promote';
	  
	
    if (isset($op))
      return l(empty($this->options[$op]) ? t($op) : $this->options[$op],
               "node/$node->nid/$op/". drupal_get_token(), $options);
  }
}
