<?php
// $Id$

/**
 * @file
 * Magento structure views filter
 */

class magento_structure_website_handler_filter extends magento_structure_universal_handler_filter {
  function get_current_structure_nid() {
    $nid = magento_structure_website_nid();
    return $nid ? $nid : -1;
  }

  function ui_name($short = FALSE) {
    return 'Magento structure website';
  }
}
