<div class="post-section banner-section"> 
  <?php if ($rows): ?>

    
    <div class="post-visual ">
      <?php print $rows; ?>
     </div>

  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>
</div>