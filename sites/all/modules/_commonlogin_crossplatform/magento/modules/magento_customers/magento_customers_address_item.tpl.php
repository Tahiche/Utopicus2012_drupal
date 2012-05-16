<?php
// $Id: magento_customers_address_item.tpl.php,v ... $

/**
 * @file
 * magento_customers_address_item.tpl.php
 * Default theme implementation for wrapping customers's address info.
 * Available variables:
 * - $address - array of address information data
 * - $account - user object
 */
?><span class="customer-addr-info">
<strong><?php print $address['firstname'] . ' ' . $address['lastname'] ?></strong> :
 <?php print $address['street'] ?>,
<?php print $address['postcode'] . ' ' . $address['city'] ?>
</span>