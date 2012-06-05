<?php
/**
 * @file
 * Theme implementation to display a node. 
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $display_submitted: whether submission information should be displayed.
 * - $submitted: Themed submission information output from
 *   theme_node_submitted().
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 *   The following applies only to viewers who are registered users:
 *   - node-by-viewer: Node is authored by the user currently viewing the page.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $build_mode: Build mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $build_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * The following variable is deprecated and will be removed in Drupal 7:
 * - $picture: This variable has been renamed $user_picture in Drupal 7.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see zen_preprocess()
 * @see zen_preprocess_node()
 * @see zen_process()
 */
?>
<div id="content">

 



<div id="node-<?php print $node->nid; ?>" class=" <?php print $classes; ?>">

<div class="heading">
<em class="date"><?php print $date; ?></em>
<div class="image">
<?php 
$imgtit=$node->field_img_ppal[0]['data']['title']?$node->field_img_ppal[0]['data']['title']:$node->title;
print theme("imagecache","Imagen_articulo_683x350",$node->field_img_ppal[0]['filepath'],$imgtit,$imgtit); ?>
</div>

<div class="holder">

<div class="addthis_toolbox addthis_default_style">
<ul class="social-networks">

<!-- AddThis Button BEGIN -->


<li><a class="addthis_button_facebook facebook" style="cursor:pointer"><span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
<li><a class="addthis_button_twitter twitter" style="cursor:pointer"><span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
<li><a class="addthis_button_email mail" style="cursor:pointer"><span class="mask" style="opacity: 1;">&nbsp;</span></a></li>

<script type="text/javascript" src="http://s7.addthis.com/js/250/addthis_widget.js#pubid=xa-4f472587705fa7b2"></script>
<!-- AddThis Button END -->

    <li><a class="facebook" href="#">facebook<span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
    <li><a class="twitter" href="#">twitter<span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
    <li><a class="igoogle" href="#">iGoogle<span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
    <li><a class="vimeo" href="#">vimeo<span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
    <li><a class="flickr" href="#">flickr<span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
    <li><a class="rss" href="#">rss<span class="mask" style="opacity: 1;">&nbsp;</span></a></li>
</ul>
</div>


</div>

</div>




 

  <div class="contenidonodo node-<?php print $node->type; ?> ">
    <?php print $content; ?>
  </div>
  
  
   <?php if ($display_submitted || $terms): ?>
    <div class="meta">
      <?php if ($display_submitted): ?>
        <span class="submitted">
          <?php print $submitted; ?>
        </span>
      <?php endif; ?>

      <?php if ($terms): ?>
        <div class="terms terms-inline"><?php print $terms; ?></div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php print $links; ?>
  </div>
  
  
</div><!-- /.node -->



<div id="sidebar" class="articder">
<div class="box">
<?php
//http://www.utopicus.consinergia.es/es/admin/build/block/configure/views/agenda_front-ultimas_entradas
if ($node->type=="noticia"):
$block = module_invoke('views', 'block', 'view','agenda_front-ultimas_entradas');
?>
<h2><?php print t("Últimas Entradas"); ?></h2>
<?php print $block['content']; ?>

<?php
// es agenda
elseif ($node->type=="actividad_agenda"):  
// $block = module_invoke('views', 'block', 'view','agenda_front-ultimas_entradas');
?>

<h2><?php print t("Categorías"); ?></h2>
<ul class="category-list">
<?php 
$items = array();
      $tree = taxonomy_get_tree(10);
	  //miKrumo($tree);
      if ($tree && (count($tree) > 0)) {
        $options = array();
        foreach ($tree as $term) {
         // $items[$term->tid] = str_repeat('-', $term->depth) . $term->name;
		  $items[$term->tid] = l($term->name, "taxonomy/term/".$term->tid);
		  print "<li>". $items[$term->tid]."</li>";
        }
      }
	  // theme_item_list($items, $title, $type, $attributes);
      
 ?>
 </ul>
<?php
else: 
?>

<?php endif; ?>
</div> <!-- fin box if else -->

    <!-- mas leido -->
    <div class="box">
    <h2><?php print t("Más leído"); ?></h2>
    <?php
    // http://www.utopicus.consinergia.es/es/admin/build/block/configure/views/-exp-coworkers_grid-page_3
    $block = module_invoke('views', 'block', 'view','popular-masvisto_block');
    print $block['content'];
    ?>
    </div>


</div> <!-- fin sidebar -->
