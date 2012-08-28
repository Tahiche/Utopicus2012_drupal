<?php

// $Id: sites_handler_filter_site.inc,v 1.1.2.3 2010/03/31 20:05:36 brynbellomy Exp $

class sites_handler_filter_site extends views_handler_filter_many_to_one {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = array();
    $sites = _sites_get_sites();
    foreach ($sites as $site) {
      $this->value_options[$site->sid] = $site->title;
    }
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['display_all_nodes'] = array(
      '#type' => 'radios',
      '#title' => t('Display nodes without attached sites'),
      '#default_value' => isset($this->options['display_all_nodes']) ? $this->options['display_all_nodes'] : 0,
      '#options' => array(1 => t('Yes'), 0 => t('No')),
    );
  }

  function query() {
    // adjust the last query
    if ($this->options['display_all_nodes']) {
      // make sure it is LEFT join
      if ($this->relationship) {
        $base_table = $this->view->query->relationships[$this->relationship]['table'];
      }
      else {
        $base_table = $this->view->base_table;
      }

      $this->view->query->ensure_table('site_node', $this->relationship);
      parent::query();

      $where = $this->query->where[$this->options['group']];

      $table = $this->ensure_my_table();
      $clause = array_pop($where['clauses']);

      $clause .= " OR $table.sid IS NULL";
      $where['clauses'][] = $clause;

      $this->query->where[$this->options['group']] = $where;
    } else {
      parent::query();
    }
  }
}
