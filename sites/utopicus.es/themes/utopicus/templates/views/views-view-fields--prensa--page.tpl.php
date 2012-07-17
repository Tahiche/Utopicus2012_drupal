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
 
if($fields['title']->content =='<span class="field-content">prueba2</span>'){
	//miKrumo($fields['nid']->raw);
	// miKrumo(array_shift($fields['upload_fid']->handler->items[$fields['nid']->raw]) );
}


?>
<div class="heading">
	<!-- date -->
<em class="date"><?php echo $fields['field_fecha_value']->content?></em>
<!-- category -->
<strong class="category"><?php 
// echo $cat;
?></strong>
</div>

<div class="image">
<?php echo $fields['field_img_ppal_fid']->content?>
</div>



<div class="text">
<h3><?php echo $fields['title']->content; ?></h3>
<p><?php echo $fields['field_textoprensa_value']->content; ?></p>

<?php 
echo $fields['field_enlace_url']->content;
?>

<?php 
echo $fields['upload_fid']->content;
//foreach ($fields as $id => $field):?>

</div>
                                                                                    


    <?php //print $field->content; ?>
    

<?php //endforeach; ?>