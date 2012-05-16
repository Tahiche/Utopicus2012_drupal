<?php
// $Id: magento_cart_quote_address_item.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_address_item.tpl.php
 * Default theme implementation for wrapping for cart customers's address.
 * Available variables:
 * - $address - array of address information data
 * - $account - user object
 */
?>
<span class="customer-addr-info">
<strong><?php print $address['firstname'] . ' ' . $address['lastname'] ?></strong> :
  <?php print $address['street_name'];?>,
  <?php print $address['postcode'] . ' ' . $address['city'] ?>
</span>
<?php
$addres_destination = array('query' => array('destination' => 'cart/delivery'));
if (empty($address['is_read_only'])) { ?>
 <?php print l( t('Modify') , "user/" . $account->uid . "/edit-address/{$address['customer_address_id']}", $addres_destination); ?>
 -
 <?php print l( t('Remove'), "user/" . $account->uid . "/remove-address/{$address['customer_address_id']}", $addres_destination); ?>
<?php
}