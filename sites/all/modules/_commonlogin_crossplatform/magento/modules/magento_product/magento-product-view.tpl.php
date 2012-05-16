<?php
// $Id: magento-product-view.tpl.php, Exp $
/**
 * @file
 * Theme implemantation for Magento Poduct node
 */
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php print(($sticky)? ' sticky' : ''); ?><?php print((!$status)? ' node-unpublished' : ''); ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>

  <div class="content clear-block">
    <div class="product-image">
      <div class="main-product-thumbnail"><?php print theme('magento_product_image', $product->field_thumbnail[0]['value']) ?></div>
    </div>
    <div class="product-info product display">
      <?php if (_magento_product_image_path($product->field_image[0]['value'])): ?>
      <a href="<?php print _magento_product_image_path($product->field_image[0]['value']) ?>" target="_blank" class="magento-product-image-zoom"><?php print t("Zoom") ?></a>
      <?php endif; ?>
      <span class="magento-product-price magento-product-price-display"><?php print theme('magento_product_price', $product); ?></span>
    </div>
    <div class="product-info model"><?php print t("SKU: @sku", array('@sku' => $product->field_sku[0]['value'])) ?></div>
    <div class="product-body"><p><?php print $product->field_description[0]['value'] ?></p></div>
    <div class="product-info product price"><?php print theme('magento_product_price', $product); ?></div>
    <div class="add-to-cart"><?php print $node->content['add_to_cart']['#value'] ?></div>
  </div>

  <div class="clear-block">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>
