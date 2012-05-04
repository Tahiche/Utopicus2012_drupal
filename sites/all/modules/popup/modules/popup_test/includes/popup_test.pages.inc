<?php



/**
 * Popup test position page
 */
function popup_test_position(){

  module_load_include('inc', 'popup', 'includes/popup.api');

  $rendered = '';

  $corner = array(
    'top left' => 'top-left',
    'top right' => 'top-right',
    'bottom left' => 'bottom-left',
    'bottom right' => 'bottom-right',
  );

  foreach ($corner as $origin_label => $origin_setting){
    foreach($corner as $expand_label => $expand_setting){
      $rendered .=
        'This is the <strong>' . $origin_label . '</strong> origin test: ' .
        popup_element('popup title', 'Expanding to the ' . $expand_label, array('origin' => $origin_setting, 'expand' => $expand_setting)) .
        ' and some suffix text.<br />';
      ;
    }
  }

  return $rendered;

}



/**
 * Popup test effects page
 */
function popup_test_effects(){

  module_load_include('inc', 'popup', 'includes/popup.api');

  $rendered = '';

  $effects = module_invoke_all('popup_effects');

  foreach($effects['show'] as $name=>$effect){
    $rendered .= popup_element('Effect: ' . $name, 'Test popup text', array('effect' => $name, 'id' => 'class-effect-'.$name, 'class' => 'class-effect-'.$name));
  }

  return $rendered;

}



/**
 * Popup test nesting page
 */
function popup_test_nesting(){

  module_load_include('inc', 'popup', 'includes/popup.api');

  $rendered = '';

  foreach(array('bottom-left', 'bottom-right') as $expand){
    $popup = 'LAST!';
    for($i = 5; $i>0; $i--){
      $popup = popup_element('Level ' . $i, $popup, array('effect' => $name, 'expand' => $expand));
    }
    $rendered .= $popup;
  }

  return $rendered;

}



/**
 * Popup test activation page
 */
function popup_test_activation(){

  module_load_include('inc', 'popup', 'includes/popup.api');

  $rendered =
    popup_element('Hover', 'This is a popup that activates on hover', array('activate' => 'hover')) .
    popup_element('Click', 'This is a popup that must be clicked to activate', array('activate' => 'click')) .
    popup_element('Click and Close', 'This is a popup that must be clicked to activate, and can be closed with a button', array('activate' => 'click', 'close' => TRUE));

  return $rendered;

}



/**
 * Popup test activation page
 */
function popup_test_styles(){

  module_load_include('inc', 'popup', 'includes/popup.util');
  module_load_include('inc', 'popup', 'includes/popup.api');

  $styles = _popup_styles();

  foreach($styles as $style=>$path){
    $rendered .= popup_element($style, 'This is a test of the ' . $style . ' popup style', array('style' => $style, 'activate' => 'hover'));
  }

  return $rendered;

}



/**
 * Popup test content page
 */
function popup_test_content(){

  module_load_include('inc', 'popup', 'includes/popup.api');
  $firstnode = db_result(db_query("SELECT nid FROM {node} LIMIT 1"));

  $rendered =
    'Text: Some text ' . popup(array('text' => 'This is popup text', 'activate' => 'click')) . ' and some more text.<br />' .
    'Node: Some text ' . popup(array('node' => $firstnode, 'activate' => 'click', 'width' => 450)) . ' and some more text.<br />' .
    'Block: Some text ' . popup(array('block' => TRUE, 'module' => 'user', 'delta' => 3, 'activate' => 'click')) . ' and some more text.<br />' .

    'Form: Some text ' . (
      module_exists('search') ?  popup(array('form' => 'search_form', 'activate' => 'click', 'width' => 450)) : '[Search module not enabled, skipping form test.]'
    ) . ' and some more text.<br />'.

    'View: Some text ' . (
      module_exists('views') ?  popup(array('view' => 'tracker', 'activate' => 'click', 'width' => 500)) : '[Views module not enabled, skipping view test.]'
    ) . ' and some more text.<br />' .

    'PHP: Some text ' . popup(array('php' => 'return "Some php generated text: ' . date('d-M-Y', time()) . ' ";', 'activate' => 'click')) . ' and some more text.<br />'.

    'Menu: Some text ' . popup(array('menu' => 'navigation', 'origin' => 'top-right',  'expand' => 'bottom-right')) . ' and some more text.<br />';

  ;

  return $rendered;

}



/**
 * Popup test AJAX content page
 */
function popup_test_ajax(){

  module_load_include('inc', 'popup', 'includes/popup.api');
  $firstnode = db_result(db_query("SELECT nid FROM {node} LIMIT 1"));

  $rendered =
    'Node: Some text ' . popup(array('node' => $firstnode, 'activate' => 'click', 'ajax' => 1, 'width' => 450)) . ' and some more text.<br />' .
    'Block: Some text ' . popup(array('block' => TRUE, 'module' => 'user', 'delta' => 3, 'activate' => 'click', 'ajax' => 1)) . ' and some more text.<br />' .

    'Form: Some text ' . (
      module_exists('search') ?  popup(array('form' => 'search_form', 'activate' => 'click', 'ajax' => 1, 'width' => 350)) : '[Search module not enabled, skipping form test.]'
    ) . ' and some more text.<br />'.

    'View: Some text ' . (
      module_exists('views') ?  popup(array('view' => 'tracker', 'activate' => 'click', 'ajax' => 1, 'width' => 500)) : '[Views module not enabled, skipping view test.]'
    ) . ' and some more text.<br />' .

    'PHP: Some text ' . popup(array('php' => 'return "Some php generated text: ' . date('d-M-Y', time()) . ' ";', 'activate' => 'click', 'ajax' => 1)) . ' and some more text.<br />';

  return $rendered;

}
