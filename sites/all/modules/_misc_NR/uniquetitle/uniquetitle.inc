<?php

/**
 * @file
 * Administration and checking functions
 */

/**
 * FAPI form builder callback
 */
function uniquetitle_settings_form($form_state) {
  $node_types = node_get_types();
  $settings = uniquetitle_get_settings();

  $form = array();

  $form['uniquetitle_settings'] = array(
    '#tree' => TRUE,
  );

  $form['uniquetitle_settings']['uniqueinfo'] = array(
    '#tree'        => TRUE,
    '#type'        => 'fieldset',
    '#title'       => t('Unique settings for content types'),
    '#description' => t('If the <em>"Unique"</em> checkbox unchecked, the <em>Casesensitive</em> checkbox will be ignored.'),
  );

  foreach ($node_types as $type => $node_type) {
    $form['uniquetitle_settings']['uniqueinfo'][$type] = array(
      '#type'    => 'checkboxes',
      '#title'   => $node_type->name,
      '#options' => array(
        'unique'        => t('Unique'),
        'casesensitive' => t('Casesensitive')
      ),
      '#default_value' => array_keys($settings['uniqueinfo'][$type], TRUE),
    );
  }

  return system_settings_form($form);
}

/**
 * FAPI form #validate callback
 * @see uniquetitle_settings_form()
 */
function uniquetitle_settings_form_validate($form, &$form_state) {
  $items = array();
  $node_types = node_get_types();
  foreach ($form_state['values']['uniquetitle_settings']['uniqueinfo'] as $type => $uniqueinfo) {
    if ($uniqueinfo['unique']) {
      $duplications = uniquetitle_get_title_duplications($type, $uniqueinfo['casesensitive']);
      if (count($duplications)) {
        form_set_error("uniquetitle_settings][uniqueinfo][$type", '');
        $items[$type] = uniquetitle_get_title_duplications_item_list($duplications, $node_types[$type]->name);
      }
    }
  }
  if (count($items)) {
    form_set_error('uniquetitle_settings][uniqueinfo', theme('item_list', $items));
  }
}

function uniquetitle_get_title_duplications_item_list($duplications, $type_name) {
  $max_title = 2;
  $max_node = 2;
  $count_dup = count($duplications);
  $to_dup = min($max_title, $count_dup);

  $items = array(
    'data'     => t('Title duplications in content type %type. Cannot setable to unique.', array('%type' => $type_name)),
    'children' => array(),
  );
  for ($i = 0; $i < $to_dup; $i++) {
    $current_t = each($duplications);
    $items['children'][$i]['data'] = $current_t['key'];
    $to_node = min($max_node, count($current_t['value']));
    for ($x = 0; $x < $to_node; $x++) {
      $current_n = each($current_t['value']);
      $items['children'][$i]['children'][] = l($current_n['value']->title, "node/{$current_n['value']->nid}", array('attributes' => array('target' => '_blank')));
    }
    if ($to_node < count($current_t['value'])) {
      $items['children'][$i]['children'][] = '...';
    }
  }
  if ($to_dup < $count_dup) {
    $items['children'][] = '...';
  }
  return $items;
}

/**
 * Azt vizsgálja, hogy egy tartalom típus cím mezőjének egyedinek kell-e lennie.
 * Is required the title must be unique in the $type type?
 *
 * @param string $type
 *    {node}.type
 *
 * @return boolean
 *  Return TRUE if the title field must be unique else return FALSE
 */
function uniquetitle_must_be_unique($type) {
  $settings = uniquetitle_get_settings();
  return (boolean)($settings['uniqueinfo'][$type]['unique']);
}

/**
 * Azt vizsgálja, hogy egy tartalom típus cím mezőjének egyediségének vizsgálatakor
 * legyen-e különbség a kis és nagy betük között.
 *
 * Check the unique title validataion must be casesensitive.
 *
 * @param string $type
 *    {node}.type
 *
 * @return boolean
 */
function uniquetitle_must_be_casesensitive($type) {
  $settings = uniquetitle_get_settings();
  return (boolean)($settings['uniqueinfo'][$type]['casesensitive']);
}

