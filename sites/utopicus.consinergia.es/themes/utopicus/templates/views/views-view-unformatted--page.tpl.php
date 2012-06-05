<?php
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
 // krumo($variables);
?>
<?php if (!empty($title)): ?> 
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php 
$columnL=true;
foreach ($rows as $id => $row): 
$align=$columnL ? 'alignleft' : 'alignright'; $columnL = !$columnL; ?>
  <div class="column <?php print $align; ?>  <?php print $classes[$id]; ?>">
 <div class="post-block">
    <?php print $row; ?>
    </div>
  </div>
<?php endforeach; ?>