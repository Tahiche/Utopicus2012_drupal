<?php

// $Id: sites.purl.inc,v 1.1.2.1 2010/02/19 21:13:42 brynbellomy Exp $


/**
 * Implementation of hook_purl_provider().
 */
function sites_purl_provider() {
  $items = array();
  $items['sites'] = array(
    'name' => 'Sites module',
    'description' => t('Sets a context based on PURL modifier.'),
    'callback' => 'sites_set_current_site',
    'callback arguments' => NULL,
    'example' => 'somesite',
  );
  return $items;
}






