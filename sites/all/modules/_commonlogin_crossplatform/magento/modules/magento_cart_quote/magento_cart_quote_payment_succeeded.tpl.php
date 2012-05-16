<?php
// $Id: magento_cart_quote_payment_succeeded.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_payment_succeeded.tpl.php
 * Default theme implementation for wrapping magento_cart PAYMENT_SUCCEDED state.
 *
 * Available variables:
 * - $cart_items: Array of Cart Items. Eeach Item is array of product details,
 *     like image, title, price, item controls.
 * - $cart_header: Cart header columns.
 * - $products_list: Cart Items rendered html-table format.
 * - $cart_navigation: shows navigation links to all magento_cart states.
 * - $shipping_address: shipping address array
 * - $billing_address: billing address array
 * - $order: object with order details
 * - $form: Cart PAYMENT controls.
 *
 * @see template_preprocess_magento_cart_quote_payment_succeeded_form()
 */
?>
<div id="cart-content">
  <?php print $cart_navigation; ?>

  <div id="block-confirmation-alerte">
    <strong><?php print t('Your order was registered!');?></strong><br /><?php print t('Confirmation email was sent. Thank you for the order.');?>
  </div>

  <?php print $products_list; ?>

  <div id="block-confirmation-informations" class="group">

  <fieldset>
    <legend class="collapse-processed">
      <b><?php print t('Shipping address');?></b>
    </legend>
    <div id="block-confirmation-adresseliv">
      <?php print $shipping_address['firstname']?> <?php print $shipping_address['lastname']?><br/>
      <?php print $shipping_address['street'] . ' ' . $shipping_address['postcode'] . ' ' . $shipping_address['city']; ?>
    </div>
  </fieldset>

  <fieldset>
    <legend class="collapse-processed">
      <b><?php print t('Billing address');?></b>
    </legend>
    <div id="block-confirmation-adressefac">
      <?php print $billing_address['firstname']?> <?php print $billing_address['lastname']?><br/>
      <?php print $billing_address['street'] . ' ' . $billing_address['postcode'] . ' ' . $billing_address['city']; ?>
    </div>
  </fieldset>

  <fieldset>
    <legend class="collapse-processed">
      <b><?php print t('Important information');?></b>
    </legend>
    <div id="block-confirmation-infos">
      <div><?php print t('Order Number');?> : <strong><?php print $order->order_id; ?></strong></div>
    </div>
  </div>
  </fieldset>

  <fieldset>
    <legend>
      <b><?php print t('Conditions');?></b>
    </legend>
    <div id="block-confirmation-text">
       <?php print drupal_render($form['confirmation_text']); ?>
    </div>
  </fieldset>

  <div class="conf-actions">
    <?php print l(t("<< Back to catalog"), '<front>');?>
    <a href="#print" onclick="window.print();return false;"><?php print t("Print Order");?></a>
  </div>

</div>

<style type="text/css" media="print">#cart-nav,div.conf-actions{display:none;}</style>