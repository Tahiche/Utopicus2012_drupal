<?php
// $Id$

/**
 * @file
 * Provides basic admin settings for Magento API connection
 */

/**
 * Provide Magento API connection settings form
 */
function magento_api_settings_form() {
  $form = array();

  // Make sure we have already created a magento user
  $magento_uid = magento_get_default_user();

  $form['magento_host_url'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Magento host URL',
    '#description'   => t('Enter host for XML-RPC connection'),
    '#default_value' => variable_get('magento_api_url', 'http://magentohost/api/xmlrpc/'),
    '#required'      => TRUE,
  );

  $form['magento_user'] = array(
    '#type'          => 'textfield',
    '#title'         => 'API user',
    '#description'   => t('Enter username for XML-RPC connection'),
    '#default_value' => variable_get('magento_api_login', ''),
    '#required'      => TRUE,
  );

  $form['magento_key'] = array(
    '#type'          => 'password',
    '#title'         => 'API key (password)',
    '#description'   => t('Enter password for XML-RPC connection'),
    //'#default_value' => variable_get('magento_api_password', ''),
  );

  $api_key = variable_get('magento_api_password', '');
  if (empty($api_key)) {
    $form['magento_key']['#required'] = TRUE;
    $form['magento_key']['#title'] = 'API key (password)';
  }

  $form['magento_api_log'] = array(
    '#type'          => 'select',
    '#title'         => t('Log All Magento API calls'),
    '#description'   => t("Don't enable on production."),
    '#options'       => array('0' => t('Disabled'), '1' => t('Enabled'), '9' => t('Enabled debug mode (log call params)')),
    '#default_value' => variable_get('magento_api_log', 0),
  );

  $form['magento_test_connection'] = array(
    '#type'          => 'submit',
    '#value'         => MAGENTO_T_TEST_CONNECTION,
    '#submit'        => array('magento_api_settings_form_submit'),
  );


  $form = system_settings_form($form);

  $form['#submit'][] = 'magento_api_settings_form_submit';

  return $form;
}

/**
 * magento_api_settings_form_submit hook
 */
function magento_api_settings_form_submit($form, &$form_state) {
  // TODO: do we need to check for Magento API module ?
  $form_values = $form_state['values'];

  $credentials = array(
    'url'      => $form_values['magento_host_url'],
    'login'    => $form_values['magento_user'],
    'password' => $form_values['magento_key']
  );

  switch ($form_state['clicked_button']['#value']) {
    // Test connection
    case MAGENTO_T_TEST_CONNECTION:
      if (empty($credentials['login'])) {
        drupal_set_message(t("API user can't be emtpy. Please, set Magento API user and API password."), 'warning');
        return;
      }
      if (empty($credentials['password'])) {
        drupal_set_message(t('No API key (password) were specified. The stored password was used.'), 'warning');
        $credentials['password'] = variable_get('magento_api_password', '');
      }
      $result = magento_api_test_credentials($credentials);
      if ($result && is_array($result)) {
        $message_type = '';
        if ('ok' == $result['status']) {
          $message_type = 'status';
          drupal_set_message(t('Connection succeeded'), 'ok');
        }
        else {
          $message_type = 'warning';
          drupal_set_message(t('Connection failed'), 'error');
        }
        if (isset($result['message']) && is_array($result['message'])) {
          foreach ($result['message'] as $message) {
            drupal_set_message($message, $message_type);
          }
        }
        elseif (!empty($result['message'])) {
          drupal_set_message($result['message'], $message_type);
        }
      }
      break;

    // Save credentials
    case t('Save configuration'):
      // Don't change password if no password were specified.
      if (empty($credentials['password'])) {
        unset($credentials['password']);
      }
      magento_api_set_credentials($credentials);
      break;
  }
}
