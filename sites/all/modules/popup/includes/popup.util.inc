<?php



/**
 *  hook_theme helper function to generate a theme array for a specific style
 */
function _popup_theme_array($style = FALSE, $path = FALSE){

  $suffix = $style ? '-' . $style : '';
  $theme = array(

    'popup-element' . $suffix => array(
      'arguments' => array(
        'title' => '',
        'body' => '',
        'css_id' => '',
        'class' => '',
        'style' => '',
      ),
      'template' => 'popup-element',
      'preprocess functions' => array(
        'popup_preprocess_popup_element',
      ),
    ),

    'popup-element-body' . $suffix => array(
      'arguments' => array(
        'body' => '',
        'class' => '',
        'close' => '',
      ),
      'template' => 'popup-element-body',
    ),

    'popup-element-title' . $suffix => array(
      'arguments' => array(
        'title' => '',
        'class' => '',
        'href' => '',
      ),
      'template' => 'popup-element-title',
    ),

  );

  if ($path){
    foreach($theme as &$element){
      $element['path'] = $path;
    }
  }

  return $theme;
}



/**
 *  Returns all popup styles provided by all modules
 */
function _popup_styles(){

  static $all_styles = FALSE;
  if ($all_styles){
    return $all_styles;
  }

  $all_styles = module_invoke_all('popup_styles');
  ksort($all_styles);

  return $all_styles;
}



/**
 *  Returns all popup effects provided by all modules
 */
function _popup_effects(){

  static $all_effects = FALSE;
  if ($all_effects){
    return $all_effects;
  }

  $all_effects = module_invoke_all('popup_effects');
  ksort($all_effects);

  return $all_effects;
}



/**
 * Converts a machine readable key into a title
 */
function _popup_key_to_title($key){
	return ucwords(preg_replace('/[^a-zA-Z]/', ' ', $key));
}



/**
 * Converts a title to a machine readable key
 */
function _popup_title_to_key($title){
	return strtolower(preg_replace('/[^a-zA-Z0-9]/', '_', $title));
}



/**
 * Generates the correct classes for each of the popup element, title and body.
 */
function _popup_classes($attributes = array()){

  // Defaults
  _popup_default($attributes['activate'], 'hover');
  _popup_default($attributes['effect'], 'default');
  _popup_default($attributes['origin'], 'bottom-left');
  _popup_default($attributes['expand'], 'bottom-right');
  _popup_default($attributes['width'], variable_get('popup-width', 150));

  $classes = array(
    'element' => array('popup-element', 'popup-element-noscript',),
    'title' => array('popup-element-title'),
    'body' => array('popup-element-body'),
  );

  // Add style specific class
  if ($attributes['style-class']){
    $classes['element'][] = 'popup-style-' . $attributes['style-class'];
  }

  // Add element specific classes
  foreach($classes as $element => $class_array){
    if ($attributes[$element . '-class']){
      $classes[$element][] = $attributes[$element . '-class'];
    }
  }

  // Add additional classes if needed
  if (is_array($attributes['classes'])){
    foreach($attributes['classes'] as $element=>$add_classes){
      $classes[$element] = array_merge(
        $attributes['classes'][$element],
        $classes[$element]
      );
    }
  }

  // Define which attribute classes apply to which sub-element
  $meta = array(
    'activate' => array('element'),
    'ajax' => array('body'),
    'class' => array('title', 'element', 'body'),
    'effect' => array('element'),
    'expand' => array('element', 'body'),
    'opacity' => array('body'),
    'origin' => array('element', 'body'),
    'width' => array('body'),
  );
  foreach($meta as $attribute => $apply_to){
    if ($attributes[$attribute]) {
      foreach($apply_to as $element){
        $classes[$element][] = $attributes[$attribute] == 1
          ? $attribute
          : $attribute . '-' . $attributes[$attribute];
      }
    }
  }

  foreach($classes as $element=>$element_classes){
    $classes[$element] = implode(' ', $element_classes);
  }

  return $classes;
}



/**
 * Sets a default value on a variable if it is not set
 */
function _popup_default(&$attribute, $default){
  if (!$attribute) {
    $attribute = $default;
  }
}



/**
 * Generates a unique id for a popup element.
 */
function _popup_id($id = FALSE){

  static $ids = array();
  static $popup_index = 0;

  if ($id){

    $offset = 0;
    $offset_id = $id . '-' . $offset;

    while ($ids[$offset_id]){
      $offset_id = $id . '-' . $offset++;
    }
    $ids[$offset_id] = TRUE;

    return $offset_id;
  }

  return 'popup-element-'.$popup_index++;
}



