<?php
// [...]
echo "Menu rebuild";
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

/*function user_is_editor(){
global $user;
$adminRoles= array('administrator','editor','redactor','diseñador');
$check = array_intersect($adminRoles, array_values($user->roles));
$adminAble = empty($check) ? FALSE : TRUE;
return $adminAble;
}*/


drupal_flush_all_caches();

menu_rebuild(); // use just once
die();          // use just once

$return = menu_execute_active_handler();
// [...]
?>
