<?php
global $base_root;
if (empty($base_root)){
    $base_root="http://www.utopicus.es";
}

global $theme_pathabsoluto;
if (empty($theme_pathabsoluto)){
    $theme_pathabsoluto= $base_root."/sites/utopicus.es/themes/utopicus";
}

global $isloggedin;
if (!isset($isloggedin)){
    $isloggedin= FALSE;
}
global $user;
if(!empty($user->uid)) $isloggedin= TRUE;

global $loginurl;
if (!isset($loginurl)) $loginurl = $base_root . '/user';

global $logouturl;
if (!isset($logouturl)) $logouturl = $base_root . '/logout';


// $homelink=($variables['page_vars']['front_page']?$variables['page_vars']['front_page']:"/");
$homelink="/";
?>

            <!-- header -->
            <div id="header">
                <!-- topbar -->
                <div class="topbar">
                    <div class="alignright">
                        <!-- usermenu -->
                        <ul class="usermenu">
                            <li class="active"><a href="/es/espacios/open_us">OPEN_US</a></li>
                            <li><a href="#">CONTACTO</a></li>
                            <!--<li><a href="#">AYUDA</a></li>
                             <li><a href="#">ENGLISH</a></li> -->
                        </ul>
                        <!-- loginmenu -->
                        <ul class="loginmenu">
                        <?php if(!$isloggedin): ?>
                            <!-- <li><a href="/user">ÚNETE</a></li> -->
                            <li ><a id="toboggan-login-link" href="<?php echo $loginurl; ?>">LOG-IN</a></li>


                         <?php else: ?>
                            <!--<li><a href="/user">ÚNETE </a></li> -->
                            <li ><a id="toboggan-login-link" href="<?php echo $logouturl; ?>">LOGOUT</a></li>

                         <?php endif; ?>
                        </ul>
                    </div>
                </div>

                            <?php
//primary-menu.tpl theme hook
// print theme('header_menu');
include_once 'commonforms/login.inc.php';
?>


                <!-- // topbar -->
                <!-- header-holder -->
                <div class="header-holder">
                    <!-- logo -->
                    <strong class="logo"><a href="<?php print $homelink; ?>" title="<?php print 'utopic_US Home'; ?>"  alt="<?php print 'utopic_US Home'; ?>"  rel="home">Utopic_US </a></strong>
                    <!-- nav -->
<?php
//primary-menu.tpl theme hook
// print theme('header_menu');
include_once 'menus/primary-menu-moodle.tpl.php';
?>
                </div>
                <!-- // header-holder -->
            </div>
            <!-- // header -->



            <?php
            //krumo($variables);

            //$tabs=$variables['page_vars']['tabs'];
            //$tabs2=$variables['page_vars']['tabs2'];
            $tabs=null;
            $tabs2=null;
            ?>
            <?php if ($tabs ): ?>
      <div id="drupal-control-bar">
      <?php endif; ?>
        <?php if ($tabs): print '<ul>' . $tabs . '</ul>'; endif; ?>
        <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
      <?php if ($tabs): ?>
      </div><!--/ #drupal-control-bar -->
      <?php endif; ?>
