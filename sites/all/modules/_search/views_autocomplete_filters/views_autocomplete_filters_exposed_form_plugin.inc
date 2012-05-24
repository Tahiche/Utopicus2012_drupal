<?php

/**
 * @file
 * Provides an Views Autocomplete Filters exposed form plugin for View 3.x.
 */
class views_autocomplete_filters_exposed_form_plugin extends views_plugin_exposed_form_basic {

  function summary_title() {
    return t('Views Autocomplete Filters');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['views_autocomplete'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array();

    // Go through each filter and add the same options we used to add in hook_form_alter()
    foreach ($this->display->handler->get_handlers('filter') as $label => $filter) {
      if (!$filter->options['exposed']) {
        continue;
      }
      if (!$options_header) {
        $options['header'] = array(
          '#value' => '<strong>'. t("Autocomplete Filters") .'</strong>',
          );
      }
      $options[$label]['autocomplete_enabled'] = array(
        '#title' => t("Enable autocomplete for "). $label,
        '#type' => 'checkbox',
        '#default_value' => $this->options['views_autocomplete'][$label]['autocomplete_enabled'],

        );
    }
    $form['views_autocomplete'] = $options;
  }

  function exposed_form_alter(&$form, &$form_state) {
    parent::exposed_form_alter($form, $form_state);
    $filters = $form_state['view']->display_handler->handlers['filter'];
    foreach ($filters as $label => $filter) {
      if ($filter->options['exposed'] && $this->options['views_autocomplete'][$label]['autocomplete_enabled']) {
        $form[$label]['#autocomplete_path'] = 'autocomplete_filter/'. $label .'/'. $this->view->name .'/'. $this->view->current_display;
      }
    }
  }
}
