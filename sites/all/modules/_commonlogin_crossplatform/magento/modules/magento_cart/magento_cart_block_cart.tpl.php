<?php
// $Id: magento_cart_block_cart.tpl.php, Exp $
/**
 * @file
 * magento_cart_block_cart.tpl.php
 * Default theme implementation `magento_cart_block_cart` block.
 *
 */
?>
<div class="block-my-cart">
  <div class="block-title"><h3><?php print t('My cart');?></h3></div>
  <div id="magento_cart_state" ><?php print theme('magento_cart_state'); ?></div>
</div>