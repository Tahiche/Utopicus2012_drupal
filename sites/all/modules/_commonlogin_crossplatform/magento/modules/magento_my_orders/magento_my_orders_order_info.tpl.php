<?php
// $Id: magento_my_orders_order_info.tpl.php, ... $

/**
 * @file
 * Default theme implementation for magento_my_orders_order_info
 * Available variables:
 * - $order: an assosiative array with raw order data.
 * - $order_date: order created date
 * - $order_shipping_method - order shipping method label
 * - $order_shipping_address - rendered shipping address
 * - $order_payment_method - order payment method label
 * - $order_billing_address - rendered billing address
 * - $items_ordered - rendered html table with ordered items
 */
?>
<div class="order-info-wrapper">
  <div class="order-actions"><a href="#print" onclick="window.print();return false;"><?php print t('Print');?></a></div>
  <h3><?php print t("About this Order:");?> <strong><?php print t('Order Information');?></strong></h3>
  <div class="order-date"><?php print t("Order Date:");?><?php print $order_date;?></div>
  <div class="box shipping">
    <div class="method">
      <h4><?php print t('Shipping Method');?></h4>
      <?php print $order_shipping_method?>
    </div>
    <div class="address">
      <h4><?php print t('Shipping Address');?></h4>
      <?php print $order_shipping_address;?>
    </div>
  </div>
  <div class="box billing">
    <div class="method">
      <h4><?php print t('Payment Method');?></h4>
      <?php print $order_payment_method;?>
    </div>
    <div class="address">
      <h4><?php print t('Billing Address');?></h4>
      <?php print $order_billing_address;?>
    </div>
  </div>
  <?php print $items_ordered;?>
</div>