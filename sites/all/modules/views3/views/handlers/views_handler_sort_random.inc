<?php

/**
 * Handle a random sort.
 *
 * @ingroup views_sort_handlers
 */
 
 /****************** patch ***************************/
 // http://drupal.org/files/issues/views_handler_sort_random_patch_0.patch
 // http://drupal.org/node/996168
class views_handler_sort_random extends views_handler_sort {
  function query() {
	     // the idea is to add integer argument to MySQL RAND() function, which will remain
   // among all the pages for the same view, going back to the first page (no $_REQUEST['page'])
    // will initialize and randomize entire view
    if (!$_SESSION['views_random_sort_int'] || !$_REQUEST['page']) {
      $_SESSION['views_random_sort_int'] = rand(1, 1001);
    }
    global $db_type;
    switch ($db_type) {
      case 'mysql':
      case 'mysqli':
       // $formula = 'RAND()';
		$formula = 'RAND('. $_SESSION['views_random_sort_int'] . ')';
        break;
      case 'pgsql':
        $formula = 'RANDOM()';
        break;
    }
    if (!empty($formula)) {
      //$this->query->add_orderby(NULL, $formula, $this->options['order'], '_' . $this->field);
	  $this->query->add_orderby(NULL, NULL, '', $formula);
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['order']['#access'] = FALSE;
  }
}
