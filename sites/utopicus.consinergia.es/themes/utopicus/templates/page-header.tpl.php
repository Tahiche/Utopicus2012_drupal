<?php
/**
 * @file
 * Default theme implementation to display a region.
 *
 * Available variables:
 * - $content: The content for this region, typically blocks.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - region: The current template type, i.e., "theming hook".
 *   - region-[name]: The name of the region with underscores replaced with
 *     dashes. For example, the page_top region would have a region-page-top class.
 * - $region: The name of the region variable as defined in the theme's .info file.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see zen_preprocess_region()
 * @see zen_process()
 */
//krumo(__FILE__);
global $base_root;
if (empty($base_root)){
	$base_root="http://www.utopicus.consinergia.es";
}

global $theme_pathabsoluto;
if (empty($theme_pathabsoluto)){
	$theme_pathabsoluto= $base_root."/sites/utopicus.consinergia.es/themes/utopicus";
}
?>
			<!-- header -->
			<div id="header">
				<!-- topbar -->
				<div class="topbar">
					<div class="alignright">
						<!-- usermenu -->
						<ul class="usermenu">
							<li class="active"><a href="#">OPEN_US</a></li>
							<li><a href="#">CONTACTO</a></li>
							<li><a href="#">AYUDA</a></li>
							<li><a href="#">ENGLISH</a></li>
						</ul>
						<!-- loginmenu -->
						<ul class="loginmenu">
							<li><a href="/user">ÚNETE </a></li>
							<li><a href="/user">ACCEDER</a></li>
						</ul>
					</div>
				</div>
                <!-- // topbar -->
				<!-- header-holder -->
				<div class="header-holder">
					<!-- logo -->
					<strong class="logo"><a href="<?php print $base_root; ?>" title="<?php print 'utopic_US Home'; ?>"  alt="<?php print 'utopic_US Home'; ?>"  rel="home">Utopic_US </a></strong>
					<!-- nav -->
<?php
//primary-menu.tpl theme hook 
// print theme('header_menu');
include_once 'menus/primary-menu.tpl.php';
?>
				</div>
                <!-- // header-holder -->
			</div>
            <!-- // header -->
            <?php 
			//krumo($variables); 
			
			$tabs=$variables['page_vars']['tabs'];
			$tabs2=$variables['page_vars']['tabs2'];
			?>
            <?php if ($tabs ): ?>
      <div id="drupal-control-bar">      
      <?php endif; ?>
        <?php if ($tabs): print '<ul>' . $tabs . '</ul>'; endif; ?>    
        <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
      <?php if ($tabs): ?>        
      </div><!--/ #drupal-control-bar -->
      <?php endif; ?>
