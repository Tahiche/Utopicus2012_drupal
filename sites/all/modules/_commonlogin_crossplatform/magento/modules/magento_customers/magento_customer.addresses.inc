<?php
// $Id$
/**
 * @file
 * Implements forms/callbacks for managing customers addresses
 */
define('ALLOWED_ADDRESS_NUM_PER_USER', 10);
/**
 * Page builder; Present the page to Manage user's addreses (magento_customer_addresses).
 */
function magento_customer_manage_addresses($account, $category = 'account') {
  $name = empty($account->uid)?t('Guest'):$account->name;
  drupal_set_title(check_plain($name) . ': ' . t("Addresses"));
  $output = '';
  $items = array();

  $existed_addresses = magento_customers_get_customer_addresses($account);
  foreach ($existed_addresses as $key => $address) {
    $output = '';
    $output .= theme('magento_customers_address_item', $account, $address);
    if (empty($address['is_read_only'])) {
        $output .= ' ' . l( t('Modify') , "user/" . $account->uid . "/edit-address/{$address['customer_address_id']}");
        $output .= ' ' . l( t('Remove'), "user/" . $account->uid . "/remove-address/{$address['customer_address_id']}");
    }
    $output .= '<br/>';
    $items[] = $output;
  }
  $output = theme('item_list', $items, $title = NULL, $type = 'ul', $attributes = NULL);
  return '<div>' . l( t('New address') , "user/" . $account->uid . "/new-address") . '</div>' . $output;
}

/**
 * New Address form
 *
 * @ingroup forms
 * @see magento_customer_form_address_form()
 * @see magento_customer_form_address_form_validate()
 * @see magento_customer_form_address_form_submit()
 */
function magento_customer_new_address($account) {
  $name = empty($account->uid)?t('Guest'):$account->name;
  drupal_set_title(check_plain($name) . ': ' . t("New Address"));
  return drupal_get_form('magento_customer_form_address_form', $account);
}
/**
 * Edit Address form
 *
 * @ingroup forms
 * @see magento_customer_form_address_form()
 * @see magento_customer_form_address_form_validate()
 * @see magento_customer_form_address_form_submit()
 */
function magento_customer_edit_address($account, $address_id) {
  $address = magento_customers_get_customer_addresses($account, $address_id);
  if (empty($address[0]) || empty($address_id)) {
    drupal_set_message(t('The user doesn\'t have requested address.'));
    drupal_goto('user/' . $account->uid . '/manage-addresses');
  }

  $name = empty($account->uid)?t('Guest'):$account->name;
  drupal_set_title(check_plain($name) . ': ' . t("Edit Address"));
  return drupal_get_form('magento_customer_form_address_form', $account, $address[0], $address_id);
}
/**
 * Remove Address form
 *
 * @ingroup forms
 * @see magento_customer_delete_address_form()
 * @see magento_customer_delete_address_form_submit()
 */
function magento_customer_remove_address($account, $address_id) {
  $address = magento_customers_get_customer_addresses($account, $address_id);
  if (empty($address[0]) || empty($address_id)) {
    drupal_set_message(t('The user doesn\'t have requested address.'));
    drupal_goto('user/' . $account->uid . 'manage-addresses');
  }

  $name = empty($account->uid)?t('Guest'):$account->name;
  drupal_set_title(check_plain($name) . ': ' . t("Delete Address"));
  return drupal_get_form('magento_customer_delete_address_form', $account, $address_id);
}
/**
 * Genereate form to remove user address
 */
