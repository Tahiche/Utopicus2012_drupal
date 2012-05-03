<?php
function utopicadmin_preprocess_page(&$vars, $hook) {


  // Add body class for theme.
  $vars['attr']['class'] .= ' utopicadmin';
  
  
  if(isset($_GET['krumo'])){
	krumo($vars);
  }
  
  }