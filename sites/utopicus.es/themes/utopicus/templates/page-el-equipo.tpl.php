<?php
/**
 * @file
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $css: An array of CSS files for the current page.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page. Used to toggle the mission statement.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Page metadata:
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It should be placed within the <body> tag. When selecting through CSS
 *   it's recommended that you use the body tag, e.g., "body.front". It can be
 *   manipulated through the variable $classes_array from preprocess functions.
 *   The default values can be one or more of the following:
 *   - front: Page is the home page.
 *   - not-front: Page is not the home page.
 *   - logged-in: The current viewer is logged in.
 *   - not-logged-in: The current viewer is not logged in.
 *   - node-type-[node type]: When viewing a single node, the type of that node.
 *     For example, if the node is a "Blog entry" it would result in "node-type-blog".
 *     Note that the machine name will often be in a short form of the human readable label.
 *   - page-views: Page content is generated from Views. Note: a Views block
 *     will not cause this class to appear.
 *   - page-panels: Page content is generated from Panels. Note: a Panels block
 *     will not cause this class to appear.
 *   The following only apply with the default 'sidebar_first' and 'sidebar_second' block regions:
 *     - two-sidebars: When both sidebars have content.
 *     - no-sidebars: When no sidebar content exists.
 *     - one-sidebar and sidebar-first or sidebar-second: A combination of the
 *       two classes when only one of the two sidebars have content.
 * - $node: Full node object. Contains data that may not be safe. This is only
 *   available if the current page is on the node's primary url.
 * - $menu_item: (array) A page's menu item. This is only available if the
 *   current page is in the menu.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $mission: The text of the site mission, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $search_box: HTML to display the search box, empty if search has been disabled.
 * - $primary_links (array): An array containing the Primary menu links for the
 *   site, if they have been configured.
 * - $secondary_links (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title: The page title, for use in the actual HTML content.
 * - $messages: HTML for status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page (e.g., the
 *   view and edit tabs when displaying a node).
 * - $help: Dynamic help text, mostly for admin pages.
 * - $content: The main content of the current page.
 * - $feed_icons: A string of all feed icons for the current page.
 *
 * Footer/closing data:
 * - $footer_message: The footer message as defined in the admin settings.
 * - $closure: Final closing markup from any modules that have altered the page.
 *   This variable should always be output last, after all other dynamic content.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * Regions:
 * - $content_top: Items to appear above the main content of the current page.
 * - $content_bottom: Items to appear below the main content of the current page.
 * - $navigation: Items for the navigation bar.
 * - $sidebar_first: Items for the first sidebar.
 * - $sidebar_second: Items for the second sidebar.
 * - $header: Items for the header region.
 * - $footer: Items for the footer region.
 * - $page_closure: Items to appear below the footer.
 *
 * The following variables are deprecated and will be removed in Drupal 7:
 * - $body_classes: This variable has been renamed $classes in Drupal 7.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see zen_preprocess()
 * @see zen_process()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>">


<!-- wrapper -->
	<div id="wrapper">
    <!--  holder -->
		<div class="holder">
  
  <?php 
  
  if ($content_top): ?>
          <div class="content_top"><?php // print "<pre>".$content_top."</pre>"; ?></div>
         
   <?php endif; ?>
           
              
        <?php print $page_header; ?>

<?php if ($messages) print $messages;?>
 <?php //print $highlight; ?>
        <?php //print $breadcrumb; ?>
        
        <?php //print $messages; ?>
        <?php if ($tabs): ?>
          <div class="tabs"><?php //print $tabs; ?></div>
        <?php endif; ?>
        <?php //print $help; ?>
        
        
        
			<!-- main -->
		<div id="main">
        
        <?php if ($title): ?>
        <div class="main-heading">
          <h1 class="title"><?php print $title; ?></h1>
          </div>
        <?php endif; ?>
        
        <div class="main-section">
        
        <div id="equipo_intro_text">
        
       <?php print $node->body; ?> 
       
</div>
        
   
   
   <div class="post-columns">
<div class="grid-div">
<ul class="grid-list size">

<h2 class="secc_equipo">FUNDADORES</h2>
<li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //RAFA
$nid = 155;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Socio fundador, CEO<br />
Desarrollo de negocio
</div>
  </li>
  

<li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //jUAN
$nid = 359;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Socio fundador<br />
Comunidad coworkers
</div>
  </li>

<h2 class="secc_equipo">EQUIPO</h2>

<li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //yago
$nid = 261;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Gestión de espacios y eventos
</div>
  </li>
  
 
 

<li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //olivia
$nid = 257;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Comunicación y proyectos
</div>
  </li>
  
 
 
 <li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //aurea
$nid = 149;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Responsable utopic_US virtual
</div>
  </li>
  
  
 
  <li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //susana
$nid = 581;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Administración
</div>
  </li>
  
  
    <li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //santi
$nid = 147;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
 Contenido audiovisual
</div>
  </li>
  
 
   <li class="views-row views-row-1 views-row-odd views-row-first">     
<?php //lorena
$nid = 316;
$node = node_load($nid);
$node_tpl_output = node_view($node,TRUE);
print $node_tpl_output; //
// node_load(155)
?> 
<div class="cargo_equipo">
Programación cultural
</div>
  </li>
  
    
  </ul>
  
  </div>
  </div>      
       
        </div>
        
        
        
		</div> 
        <!-- // main -->
        
		</div>
        <!-- // holder -->
	</div>
	<!-- // wrapper -->
    
 <?php print $page_footer; ?> 

  <?php  print $page_closure; ?>

  <?php print $closure; ?>

</body>
</html>