function magento_customer_delete_address_form($form_state, $user = NULL, $address_id = NULL) {
  $form = array();
  $form['user_uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $form['address_id'] = array(
    '#type' => 'value',
    '#value' => $address_id,
  );
  $form['confirm_msg'] = array(
    '#type' => 'markup',
    '#value' => t('Are you sure you want to delete this address?'),
  );
  $form['delete'] = array(
      '#prefix' => '<div class="btn30">',
      '#suffix' => '</div>',
      '#type' => 'submit',
      '#value' => t('Delete address'),
    );

  if (empty($user->uid)) {
    $form['#submit'][] = 'magento_customer_anonymous_delete_address_form_submit';
  }
  else {
    $form['#submit'][] = 'magento_customer_delete_address_form_submit';
  }
  return $form;
}
function magento_customer_delete_address_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $account = user_load($values['user_uid']);
  $profile = $account->customer;

  $address_id = $values['address_id'];

  foreach ($profile['addresses'] as $key => $address ) {
    if ($address['customer_address_id'] == $address_id) {
      unset($profile['addresses'][$key]);
      break;
    }
  }

  $data = array( 'customer' => $profile );
  user_save($account, $data);

  drupal_set_message(t('Address deleted.'));
  drupal_goto('user/' . $account->uid . '/manage-addresses');
}

/**
 * Delete address data of the anonymous user
 */
function magento_customer_anonymous_delete_address_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $address_id = $values['address_id'];

  unset($_SESSION['addresses'][$address_id]);

  drupal_set_message(t('Address deleted.'));
}

/**
 * Generate form for add/edit user adress
 */
