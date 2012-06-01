<?php
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
 // krumo($variables);
?>
<?php 
$columnL=true;
foreach ($rows as $id => $row): 
?>
  <div class="column <?php print $classes[$id]; ?>">
 <div class="post-block">
    <?php print $row; ?>
    </div>
  </div>
<?php endforeach; ?>