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
 *   entirety and paste it here, changing the prefix from theme_ to utopicus_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: utopicus_breadcrumb()
 *
 *   where utopicus is the name of your sub-theme. For example, the
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
function utopicus_theme(&$existing, $type, $theme, $path) {
		
 /* $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  
  $hooks['header_menu'] = array(
          'arguments' => array('page_vars' => NULL),
          'template' => 'templates/menus/primary-menu', // this is the name of the template
        );
  $hooks['page_header']=array(
     'arguments' => array('page_vars' => NULL),
     'template' => 'templates/page-header'
    );
  $hooks['page_footer']=array(
     'arguments' => array('page_vars' => NULL),
     'template' => 'templates/page-footer'
    );
 
//array('arguments' => array('links' => NULL));
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
  /* return array(
     'page_header' => array(
     'arguments' => array('page_vars' => NULL),
     'template' => 'templates/page-header'
    ),
    'page_footer' => array(
     'arguments' => array('page_vars' => NULL),
     'template' => 'page-footer'
    ),
	
	
    
  );*/
}

function utopicus_preprocess_page(&$vars, $hook) {
	
	//krumo($vars);
	
	
	//drupal_add_css('http://fonts.googleapis.com/css?family=Asap:400,700,700italic,400italic');
	/*drupal_add_css('http://fonts.googleapis.com/css?family=Asap:400,700,700italic,400italic', 'theme', 'all', FALSE);
	$vars['styles'] = drupal_get_css();*/

	// añadimos css de Google Fonts
	$vars['styles'] .= '<link href="http://fonts.googleapis.com/css?family=Asap:400,700,700italic,400italic" rel="stylesheet" type="text/css" />';

//$vars['styles'] .= '<link href="/'.path_to_theme().'/css/utopicfront.css?r='.rand().'" rel="stylesheet" type="text/css" />';

	$vars['page_header']=theme('page_header',$vars);
	$vars['page_footer']=theme('page_footer',$vars);

	// $vars['page_header']="ewrewweewewew";


		// cambio el titulo de paginas de usuario
	/*if(arg(0)=="user" && is_numeric(arg(1))){
		$vars['title']="Comunidad";
		}	*/
	if (isset($vars['node'])){
		//$path = explode('/', drupal_get_path_alias($_GET['q']));
		// If the node type is "blog" the template suggestion will be "page-blog.tpl.php".
		$vars['template_files'][] = 'page-'. str_replace('_', '-', $vars['node']->type);

		if(arg(2)=="delete" ){
			$vars['template_files'][] = 'page-delete';
		}

		/* CSS para cada tipo de nodo */
		$path_style = path_to_theme() .'/css/'. $vars['node']->type .'.css';
			if (file_exists($path_style)) {
			$include_style = $path_style;
		}

		if (isset($include_style)) {
			drupal_add_css($include_style, 'theme', 'all', FALSE);
			$vars['styles'] = drupal_get_css();
		}
		
		
	}
	// por ejemplo empty pages, no da tipo de nodo...
	else{
	
	}

	/* CSS para cada ruta/template de pag */
	$nombreTemplate=str_replace("page-","",$vars['template_files'][0]);
	//echo "nombreTemplate ".$nombreTemplate;
	$path_style = path_to_theme() .'/css/'.$nombreTemplate.'.css';

	//echo "    path_style ".$path_style;

	if (file_exists($path_style)) {
		$include_style = $path_style;
	}
	//echo "    include_style ".$include_style;
	if (isset($include_style)) {
		drupal_add_css($include_style, 'theme', 'all', FALSE);
		$vars['styles'] = drupal_get_css();
	}
	//drupal_set_html_head('<link type="text/css" rel="stylesheet" href="http://fonts.googleapis.com/css?family=Asap:400,700,700italic,400italic" />');

	if(isset($_GET['krumo'])){ /***************************** quitar en produccion ********************/
		
		 krumo($vars);
		
	}
	// me cargo los comments del output por defecto y los asigno a variables
	$vars['comments'] = $vars['comment_form'] = '';
  if (module_exists('comment') && isset($vars['node'])) {
    $vars['comments'] = comment_render($vars['node']);
    $vars['comment_form'] = drupal_get_form('comment_form', array('nid' => $vars['node']->nid));
  }
	//$vars['template_files'][]="page-miguel-es-un-capullo";

	//print_r($vars['node']->links['print']['title']);
	//print_r($vars['node']->links['print']['title']);
	//print_r($vars['menu_item']->page_arguments[0]->links['print']['title']);

	// To remove a class from $classes_array, use array_diff().
	//$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
	// krumo($vars);

	return $vars;
}

