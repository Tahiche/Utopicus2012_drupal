<?php
// $Id: magento_cart_quote_view.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_view.tpl.php
 * Default theme implementation for wrapping magento_cart VIEW state.
 *
 * Available variables:
 * - $cart_items: Array of Cart Items. Eeach Item is array of product details,
 *     like image, title, price, item controls.
 * - $cart_header: Cart header columns.
 * - $products_list: Cart Items rendered html-table format.
 * - $form: Cart VIEW controls
 *
 * @see template_preprocess_magento_cart_quote_view()
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
  <?php print $products_list; ?>
  <?php print drupal_render($form); ?>
</div>