/**
 * Tartalom típuson belül az egyforma című tartalmak lekérdezése.
 * Looking for the same titles in the specified ($type) content type.
 *
 * @param string $type
 *    {node}.type
 *
 * @param mixed $casesensitive
 *    If NULL then the module settings will be used, else the $casesensitive must be a boolean value.
 *
 * @return array
 *    If the $type is not $casesensitive then the title key will be transformed to lowercase.
 *    This sometimes different than the real node title, but this is required for grouping the results.
 *    array(
 *      title_1 => array(node.record_1, node.record_2),
 *      title_2 => array(node.record_3, node.record_4),
 *      ...
 *    )
 */
function uniquetitle_get_title_duplications($type, $casesensitive = NULL) {
  if (is_null($casesensitive)) {
    $casesensitive = uniquetitle_must_be_casesensitive($type);
  }

  $title_field = 'LOWER(nr1.title)';
  $title_operation = 'LOWER(nr1.title) = LOWER(nr2.title)';
  if ($casesensitive) {
    $title_field = 'nr1.title';
    $title_operation = uniquetitle_db_binary_operator('nr1.title', 'nr2.title', '=');
  }

  $sql = "
    SELECT
      n1.*,
      $title_field AS title_
    FROM
      {node}           AS n1
      INNER JOIN
      {node_revisions} AS nr1 ON (n1.vid = nr1.vid)
      INNER JOIN
      {node}           AS n2  ON (n1.type = '%s' AND n1.nid <> n2.nid AND n1.type = n2.type)
      INNER JOIN
      {node_revisions} AS nr2 ON (n2.vid = nr2.vid)
    WHERE
      $title_operation
      AND
      n1.language = n2.language
    ORDER BY
      nr1.title
  ";

  $return = array();
  $dbr = db_query($sql, $type);
  while ($row = db_fetch_object($dbr)) {
    $return[$row->title_][$row->nid] = $row;
  }
  return $return;
}

/**
 * Megvizsgálja hogy az adott paraméterekkel már van-e létező tartalom.
 * Check for an exists node in a given $type and with a given $title.
 *
 * @param string or stdClass $title
 *    If the $title is string then the $type and the $nid is required.
 *    If the $title is object then the $type and the $nid will be ignored.
 *
 * @param string $type
 *    node type
 *
 * @param integer $nid
 *    Új tartalom beküldésénél 0, egyébként a $node->nid. Azért kell hogy saját magához ne hasonlítsa.
 *    Required for an exists content to prevent compare to itself.
 *
 * @return boolean or stdClass
 *    Ha a megadott $title még nem létezik akkor FALSE, egyébként a db_fetch_object() álltal vissza adott sor a node táblából.
 *    If the given $title is not exists then FALSE, else return a record form the {node} table. (The exists one record)
 */
function uniquetitle_title_exists($node) {
  if (uniquetitle_must_be_unique($node->type) == FALSE) {
    return FALSE;
  }

  global $db_type;
  $args = array(
    0 => $node->nid,
    1 => $node->type,
    2 => $node->title,
    3 => $node->language,
  );
  $where = array();
  if (uniquetitle_must_be_casesensitive($node->type)) {
    $where[] = uniquetitle_db_binary_operator('nr.title', "'%s'", '=');
  }
  else {
    $args[2] = drupal_strtolower($args[2]);
    $where[] = "LOWER(nr.title) = '%s'";
  }

  $where[] = "n.language = '%s'";

  $sql = "
    SELECT
      n.*
    FROM
      {node} as n
      INNER JOIN
      {node_revisions} as nr ON (n.nid = nr.nid AND n.nid <> '%d' AND n.type = '%s')
    WHERE
      ". implode(' AND ', $where) ."
  ";
  $dbr = db_query($sql, $args);
  return (($row = db_fetch_object($dbr)) ? $row : FALSE);
}

/**
 * The MySQL and the PostgreSQL has different syntax for the casesensitive operator.
 */
function uniquetitle_db_binary_operator($f1, $f2, $operator = '=') {
  global $db_type;
  switch ($db_type) {
    case 'mysql' :
    case 'mysqli' :
      return "BINARY $f1 $operator $f2";

    case 'pgsql' :
      return "{$f1}::bytea $operator {$f2}::bytea";
  }
}
