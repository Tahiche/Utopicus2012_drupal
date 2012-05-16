<?php
// $Id: magento_my_orders_address.tpl.php, ... $

/**
 * @file
 * Default theme implementation for magento_my_orders_address
 * Available variables:
 * - $address: raw address data assosiative array
 * - $address_block: rendered address block
 */
?>
<div class="order-address <?php print $address['address_type'];?>">
  <?php print $address_block; ?>
</div>