<?php

// $Id: site.inc,v 1.2.2.3 2010/05/06 15:47:56 brynbellomy Exp $

/**
 * Essentially this is a PURL record with some extra data attached.
 */

class site {
  var $sid;
  var $purl_prefix;
  var $title;
  var $name;
  var $front_page;
  var $menu_primary_links;
  var $menu_secondary_links;

  function __construct($title = '', $purl_prefix = '', $name = '', $front_page = '', $mpl = '', $msl = '', $sid = NULL) {
    $this->title = $title;
    $this->purl_prefix = $purl_prefix;
    $this->name = $name;
    $this->front_page = $front_page;
    $this->menu_primary_links = $mpl;
    $this->menu_secondary_links = $msl;
    $this->sid = $sid;
  }

  function save() {
    if($this->sid == NULL) {
      if(count(_sites_get_sites()) == 0) {
        $this->sid = 1;
      }
      else {
        $this->sid = max(array_keys(_sites_get_sites())) + 1;
      }
      drupal_write_record('sites', $this);
    }
    else {
      drupal_write_record('sites', $this, 'sid');
    }

    $modifier = array(
      'provider' => 'sites',
      'id' => $this->sid,
      'value' => $this->purl_prefix,
    );

    purl_save($modifier);
  }
}
