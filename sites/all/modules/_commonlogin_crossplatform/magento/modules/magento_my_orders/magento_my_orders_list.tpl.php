<?php
// $Id: magento_my_orders_list.tpl.php, ... $

/**
 * @file
 * Default theme implementation for magento_my_orders_list
 * Available variables:
 * - $orders: (an array) raw orders list retruned from magento.
 * - $orders_list: (string) rendered orders table
 * - $pager: (string) rendered paginator
 */
?>
<?php print $orders_list; ?>
<?php print $pager;