/**
 *  Recursively generates popup menus
 */
function _popup_menuelement($menu, $attributes, $level = 0){

  global $user;

  $body = '';
  $title = $attributes['title'] ? $attributes['title'] : $menu['link']['title'];
  $link = drupal_get_path_alias(
    str_replace('%', $user->uid, $menu['link']['link_path'])
  );

  if ($menu['link']['has_children'] && is_array($menu['below'])){

    $children = _popup_menuelement_visible_items($menu['below']);
    $child_attributes = _popup_menuelement_child_attribs($attributes);

    if ($attributes['flat']){
      $child_attributes['expand'] = $attributes['expand'];
      $child_attributes['origin'] = $attributes['origin'];
    }

    foreach($children as $child){
      $body .= _popup_menuelement(
        $child,
        $child_attributes,
        ($attributes['flat'] ? $level : $level + 1)
      );
    }

    $attributes['classes']['body'][] = 'popup-menu-branch-body-' . _popup_title_to_key($title);
    $attributes['classes']['element'][] = 'popup-menu-branch-element-' . _popup_title_to_key($title);
    $attributes['classes']['element'][] = 'popup-menu-branch';
    $attributes['classes']['title'][] = 'popup-menu-branch-title';
    $attributes['classes']['title'][] = 'popup-menu-item';
    $attributes['classes']['title'][] = 'popup-menu-item-' . _popup_title_to_key($title);

    $attributes['link'] = $link;

    if ($level > 0){
      $attributes['classes']['element'][] = 'popup-menu-child';
    }

    return $attributes['flat']
      ? '<div class="popup-menu-flat' .
        ($attributes['inline'] ? ' popup-menu-inline' : ' popup-menu-block') .
        ' popup-style-' . _popup_title_to_key($attributes['style']).
        '">' . $body . '</div>'
      : popup_element($title, $body, $attributes);

  } else {

    return l('<span>' . $title . '</span>', $link,
      array(
        'html' => TRUE,
        'attributes' => array(
          'class' =>
            'popup-menu-item popup-menu-leaf-title' .
            ($attributes['activate'] == 'hover' ? ' activate-hover' : '') .
            ' popup-menu-item-' . _popup_title_to_key($title)
          )
      )
    );
  }
}



/**
 * Generate popup position classes 
 */
function _popup_menuelement_position_classes($index, $count){

  $pos_class = array();
  if ($index == 0) {
    $pos_class[] = 'first';
  }
  if ($index == $count - 1) {
    $pos_class[] = 'last';
  }

  return implode(' ', $pos_class);
}



/**
 * Filters menu items and returns only visible items
 */
function _popup_menuelement_visible_items($tree){

  return array_filter(
    $tree,
    '_popup_menu_visible'
  );

}



/**
 * Determines whether a menu item is visible
 */
function _popup_menu_visible($item){
  return !$item['link']['hidden'];
}



/**
 * Filters menu attributes to return only attributes that apply to children
 */
function _popup_menuelement_child_attribs($attributes){

  $child_attributes = array(
    'activate' => $attributes['activate'],
    'effect' => $attributes['effect'],
    'expand' => $attributes['submenu-expand'],
    'style' => $attributes['style'],
    'submenu-expand' => $attributes['submenu-expand'],
    'submenu-origin' => $attributes['submenu-origin'],
    'opacity' => $attributes['opacity'],
    'origin' => $attributes['submenu-origin'],
    'width' => $attributes['width'],
  );

  if ($attributes['flat']){
    $child_attributes['expand'] = $attributes['expand'];
    $child_attributes['origin'] = $attributes['origin'];
  }

  return $child_attributes;
}



/* ---- AJAX callbacks ---- */



/**
 * Fetches dynamic popup content for an ajax call
 */
function popup_get_ahah($type, $attr_hash = ''){

  module_load_include('inc', 'popup', 'includes/popup.api');

  $attributes = _popup_cache_attributes($attr_hash);

  $function = '_popup_' . $type;

  print $function($attributes, 'body');
  exit;

}



/* ---- AJAX caching helper ---- */



/**
 * Caches popup attributes for AJAX popups
 */
function _popup_cache_attributes($cache){

  $attrib_cache = FALSE;
  if (!$attrib_cache){
    $attrib_cache = variable_get('popup_ajax_cache', array());
  }

  if (is_array($cache)){

    $hash = md5(serialize($cache));
    $attrib_cache[$hash] = $cache;

    variable_set('popup_ajax_cache', $attrib_cache);

    return $hash;

  } else {
    return $attrib_cache[$cache];
  }
}


