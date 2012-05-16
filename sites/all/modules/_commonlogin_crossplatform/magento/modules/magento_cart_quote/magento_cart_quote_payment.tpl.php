<?php
// $Id: magento_cart_quote_payment.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_payment.tpl.php
 * Default theme implementation for wrapping magento_cart PAYMENT state.
 *
 * Available variables:
 * - $cart_items: Array of Cart Items. Eeach Item is array of product details,
 *     like image, title, price, item controls.
 * - $cart_header: Cart header columns.
 * - $products_list: Cart Items rendered html-table format.
 * - $cart_navigation: shows navigation links to all magento_cart states.
 * - $payment_methods: array of available payment methods.
 * - $payment_methods_msg: Shows msg - if no $payment_methods is available.
 * - $form: Cart PAYMENT controls.
 *
 * @see template_preprocess_magento_cart_quote_payment_form()
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
  <?php print $cart_errors; ?>
  <?php print $products_list;?>
  <?php print drupal_render($form['magento_cart_quote_shipping_details']); ?>

  <fieldset>
    <legend><?php print t("Payment method:") ?></legend>
    <div class="in-bl"><?php print drupal_render($form['payment_mode']); ?></div>
    <?php print $payment_methods_msg; ?>
    <div class="paymethods method-radios">
    <?php foreach ((array)$payment_methods['paymethods'] as $paymethod => $method) { ?>
      <div class="paymethods-option group">
        <?php print drupal_render($form['magento_cart_quote_payment_methods'][$paymethod]); ?>
        <?php print drupal_render($form['paymethods'][$paymethod]); ?>
      </div>
    <?php } ?>
    </div>
  </fieldset>

  <?php print drupal_render($form); ?>
</div>

<script type="text/javascript">
setTimeout(function(){
  $('#magento-cart-quote-payment-form input[name=magento_cart_quote_payment_methods]').change(function(){
    sel_payment_method = $(this).val();
    $('#edit-payment-mode').val(sel_payment_method).change();
    $('input[name=magento_cart_quote_payment_methods]').attr('disabled', 'disabled');
  });

 }, 3);
</script>