function utopicus_preprocess_node(&$vars, $hook) {
  // me cargo los comments del output por defecto y los asigno a variables en page
    $vars['node']->comment = 0;
	
  // krumo($vars);
  //default template suggestions for all nodes
  $vars['template_files'] = array();
  $vars['template_files'][] = 'node';

  if (drupal_is_front_page()) {
    $vars['template_files'][] = 'node-front';
  }

  //individual node being displayed
  if($vars['page']) {
    $vars['template_files'][] = 'node-page';
    $vars['template_files'][] = 'node-'.$vars['node']->type.'-page';
    $vars['template_files'][] = 'node-'.$vars['node']->nid.'-page';
	
	// según lo elegido en el campo template de la Pagina Utopicus asignamos un template para el nodo
  if($vars['type']=="pageutopicus"){
	  
	  switch($vars['field_tipotemplate'][0]['value']){
		  case '111':
		   $vars['template_files'][] = 'node-template-article';
		  break;
		  case '112':
		  $vars['template_files'][] = 'node-template-content-a-page-ca';
		  break;
		  case '113':
		   $vars['template_files'][] = 'node-template-content-b-page-cb';
		  break;
		   case '501':
		   $vars['template_files'][] = 'node-template-content-grande';
		  break;
		  default:
		  break;
		  }
	  // krumo($vars);
	  } // fin -- if($vars['type']=="pageutopicus
	  
  }
  //multiple nodes being displayed on one page in either teaser
  //or full view
  else {
    //template suggestions for nodes in general
    $vars['template_files'][] = 'node-'.$vars['node']->type;
    $vars['template_files'][] = 'node-'.$vars['node']->nid;

    //template suggestions for nodes in teaser view
    //more granular control
    if($vars['teaser']) {
		$vars['template_files'][] = 'node-teaser';
      $vars['template_files'][] = 'node-'.$vars['node']->type.'-teaser';
      $vars['template_files'][] = 'node-'.$vars['node']->nid.'-teaser';
	  
    }
  }
 
  // fivestar widget como variable
   $vars['fivestar_widget']="<div id='fivestardiv'>".$vars['node']->content['fivestar_widget']['#value']."</div>";
	
	$vars['terms_by_vocab']=utopicus_separate_terms($vars['node']->taxonomy);
  // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
  if(isset($_GET['krumon'])){
	krumo($vars);
  }
}


function utopicus_separate_terms($node_taxonomy) {
   if ($node_taxonomy) {

foreach ($node_taxonomy AS $term) {
 $links[$term->vid]['taxonomy_term_'. $term->tid] = array(
   'title' => $term->name, 
  'href' => taxonomy_term_path($term),
'attributes' => array(
   'rel' => 'tag',
   'title' => strip_tags($term->description)
   ),
 );
}
   //theming terms out
     foreach ($links AS $key => $vid) {
 $terms[$key] = theme_links($vid);
   }
  }
      return $terms;
    }


/**
* Change submit button to search in exposed filters.
*/

/*function utopicus_preprocess_views_exposed_form(&$vars, $hook) {
	// krumo($vars['form']['#id']);
  // only alter the search view exposed filter form
  if ($vars['form']['#id'] == 'views-exposed-form-coworkers-grid-page-1') {

      
    // Change the text on the submit button to Search
    $vars['form']['submit']['#value'] = t('Filtrar');
    unset($vars['form']['submit']['#printed']);
    $vars['button'] = drupal_render($vars['form']['submit']);
  }

}*/



/*function utopicus_pager_next($text, $limit, $element = 0, $interval = 1, $parameters = array()) {
  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    $page_new = pager_load_array($pager_page_array[$element] + $interval, $element, $pager_page_array);
    // If the next page is the last page, mark the link as such.
    if ($page_new[$element] == ($pager_total[$element] - 1)) {
      $output = theme('pager_last', $text, $limit, $element, $parameters);
    }
    // The next page is not the last page. 
    else {
      $output = theme('pager_link', $text, $page_new, $element, $parameters);
    }
  }

  return "pagerrrrrrr".$output;
}*/
/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function utopicus_preprocess(&$vars, $hook) {
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
function utopicus_preprocess_page(&$vars, $hook) {
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
function utopicus_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // utopicus_preprocess_node_page() or utopicus_preprocess_node_story().
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
function utopicus_preprocess_comment(&$vars, $hook) {
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
function utopicus_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */
