<?php
// $Id: magento_cart_quote_content_wrapper.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_content_wrapper.tpl.php
 * Default theme implementation for wrapping magento cart content into div with ID.
 *
 * Available variables:
 * - $cart_content: renderred html with cart content form.
 */
?>
<div id="cart-wrapper">
  <?php print $cart_content; ?>
</div>