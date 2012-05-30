<?php
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */  
 // krumo($fields)
?>
<div id="content">

<div class="image">
<?php echo $fields['field_img_ppal_fid']->content?>
</div>


<div class="heading border">
	<!-- date -->
<em class="date"><?php echo $fields['created']->content?></em>

<h2><?php echo $fields['title']->content; ?></h2>

<!-- category -->
<strong class="category"><?php  
echo $fields['tid']->content
?></strong>
</div>





<div class="text block border">

<?php echo $fields['body']->content; ?>

<div class="holder">
<!-- social-networks -->
<?php echo theme('social_addthis'); ?>
<!-- categoria de la noticia -->
<div class="btn-category" ><?php  echo $fields['tid']->content; ?></div>
</div>
</div>
                                                                                    
<?php //foreach ($fields as $id => $field):?>

    <?php //print $field->content; ?>
    

<?php //endforeach; ?>
</div>