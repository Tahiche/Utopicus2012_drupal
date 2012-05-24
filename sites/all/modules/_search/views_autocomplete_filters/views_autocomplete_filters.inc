<?php
// $Id: views_autocomplete_filters.module,v 1 vasike Exp $

/**
 * @file
 * Common functions for Autocomplete Filters module.
 */
/**
 * Menu callback; Retrieve a pipe delimited string of autocomplete suggestions.
 */
function views_autocomplete_filter($field_name = NULL, $view_name, $view_display, $string = '') {
 static $autocomplete_filter_results;
 $results_id = $field_name.'_'.$view_name.'_'.$view_display.'_'.$string;
 if (isset($autocomplete_filter_results[$results_id])) {
	  drupal_json($autocomplete_filter_results[$results_id]);
		return;
 }
 $matches = array();
 //$view_args = array();
 $view = views_get_view($view_name);
 $view->set_display($view_display);
 // force to have no limit for results
 $view->display_handler->set_option('items_per_page', 0);
 $view->handler->items_per_page = $view->pager['items_per_page'] = 0;
 $currentview = $view->display[$view_display];

 //Remove other exposed filters because they will cause problems
 $new_views_filters = $view->display_handler->get_option('filters');
 foreach ($new_views_filters as $field => $definition) { 
   if ($field != $field_name && $definition['exposed']) {
    unset($new_views_filters[$field]);  
   }
 }
 $view->display_handler->override_option('filters', $new_views_filters);

 // exposed form field name
 if ($currentview->handler->options['defaults']['filters']) {
	 $currentview = $view->display['default'];
 } else {
	 $currentview = $view->display[$view_display];
 }
 $field_filter = $currentview->handler->options['filters'][$field_name];
 // assign string value to the exposed text filter
 $view->exposed_input[$field_filter['expose']['identifier']] = $string;

 $item = $view->get_item($view_display, 'filter', $field_name);
 // overwrite the fields for the view
 $view->display[$view_display]->handler->options['fields'] = array($field_name => $item);
 $view->display[$view_display]->display_options['fields'] = array($field_name => $item);
 $view->execute($view->current_display);
 
 $currentview = $view->display[$view_display];
 // views field handler data
 $field = $currentview->handler->handlers['field'][$field_name];
 $field_alias = $field->field_alias;

 foreach ($view->result as $id => $row) {
   // Add a class wrapper for a few required CSS overrides.
	 if (!empty($row->$field_alias)) {
	    $matches[$row->$field_alias] = '<div class="reference-autocomplete">'. $row->$field_alias .'</div>';
	 }
 }
 if (!empty($matches)) {
	 $autocomplete_filter_results[$results_id] = $matches;
 } elseif (!empty($view->result)) {
	 $matches[''] = '<div class="reference-autocomplete">'. t('The %field field of this filter should be present also in the result (could be excluded from display).', array('%field' => $field_filter['expose']['label'])) . '</div>';
 } else {
	 $matches[''] = '<div class="reference-autocomplete">'. t('The %string return no results. Please try something else.', array('%string' => $string)) . '</div>';
 }
 drupal_json($matches);
}
