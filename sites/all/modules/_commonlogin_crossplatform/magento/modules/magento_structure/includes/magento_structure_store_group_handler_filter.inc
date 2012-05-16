<?php
// $Id$

/**
 * @file
 * Magento structure views filters
 */

class magento_structure_store_group_handler_filter extends magento_structure_universal_handler_filter {
  function get_current_structure_nid() {
    $nid = magento_structure_group_nid();
    return $nid ? $nid : -1;
  }

  function ui_name($short = FALSE) {
    return 'Magento structure store group';
  }
}
