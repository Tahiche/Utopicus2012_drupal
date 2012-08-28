<?php 

//echo "<h1>Estoy incluido</h1>";

function user_is_admin(){
global $user;
$adminRoles= array('administrator');
//krumo(  $user);
$check = array_intersect($adminRoles, array_values($user->roles));
$adminAble = empty($check) ? FALSE : TRUE;
return $adminAble;
}
	
function user_is_editor(){
global $user;
$adminRoles= array('administrator','editor','redactor','diseñador');
$check = array_intersect($adminRoles, array_values($user->roles));
$adminAble = empty($check) ? FALSE : TRUE;
return $adminAble;
}
	
function miKrumo($var){
	if(!user_is_admin()) return false;
		
		if (!function_exists('krumo')){
	 include_once  drupal_get_path('module', 'devel'). '/krumo/class.krumo.php';
			}
	echo "<div id='krumoDiv'>	";	
	krumo($var);
	echo"</div>";
}


function _taxonomy_get_real_tree($vid){
  $result_tree = array();
  $terms = array();
  foreach(taxonomy_get_tree($vid) as $term){
    if(isset($terms[$term->tid])){
      $term->children = $terms[$term->tid]->children;
      $terms[$term->tid] = $term;
    }else{
      $terms[$term->tid] = $term;
    }

    if($term->depth === 0){
      $result_tree[$term->tid] = &$terms[$term->tid];
      continue;
    }

    foreach($term->parents as $tid){
      if($tid){
        if(!isset($terms[$tid])){
          $terms[$tid] = new stdClass();
        }
        $terms[$tid]->children[$term->tid] = &$terms[$term->tid];
      }
    }
  }
  return $result_tree;
}
?>