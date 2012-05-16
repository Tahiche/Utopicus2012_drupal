<?php
// $Id: magento_cart_quote_nav.tpl.php,v ... $

/**
 * @file
 * magento_cart_quote_nav.tpl.php
 * Default theme implementation magento_cart state's navigation.
 *
 * Available variables:
 * - $cart_states: - array of cart states.
 *
 * @see template_preprocess_magento_cart_nav()
 * <a href="<?php print $state['url']; ?>" ><?php print $state['title']; ?></a>
 */
?>
<div id="cart-nav">
  <div id="cart-nav-inner">
    <ul class="cart-nav-links">
    <?php foreach ($cart_states as $key => $state) { ?>
      <li class="<?php print $state['class'];?>">
          <?php if (!empty($state['path'])) : ?>
            <a href="<?php print url($state['path']);?>"><?php print $state['title']?></a>
          <?php else : ?>
            <?php print $state['title']; ?>
          <?php endif;?>
      </li>
    <?php } ?>
    </ul>
  </div>
</div>