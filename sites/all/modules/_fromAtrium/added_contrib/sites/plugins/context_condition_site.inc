<?php

// $Id: context_condition_site.inc,v 1.1.2.1 2010/02/19 21:13:42 brynbellomy Exp $

/**
 * Expose sites as a context condition.
 */
class context_condition_site extends context_condition {
  function condition_values() {
    $options = array();
    $sites = _sites_get_sites();
    foreach($sites as $site) {
      $options[$site->sid] = $site->title . ' (PURL prefix: ' . $site->purl_prefix . ')';
    }

    return $options;
  }

  function execute($sid) {
    foreach ($this->get_contexts($sid) as $context) {
      $this->condition_met($context, $sid);
    }
  }
}
