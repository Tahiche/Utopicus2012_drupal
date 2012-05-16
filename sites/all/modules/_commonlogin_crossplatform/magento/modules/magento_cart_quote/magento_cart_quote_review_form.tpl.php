<?php
// $Id: magento_cart_quote_review.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_review.tpl.php
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
global $__magento_cart_lock_lost;
?>
<?php if ($__magento_cart_lock_lost) :?>
<div id="cart-content">
  <b><?php print t('Maybe you start another checkout process in separate window?') ?></b>
</div>
<?php return; endif;?>

<div id="cart-content">
  <?php print $cart_navigation; ?>

  <div id="block-confirmation-alerte">
    <strong><?php print t('Final order review.');?></strong>
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
      <b><?php print t('Payment method');?></b>
    </legend>
    <div id="block-confirmation-payment">
      <?php print $payment_method['title']?> <?php print $payment_method['description']?><br/>
    </div>
  </fieldset>

  </div>

  <?php print drupal_render($form); ?>

</div>