<?php
// $Id$

/**
 * @file
 *  Template.
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
  <?php print drupal_render($form); ?>
</div>