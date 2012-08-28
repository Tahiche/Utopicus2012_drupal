<?php

// $Id: sites_handler_filter_site_current.inc,v 1.1.2.3 2010/03/31 20:05:36 brynbellomy Exp $

class sites_handler_filter_site_current extends views_handler_filter {

  function query() {
    $sid = context_get('sites','sid');
    if ($sid) {
      if ($this->relationship) {
        $base_table = $this->view->query->relationships[$this->relationship]['table'];
      }
      else {
        $base_table = $this->view->base_table;
      }

      switch ($base_table) {
        case 'node':
          $table = $this->view->query->ensure_table('site_node', $this->relationship);

          if (isset($this->options['display_all_nodes']) && $this->options['display_all_nodes']) {
            $this->view->query->add_where(0, "$table.sid = %d OR $table.sid IS NULL", $sid);
          }
          else {
            $this->view->query->add_where(0, "$table.sid = %d", $sid);
          }
          break;
      }
    }
    else if ($this->options['operator'] === 'all') {
      // Set a failure condition and let the display manager handle it.
      $this->view->build_info['fail'] = TRUE;
    }
  }

  function admin_summary() {
    return 'Current site';
  }

  function value_form(&$form, &$form_state) {
    $form['display_all_nodes'] = array(
      '#type' => 'radios',
      '#title' => t('Display nodes without attached sites'),
      '#default_value' => isset($this->options['display_all_nodes']) ? $this->options['display_all_nodes'] : 0,
      '#options' => array(1 => t('Yes'), 0 => t('No')),
    );
  }
}