function magento_customer_form_address_form($form_state, $user = NULL, $address = NULL, $address_id = NULL) {
  $existed_addresses = magento_customers_get_customer_addresses($user);

  $address_template = array(
                            // FIXME: hardcode
                            'country_id' => 'FR',
                            'region_id' => 182,
                            // end FIXME: hardcode

                            'is_default_shipping' => 0,

                            'street_number' => '',
                            'street_name' => '',

                            'postcode' => '',
                            'city' => '',
                            'street' => ''. PHP_EOL .'',

                            'gender' => 1,

                            'firstname' => '',
                            'middlename' => '',
                            'lastname' => '',

                            'company' => '',
                            'telephone' => '',
                            'fax' => '',

                            'tmp_id' => '',
                            'email' => '',
                        );
  if (empty($address)) {
    $address = $address_template;
  }
  elseif (is_array($address)) {
    $address = array_merge($address_template, $address);
  }

  //========= save ids
  $uid = empty($user->uid)?0:$user->uid;
  $form['user_uid'] = array(
    '#type' => 'value',
    '#value' => $uid,
  );
  $form['address_id'] = array(
    '#type' => 'value',
    '#value' => $address_id,
  );

  //========= address fields
  $form['address'] = array(
    '#tree' => TRUE,
  );

  $options = array(
    '1' => 'Male',
    '2' => 'Female',
  );
  $form['address']['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => $address['gender'],
  );
  $form['address']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#attributes' => array(
      'title' => $address['firstname'],
    ),
    '#default_value' => $address['firstname'],
  );

  $form['address']['middlename'] = array(
    '#type' => 'textfield',
    '#title' => t('Middle Name/Initial'),
    '#attributes' => array(
      'title' => $address['lastname'],
    ),
    '#default_value' => $address['middlename'],
  );

  $form['address']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Sirname'),
    '#required' => TRUE,
    '#attributes' => array(
      'title' => $address['lastname'],
    ),
    '#default_value' => $address['lastname'],
  );

  $form['address']['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#attributes' => array(
      'title' => $address['company'],
    ),
    '#default_value' => $address['company'],
  );

  $form['address']['street'] = array('#type' => 'item', '#tree' => TRUE);
  $address['street'] = explode(PHP_EOL, $address['street']);

  $form['address']['street'][0] = array(
    '#type' => 'textfield',
    '#title' => t('Address 1'),
    '#required' => TRUE,
    '#attributes' => array(
      'title' => '',
    ),
    '#default_value' => $address['street'][0],
  );
  $form['address']['street'][1] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#attributes' => array(
      'title' => '',
    ),
    '#default_value' => $address['street'][1],
  );

  $form['address']['telephone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#required' => TRUE,
    '#attributes' => array(
      'title' => $address['telephone'],
    ),
    '#default_value' => $address['telephone'],
  );

  $form['address']['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax'),
    '#attributes' => array(
      'title' => $address['fax'],
    ),
    '#default_value' => $address['fax'],
  );

  //========== sity code info
  $form['address']['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip/Postal Code'),
    '#required' => TRUE,
    '#attributes' => array(
      'title' => $address['postcode'],
    ),
    '#maxlength' => 7,
    '#default_value' => $address['postcode'],
  );

  $form['address']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#required' => TRUE,
    '#attributes' => array(
      'title' => $address['city'],
    ),
    '#default_value' => $address['city'],
  );
  ///
  $countries = magento_customer_address_country_list();
  $form['address']['country_id'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#required' => TRUE,
    '#options' => $countries,
    '#attributes' => array(
      'title' => $address['country_id'],
    ),
    '#default_value' => $address['country_id'],
    '#ahah' => array(
        'path' => "address/get_regions",
        'wrapper' => 'wrapper_region_id',//'edit-address-region-id-wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
  );

  $regions = magento_customer_address_regions_list($address['country_id']);
  if (!empty($regions)) {
    $form['address']['region_id'] = array(
      '#prefix' => '<div id="wrapper_region_id" style="clear:both;">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#title' => t('Region'),
      '#required' => TRUE,
      '#options' => $regions,
      '#attributes' => array(
        'title' => $address['region_id'],
      ),
      '#default_value' => $address['region_id'],
    );
  }
  else {
    $form['address']['region_id'] = array(
      '#prefix' => '<div id="wrapper_region_id">',
      '#suffix' => '</div>',
      '#type' => 'textfield',
      '#title' => t('Region'),
      '#required' => FALSE,
      '#default_value' => $address['region_id'],
    );
  }

  //=========== default
  //if (empty($existed_addresses)) {
    $form['address']['is_default_billing'] = array(
      '#type' => 'checkbox',
      '#title' => t("Use this address as default billing address"),
      '#default_value' => $address['is_default_billing'],
      //'#disabled' => TRUE,
    );
  //}
  $form['address']['is_default_shipping'] = array(
      '#type' => 'checkbox',
      '#title' => t("Use this address as default shipping address"),
      '#default_value' => $address['is_default_shipping'],
    );

  //=========== general
  if (!empty($existed_addresses)&& count($existed_addresses)>= ALLOWED_ADDRESS_NUM_PER_USER) {
    //form_set_error('user_uid', t('Only %num Addreses per user are allowed.', array('%num'=>ALLOWED_ADDRESS_NUM_PER_USER)));
    $form['#messages'][] = t('Only %num Addreses per user are allowed.', array('%num' => ALLOWED_ADDRESS_NUM_PER_USER));
  }
  else {
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => empty($address_id) ? t('Create address') : t('Modify address'),
    );
  }

  $form['#title'] = empty($address_id) ? t('Create shipping address') : t('Modifier shipping address');
  $form['#theme'] = 'magento_customer_form_address_form_theme';
  if (empty($user->uid)) {
    $form['#submit'][] = 'magento_customer_anonymous_form_address_form_submit';
    // Add an extra Address Field for anonymous checkout
    $form['address']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#required' => TRUE,
      '#attributes' => array(
        'title' => $address['email'],
      ),
      '#default_value' => $address['email'],
      '#weight' => -1,
    );
  }
  else {
    $form['#submit'][] = 'magento_customer_form_address_form_submit';
  }
  $form['#validate'][] = 'magento_customer_form_address_form_validate';

  return $form;
}

/**
 * Helper function that prepares customer address data array for saving.
 */
