<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to Utopicus_Comunidad_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: Utopicus_Comunidad_breadcrumb()
 *
 *   where Utopicus_Comunidad is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/**
 * Implementation of HOOK_theme().
 */
/*function utopicuscomunidad_theme(&$existing, $type, $theme, $path) {
    // Add your theme hooks like this:
//  $hooks['hook_name_here'] = array( // Details go here );

  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks; 
}*/

function utopicuscomunidad_preprocess_page(&$vars, $hook) {
	
  if(isset($_GET['krumo']) && $_GET['krumo']=="p"){
	krumo($vars);
  }
  
  if($vars['node']->nid==1503) {
	drupal_add_css('sites/all/themes/utopicuscomunidad/css/all.css','theme');
	drupal_add_css('sites/all/themes/utopicuscomunidad/css/fancybox.css','theme');
	$vars['styles'] = drupal_get_css();
	drupal_add_js('sites/all/themes/utopicuscomunidad/js/jquery-main.js', 'theme');
	$vars['scripts'] = drupal_get_js();
  }
  //print_r($vars['node']->links['print']['title']);
  //print_r($vars['node']->links['print']['title']);
  //print_r($vars['menu_item']->page_arguments[0]->links['print']['title']);
   // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
}

function utopicuscomunidad_preprocess_node(&$vars, $hook) {
  
   if(isset($_GET['krumo'])&& $_GET['krumo']=="n"){
	krumo($vars);
  }
    // Optionally, run node-type-specific preprocess functions, like
  // Utopicus_Comunidad_preprocess_node_page() or Utopicus_Comunidad_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
  // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
  
  $vars['template_files'][] = 'node-'. $vars['node']->nid;  
}


/**
 * Implementation of theme_upload_attachments().
 * Theme the attachments output.
 * @param $files
 *  Array of file objects (descriptors from node).
 */
function utopicuscomunidad_itweak_upload_upload_attachments($files) {
  $stats = function_exists('_download_count_stats');

//krumo($files);

  $header = $stats
    ? array(
        array('data' => t('Preview'), 'class' => 'preview'),
        array('data' => t('Attachment'), 'class' => 'file'),
//        array('data' => t('Hits'), 'class' => 'download_count', ), array('data' => t('Last download'), 'class' => 'download_last'),
        array('data' => t('Count / Last Download'), 'class' => 'download_stats', 'colspan' => 2),
        array('data' => t('Size'), 'class' => 'size'))
    : array(
        array('data' => t('Preview'), 'class' => 'preview'),
        array('data' => t('Attachment'), 'class' => 'file'),
        array('data' => t('Size'), 'class' => 'size'));
  $rows = array();
  foreach ($files as $file) {
    $file = (object)$file;
    if ($file->list && empty($file->remove) && empty($file->hidden)) {
      $extension = strtolower(substr(strrchr($file->filename, '.'), 1));
      $href = _itweak_upload_file_create_url($file);
      $text = $file->description ? $file->description : $file->filename;
      $row = array();
      if (isset($file->preview)) {
        $row[] = array(
          'data' => drupal_render($file->preview),
          'class' => 'mime mime-' . $extension,
        );
      }
      else {
		  
						// como pongo thumb quito el thumb del tipo de archivo es imagen
						if (imagefield_file_is_image($file)) {
								$row[] = array(
								  'data' => '<a href="/' .$file->filepath . '" frameborder="0" rel="lightframe[search|width:900px; height:400px; scrolling: auto;]['.htmlentities($lightboxlink).']">'.theme('imagecache', 'AttachmentThumbnail', $file->filepath, $alt = $text, $title = $text).'</a>',
								  'class' => '',
								);
							}
						 else{
								$row[] = array(
								  'data' => '',
								  'class' => 'mime mime-' . $extension,
								);
						 }
      }
      $options = isset($file->preview_options) ? $file->preview_options : array();
      if (!$file->access || !(isset($file->preview) || _itweak_upload_isimage($file) || isset($options['custom'])))
        $options = array();
	// mio
	  $lightboxlink="<a target='_BLANK' href=\"".$href."\">".t('Open in new window')."</a>";

// $file=(object)$item;
	//krumo($file);
	//echo "imagefield_file_is_image".imagefield_file_is_image($file);
	if (!imagefield_file_is_image($file)){
$rowdata='<a href="http://docs.google.com/viewer?url=' . $href . '&amp;embedded=true" frameborder="0" rel="lightframe[search|width:900px; height:400px; scrolling: auto;]['.htmlentities($lightboxlink).']">'.$text.'</a>';
}
else{
$rowdata= '<a href="/' .$file->filepath . '" frameborder="0" rel="lightframe[search|width:900px; height:400px; scrolling: auto;]['.htmlentities($lightboxlink).']">'.$text.'</a>';
 
 // theme("imagefield_admin_thumbnail",$item)
}
	  
//$rowdata='<a href="http://docs.google.com/viewer?url=' . $href . '&amp;embedded=true" frameborder="0" rel="lightframe[search|width:900px; height:400px; scrolling: auto;]['.htmlentities($lightboxlink).']">'.$text.'</a>';
	   
      $row[] = array(
	 // originaL
       // 'data' => l($text, $href, $options),
	// mio
	     
	     'data' => $rowdata ,
        'class' => 'file',
      );
      if ($stats) {
        _download_count_stats($file);
        $row[] = array('data' => $file->download_count, 'class' => 'download_count',);
        $row[] = array('data' => $file->download_last , 'class' => 'download_last',);
      }
      $row[] = array(
        'data' => format_size($file->filesize),
        'class' => 'size',
      );
      $rows[] = $row;
    }
  }
  if (count($rows)) {
    return
      '<div class="itu-attachments">'
      . theme('table', $header, $rows, array(
          'class' => 'itu-attachment-list' . ($stats ? ' withstats' : ' withoutstats'), 'id' => 'attachments'
        ))
      .'</div>'
    ;
  }
}


/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function Utopicus_Comunidad_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function Utopicus_Comunidad_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function Utopicus_Comunidad_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // Utopicus_Comunidad_preprocess_node_page() or Utopicus_Comunidad_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function Utopicus_Comunidad_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function Utopicus_Comunidad_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */
