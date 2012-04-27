<?php
/**
 * @file
 *  Defines the load more pager plugin for Views.
 */

/**
 * The plugin to handle full pager.
 *
 * @ingroup views_pager_plugins
 */
class views_plugin_pager_load_more extends views_plugin_pager_full {
  // Identifies changes to the exposed items per page value.
  private $exposed_limit_changed = FALSE;

  function summary_title() {
    if (!empty($this->options['offset'])) {
      return format_plural($this->options['items_per_page'], 'More pager, @count item, skip @skip', 'More pager, @count items, skip @skip', array('@count' => $this->options['items_per_page'], '@skip' => $this->options['offset']));
    }
      return format_plural($this->options['items_per_page'], 'More pager, @count item', 'More pager, @count items', array('@count' => $this->options['items_per_page']));
  }

  /**
   * Describes the options available from this Views plugin.
   */
  function option_definition() {
    // @TODO: Add waypoint support.
    $options = parent::option_definition();
    $options['load_more'] = array();
    $options['load_more']['link_text'] = array(
      'default' => 'Load More',
      'translatable' => TRUE,
    );
    $options['load_more']['pager_class'] = array(
      'default' => '',
      'translatable' => FALSE,
    );
    $options['load_more']['items_first_page'] = array(
      'default' => $this->options['items_per_page'],
    );
    return $options;
  }

  /**
   * Validates options set by the user.
   */
  function options_validate(&$form, &$form_state) {
    // Validate the "Load More" anchor text, if any is set.
    if (!empty($form_state['values']['pager_options']['load_more']['link_text'])) {
      // Obtain an HTML-free version of the link text.
      $link_text = filter_xss($form_state['values']['pager_options']['load_more']['link_text'], array());
      // If the cleaned link text doesn't match its source, HTML may exist.
      if (!$link_text || $link_text != $form_state['values']['pager_options']['load_more']['link_text']) {
        form_set_error('pager_options][load_more][link_text', t('The "Load More" anchor text should not contain HTML.'));
      }
    }
    // Validate the 'first page' result count.
    if (!empty($form_state['values']['pager_options']['load_more']['items_first_page'])) {
      // Ensure the user has entered a whole number.
      if (!is_numeric($form_state['values']['pager_options']['load_more']['items_first_page'])) {
        form_set_error('pager_options][load_more][items_first_page', t('Please enter a number.'));
      }
      // Prevent the user from entering a negative number.
      elseif ($form_state['values']['pager_options']['load_more']['items_first_page'] <= 0 || !is_int((int) $form_state['values']['pager_options']['load_more']['items_first_page'])) {
        form_set_error('pager_options][load_more][items_first_page', t('Please enter a whole number greater than 0.'));
      }
    }
    // Check if a link class was supplied.
    if (!empty($form_state['values']['pager_options']['load_more']['pager_class'])) {
      // Obtain an HTML-free version of the link class.
      $pager_class = filter_xss($form_state['values']['pager_options']['load_more']['pager_class'], array());
      // If the cleaned link class doesn't match its source, HTML may exist.
      if (!$pager_class || $pager_class != $form_state['values']['pager_options']['load_more']['pager_class']) {
        form_set_error('pager_options][load_more][link_text', t('The "Load More" pager class should not contain HTML.'));
      }
    }
  }

  /**
   * Provides a form for setting options for this plugin.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['load_more'] = array(
      '#tree' => TRUE,
      '#weight' => -10,
    );
    // Give the user an option to change the anchor text.
    $form['load_more']['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('"Load More" Link Text'),
      '#description' => t('The link title that will be used for the "Load More" anchor. HTML is not permitted.'),
      '#default_value' => $this->options['load_more']['link_text'],
    );
    // Give the user an option to change the pager class.
    $form['load_more']['pager_class'] = array(
      '#type' => 'textfield',
      '#title' => t('"Load More" Pager class'),
      '#description' => t('Sets a custom class name on the "Load More" pager wrapper. HTML is not permitted.'),
      '#default_value' => $this->options['load_more']['pager_class'],
    );
    // Give the user an option to change the number of results on the first page.
    $form['load_more']['items_first_page'] = array(
      '#type' => 'textfield',
      '#title' => t('"Load More" Initial Count'),
      '#description' => t('The number of items to display on the first page. Subsequent pages will use the "items per page" setting.'),
      '#default_value' => $this->options['load_more']['items_first_page'],
    );
    // I'm not sure why someone might combine the exposed limit filter and a views_load_more pager, but it could happen.
    // @todo: If exposed limits were possible, how do we need to approach them?
    /***
    $form['expose']['items_per_page']['#disabled'] = TRUE;
    $form['expose']['items_per_page']['#default_value'] = 0;
    $form['expose']['items_per_page']['#description'] = t('The exposed Items per page settings are not compatible with a "Load More" pager.');
    $form['expose']['items_per_page_options_all']['#access'] = FALSE;
    $form['expose']['items_per_page_options_all']['#default_value'] = 0;
    $form['expose']['items_per_page_options_all_label']['#access'] = FALSE;
    /***/
  }

  /**
   * Modify the query.
   */
  function query() {
    // Let the full pager plugin process offsets, limits and pagination.
    parent::query();

    // Check if we want to display a different number of items on the first page of results.
    if (!empty($this->options['load_more']['items_first_page'])) {
      // Set the limit for the first page.
      if ($this->current_page == 0) {
        // Adjust the maximum number of items we'll load.
        $this->view->query->set_limit($this->options['load_more']['items_first_page']);
      }
      // Subsequent pages will need to be offset.
      // The offset should be the number of items displayed on the first page.
      // Otherwise, we'll see the same results twice.
      else {
        // Ensure the current offset is known. If this value is empty or null, we need to force it to 0.
        $current_offset = (int) $this->view->query->offset;
        // Adjust the offset to account for our first page.
        $this->view->query->set_offset($current_offset + $this->options['load_more']['items_first_page']);
      }
    }
  }

  /**
   * Processes this plugin for output.
   */
  function render($input) {
    // Find all the theme hook suggestions for the pager.
    $pager_theme = views_theme_functions('views_load_more_pager', $this->view, $this->display);
    // Collect some information about the view.
    // This will be passed into Drupal.settings.views_load_more
    // so javascript can act on the view.
    $settings = array(
      'css_class' => !empty($this->display->display_options['css_class']) ? $this->_build_classname($this->display->display_options['css_class']) : '.view-content',
      'row_class' => !empty($this->display->display_options['style_options']['row_class']) ? $this->_build_classname($this->display->display_options['style_options']['row_class']) : '.views-row',
      'pager_class' => !empty($this->options['load_more']['pager_class']) ? $this->_build_classname($this->options['load_more']['pager_class']) : '.load-more-pager',
      'style_plugin' => $this->view->plugin_name,
      'total_items' => (int) $this->total_items,
    );

    // Get the number of items per page.
    $items_per_page = $this->options['items_per_page'];

    // If the number of items per page is different for the first page,
    // Adjust the pager's settings appropriately.
    if ($this->current_page == 0 && !empty($this->options['load_more']['items_first_page'])) {
      $items_per_page = $this->options['load_more']['items_first_page'];
    }
    return theme($pager_theme, $this->options['load_more']['link_text'], $items_per_page, $this->options['id'], $input, 9, $settings);
  }

  /**
   * Builds the classname selectors provided by options.
   * @param $class
   *  The classname string provided by an option.
   * @return
   *  Creates a css selector string out of the classname option provided.
   * @retval string
   */
  function _build_classname($class) {
    if (empty($class)) {
      return '';
    }
    $classnames = explode(' ', $class);
    return implode('.', $classnames);
  }
}