function _magento_customer_sanitize_address($values) {
  $address_data = array();

  $address_data['tmp_id'] = $values['address']['firstname'] . $values['address']['firstname'] . time();

  $address_data['street'] = implode(PHP_EOL, $values['address']['street']);
  $address_data['street_name'] = $values['address']['street'][0];

  $address_data['firstname'] = $values['address']['firstname'];
  $address_data['middlename'] = $values['address']['middlename'];
  $address_data['lastname'] = $values['address']['lastname'];
  $address_data['company'] = $values['address']['company'];

  $address_data['telephone'] = $values['address']['telephone'];
  $address_data['fax'] = $values['address']['fax'];
  $address_data['city'] = $values['address']['city'];
  $address_data['postcode'] = $values['address']['postcode'];
  $address_data['gender'] = $values['address']['gender'];
  $address_data['is_default_billing'] = empty($values['address']['is_default_billing']) ? 0 : 1;
  $address_data['is_default_shipping'] = empty($values['address']['is_default_shipping']) ? 0 : 1;

  $address_data['country_id'] = $values['address']['country_id'];
  $address_data['region_id'] = $values['address']['region_id'];

  //Sanitize address
  $address_template = array(
                            'is_default_shipping' => 0,

                            'street_number' => '',
                            'street_name' => '',

                            'postcode' => '',
                            'city' => '',
                            'street' => '',

                            'gender' => 1,

                            'firstname' => '',
                            'lastname' => '',
                            'telephone' => '',
                            'fax' => '',

                            'tmp_id' => '',
                        );
  $new_address = array_merge($address_template, $address_data);

  return $new_address;
}

/**
 * Validate an address data
 */
function magento_customer_form_address_form_validate($form, &$form_state) {
  // We need a valid user email on `Checkout as Guest`
  if (isset($form['address']['email']) && !valid_email_address($form_state['values']['address']['email'])) {
    form_set_error('address][email', t('Please, check and specify a valid email address of yours.'));
  }
}

/**
 * Save address data of the anonymous user
 */
function magento_customer_anonymous_form_address_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $address_id = $values['address_id'];

  if (!isset($_SESSION['addresses'])) {
    $_SESSION['addresses'] = array();
  }

  //Sanitize address
  $new_address =_magento_customer_sanitize_address($values);
  $new_address['email'] = $values['address']['email'];

  if ($address_id === NULL) {
    $new_address['customer_address_id'] = 'guestaddr' . count($_SESSION['addresses']);
  }
  else {
    $new_address['customer_address_id'] = $address_id;
  }

  $_SESSION['addresses'][$new_address['customer_address_id']] = $new_address;

  drupal_set_message(empty($address_id)? t('Address created'):t('Address modified.'));
}

/**
 *  Save address data
 */
function magento_customer_form_address_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $account = user_load($values['user_uid']);
  $profile = $account->customer;

  $address_id = $values['address_id'];

  //Sanitize address
  $new_address =_magento_customer_sanitize_address($values);

  // Uncheck other is_default_shipping/is_default_billing IF the new_address is_default_shipping/is_default_billing
  if (!empty($profile['addresses'])) {
    foreach ($profile['addresses'] as $key => $address ) {
      if (!empty($new_address['is_default_billing'])) {
        $profile['addresses'][$key]['is_default_billing'] = 0;
      }
      if (!empty($new_address['is_default_shipping'])) {
        $profile['addresses'][$key]['is_default_shipping'] = 0;
      }
    }
  }

  if (empty($address_id)) {
    // new address
    $profile['addresses'][] = $new_address;
  }
  else {
    foreach ($profile['addresses'] as $key => $address ) {
      if ($address['customer_address_id'] == $address_id) {
        $new_address['customer_address_id'] = $address_id;
        $profile['addresses'][$key] = $new_address;
        break;
      }
    }
  }

  $data = array( 'customer' => $profile );
  user_save($account, $data);

  drupal_set_message(empty($address_id)? t('Address created'):t('Address modified.'));
  drupal_goto('user/' . $account->uid . '/manage-addresses');
}

/**
 * Load Countries and Regions data from Magento and save it in local storage.
 *
 * @return an associative array of countries
 *   ( where item's key is country code and item's value is country name)
 */
function magento_customer_load_countries_from_magento() {
  $countries = array();

  $data = magento_api_get_country_list();

  if (!empty($data['countries'])) {
    // Clear old Countries data and save new data
    db_query("TRUNCATE {magento_country}");

    $country_items = (array)$data['countries'];
    $sql2 = '';
    $variables = array();
    foreach ($country_items as $country) {
      if (drupal_strlen($sql2)) {
        $sql2 .= ', ';
      }
      $sql2 .= "('%s', '%s')";
      $variables[] = $country['value'];
      $variables[] = $country['label'];

      $countries[$country['value']] = $country['label'];
    }

    if (db_query('INSERT INTO {magento_country} (country_id, name) VALUES ' . $sql2, $variables)) {
      drupal_set_message(t('%count countries were imported from Magento', array('%count' => count($countries))));
    }
    else {
      $countries = array();
    }
  }

  if (!empty($data['regions']['items'])) {
    // Clear old Regions data and save new data
    db_query("TRUNCATE {magento_region}");

    $regions = (array)$data['regions']['items'];
    $sql2 = '';
    $variables = array();
    foreach ($regions as $region) {
      if (drupal_strlen($sql2)) {
        $sql2 .= ', ';
      }
      $sql2 .= "(%d, '%s', '%s', '%s')";
      $variables[] = $region['region_id'];
      $variables[] = $region['country_id'];
      $variables[] = $region['code'];
      //$variables[] = $region['default_name'];
      $variables[] = empty($region['name']) ? $region['code'] : $region['name'];
    }

    db_query('INSERT INTO {magento_region} (region_id, country_id, code, name) VALUES ' . $sql2, $variables);
  }

  return $countries;
}
/**
 * Get list of Country options in Drupal
 *
 * @return associative of countries
 *   ( where item's key is country code and item's value is country name)
 */
function magento_customer_address_country_list() {
  $countries = array();
  $res = db_query("SELECT * FROM {magento_country}");
  while ($country = db_fetch_array($res)) {
    $countries[$country['country_id']] = $country['name'];
  }
  // if there is no local data then load data from remote Magento
  if (empty($countries)) {
    $countries = magento_customer_load_countries_from_magento();
  }
  return $countries;
}

/**
 * Get list of allowed Region options in the specified Country
 *
 * @return array of allowed regions in the country
 *   ( where item's key is region ID and item's value is region name)
 */
function magento_customer_address_regions_list($country_id = NULL) {
  if (empty($country_id)) {
    return array();
  }

  $regions = array();
  $res = db_query("SELECT * FROM {magento_region} WHERE country_id = '%s'", $country_id);
  while ($region = db_fetch_array($res)) {
    $regions[$region['region_id']] = empty($region['name']) ? $region['code'] : $region['name'];
  }

  return $regions;
}

/**
 * Callback to load regions by country ID
 */
function magento_customer_get_regions_field() {
  $country_id = empty($_REQUEST['address']['country_id']) ? NULL : $_REQUEST['address']['country_id'];

  $regions = magento_customer_address_regions_list($country_id);
  if (!empty($regions)) {
    $form['address']['region_id'] = array(
      '#type' => 'select',
      '#title' => t('Region'),
      '#required' => TRUE,
      '#options' => $regions,
    );
  }
  else {
    $form['address']['region_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Region'),
      '#required' => FALSE,
      '#default_value' => '',
      '#value' => '',
    );
  }

  // Final rendering callback.
  print drupal_json(array('status' => FALSE, 'data' => _magento_customer_get_regions_ahah_render($form['address']['region_id'], 'region_id')));
}
/**
 * Helper functions for magento_customer_get_regions_field
 *
 * @see magento_customer_get_regions_field()
 */
function _magento_customer_get_regions_ahah_render($fields, $name) {
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);
  $form['address'][$name] = $fields;
  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $new_form = $form['address'][$name];
  return drupal_render($new_form);
}