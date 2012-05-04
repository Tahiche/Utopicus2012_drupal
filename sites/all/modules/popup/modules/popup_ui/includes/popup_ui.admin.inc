<?php



/* ---- Forms ---- */



function popup_ui_form_formatters(){

  drupal_add_css(drupal_get_path('module', 'popup_ui').'/popup-ui.css');
  module_load_include('inc', 'popup', 'includes/popup.util');

  $formatters = _popup_ui_formatter_settings();

  $form = array(
    '#tree' => TRUE,
  );
  $form['rows'] = array();

	$effect_names = array_keys(array_pop(_popup_effects()));
  $effect_options = array_combine(
    $effect_names,
    array_map('_popup_key_to_title', $effect_names)
  );

  $style_names = array_keys(_popup_styles());
  $style_options = array_combine(
    $style_names,
    array_map('_popup_key_to_title', $style_names)
  );
  array_unshift($style_options, t('Default'));

  $widths = range(100, 500, 50);

  $options = array(
    'position' => array('top-left' => t('Top left'), 'top-right' => t('Top right'), 'bottom-right' => t('Bottom right'), 'bottom-left' => t('Bottom left'),),
    'effect' => $effect_options,
    'style' => $style_options,
    'opacity' => array('1.0' => '1.0', '0.9' => 0.9, '0.8' => 0.8, '0.7' => 0.7, '0.6' => 0.6, '0.5' => 0.5, '0.4' => 0.4, '0.3' => 0.3, '0.2' => 0.2, '0.1' => 0.1,),
    'width' => array_combine($widths, $widths),
  );

  foreach ($formatters as $label => $formatter){
    $form['rows'][$label] = _popup_ui_form_formatters_row(
      $label,
      $formatter,
      $options
    );
  }

  $form['rows']['new'] = _popup_ui_form_formatters_row(
    'new',
    array(
      'origin' => 'bottom-left',
      'expand' => 'bottom-right',
      'effect' => 'none',
      'style' => 0,
      'activate' => 'hover',
    ),
    $options
  );

  $form['buttons']= array(

    array('#value' => '<div class="form-item form-buttons">'),

    'submit' => array(
      '#type'=>'submit',
      '#value'=>t('Save'),
    ),

    array(
    	'#value' => popup_ui_confirm(
    		'Reset to defaults',
    		'Are you sure you want to reset to defaults?<br /> This action cannot be undone!',
    		'admin/settings/popup/formats/reset',
				array(
					'origin' => 'top-left',
					'expand' => 'top-right',
				)
    	)
    ),

    array(
    	'#value' => popup_ui_confirm(
    		'Delete all',
    		'Are you sure you want to delete all the formats?<br /> This action cannot be undone!',
    		'admin/settings/popup/formats/whipe',
				array(
					'origin' => 'top-left',
					'expand' => 'top-right',
				)
    	)
    ),

    array('#value' => '</div>'),

    '#weight' => 2,
  );

  if (!module_exists('ctools')){
    $form[] = array(
      '#prefix' => '<div class="messages info">',
      '#suffix' => '</div>',
      '#value' => t(
        'To enable panel display of nodes, the CTools module should be installed and enabled on the !link.',
        array(
          '!link' => l(
            t('modules administration page'),
            'admin/build/modules'
          )
        )
      ),
      '#weight' => 0,
    );
  }

  return $form;
}



function _popup_ui_form_formatters_row($key, $values, $options){

  module_load_include('inc', 'popup_ui', 'includes/popup_ui.api');

  $row = $key == 'new'
    ? array(
        'label' => array(
          '#default_value' => $values['label'],
          '#size' => 20,
          '#type' => 'textfield',
        ),
      )
    : array(
        'label' => array(
          '#value' => $values['label'],
          '#size' => 20,
          '#type' => 'hidden',
          '#suffix' => $values['label'],
        ),
      );

  $row += array(

    'origin' => array(
      '#default_value' => $values['origin'],
      '#type' => 'select',
      '#options' => $options['position'],
    ),
    
    'expand' => array(
      '#default_value' => $values['expand'],
      '#type' => 'select',
      '#options' => $options['position'],
    ),
  
    'effect' => array(
      '#default_value' => $values['effect'],
      '#type' => 'select',
      '#options' => $options['effect'],
    ),

    'style' => array(
      '#default_value' => $values['style'],
      '#type' => 'select',
      '#options' => $options['style'],
    ),

    'opacity' => array(
      '#default_value' => $values['opacity'],
      '#type' => 'select',
      '#options' => $options['opacity'],
    ),

    'width' => array(
      '#default_value' => $values['width'],
      '#type' => 'select',
      '#options' => $options['width'],
    ),

    'activate' => array(
      '#default_value' => $values['activate'],
      '#type' => 'select',
      '#options' => array('hover' => t('Hover'), 'click' => t('Click')),
    ),
  
    'close' => array(
      '#default_value' => $values['close'],
      '#type' => 'checkbox',
    ),

    'ajax' => array(
      '#default_value' => $values['ajax'],
      '#type' => 'checkbox',
    ),

    'teaser' => array(
      '#default_value' => $values['teaser'],
      '#type' => 'checkbox',
    ),

    'page' => array(
      '#default_value' => $values['page'],
      '#type' => 'checkbox',
    ),

    'links' => array(
      '#default_value' => $values['links'],
      '#type' => 'checkbox',
    ),

    'panel' => array(
      '#default_value' => $values['panel'],
      '#type' => 'checkbox',
    ),

    'inline' => array(
      '#default_value' => $values['inline'],
      '#type' => 'checkbox',
    ),

    'flat' => array(
      '#default_value' => $values['flat'],
      '#type' => 'checkbox',
    ),

    'submenu-origin' => array(
      '#default_value' => $values['submenu-origin'],
      '#type' => 'select',
      '#options' => $options['position'],
    ),

    'submenu-expand' => array(
      '#default_value' => $values['submenu-expand'],
      '#type' => 'select',
      '#options' => $options['position'],
    ),

    'ops' => array(
      '#value' => (
        $key == 'new' 
          ? '&nbsp;'
          : popup_ui_confirm(
          		'Delete', 
          		'Are you sure you want to delete this popup format? It cannot be undone!', 
          		'admin/settings/popup/formats/delete/'.$key,
          		array(
								'origin' => 'bottom-right',
								'expand' => 'bottom-left',
          		)
          	)
      ),
    ),

  );

  if (!module_exists('ctools')){
    $row['panel']['#attributes']['disabled'] = 'disabled';
  }

  return $row;
}



function popup_ui_form_formatters_validate(&$form, &$form_state){

  $values = $form_state['values'];
  $formatters = _popup_ui_formatter_settings();

  foreach($values['rows'] as $label=>$row){

    if (!preg_match('/^[A-Za-z0-9\ ]*$/', $row['label'])){
      form_set_error('rows]['.$label.'][label', t('Labels may only contain letters, numbers and spaces.'));
    }

    if (!$row['label'] && $label!='new'){
      form_set_error('rows]['.$label.'][label', t('Please provide a label.'));
    }

  }
}


function popup_ui_form_formatters_submit(&$form, &$form_state){

  $values = $form_state['values'];
  $formatters = array();

  foreach($form_state['values']['rows'] as $label=>$row){
    if ($row['label']){
      $formatters[$row['label']] = $row;
    }
  }

  _popup_ui_formatter_settings($formatters);
  _popup_ui_flush_all();

  drupal_set_message(t('The popup filters have been saved.'));

  $form['#redirect'] = array('admin/settings/popup/formats');
}



/* ---- Actions ---- */



function popup_ui_delete_format($format){

  $settings = _popup_ui_formatter_settings();

  unset($settings[$format]);

  _popup_ui_formatter_settings($settings);
  _popup_ui_flush_all();

  drupal_set_message(t('The "%format" filter format has been deleted.', array('%format' => $format)));
  drupal_goto('admin/settings/popup/formats');
}



function popup_ui_whipe_formats(){

  _popup_ui_formatter_settings(array());
  _popup_ui_flush_all();

  drupal_set_message(t('All filter formats have been deleted.'));
  drupal_goto('admin/settings/popup/formats');
}



function popup_ui_reset(){

  _popup_ui_formatter_settings(_popup_ui_formatter_defaults());
  _popup_ui_flush_all();

  drupal_set_message(t('Default settings has been restored.'));
  drupal_goto('admin/settings/popup/formats');
}



/* ---- Settings ---- */



function _popup_ui_formatter_defaults(){

  return array (

    'Default hover' => array (
      'label' => 'Default hover',
      'origin' => 'bottom-left',
      'expand' => 'bottom-right',
      'effect' => 'default',
      'opacity' => '1.0',
      'width' => '150',
      'activate' => 'hover',
    ),
  
    'Default click' => array (
      'label' => 'Default click',
      'origin' => 'bottom-left',
      'expand' => 'bottom-right',
      'effect' => 'default',
      'opacity' => '1.0',
      'width' => '150',
      'activate' => 'click',
      'close' => 1,
    ),
  
    'Default node' => array (
      'label' => 'Default node',
      'origin' => 'bottom-left',
      'expand' => 'bottom-right',
      'effect' => 'default',
      'opacity' => '1.0',
      'width' => '150',
      'activate' => 'hover',
      'teaser' => 1,
      'links' => 1,
    ),
  
    'Left block' => array (
      'label' => 'Left block',
      'origin' => 'top-right',
      'expand' => 'bottom-right',
      'effect' => 'default',
      'opacity' => '1.0',
      'width' => '150',
      'activate' => 'hover',
    ),
  
    'Right block' => array (
      'label' => 'Right block',
      'origin' => 'top-left',
      'expand' => 'bottom-left',
      'effect' => 'default',
      'opacity' => '1.0',
      'width' => '150',
      'activate' => 'hover',
    ),
  
    'Popup menu' => array (
      'label' => 'Popup menu',
      'origin' => 'top-right',
      'expand' => 'bottom-right',
      'effect' => 'default',
      'opacity' => '1.0',
      'submenu-origin' => 'top-right',
      'submenu-expand' => 'bottom-right',
      'width' => '200',
      'activate' => 'hover',
    ),

    'Flat popup menu' => array (
      'label' => 'Flat popup menu',
      'origin' => 'top-right',
      'expand' => 'bottom-right',
      'effect' => 'default',
      'opacity' => '1.0',
      'width' => '200',
      'activate' => 'hover',
      'flat' => 1,
    ),
  );
}



function _popup_ui_formatter_settings($new_formatters = FALSE){

  static $formatters = FALSE;

  if (is_array($new_formatters)){
    $formatters = $new_formatters;
    variable_set('popup-formatters', $formatters);
  }

  if (!$formatters){
    $formatters = variable_get('popup-formatters', _popup_ui_formatter_defaults());
  }

  return $formatters;
}



/* ---- Utility functions ---- */



function _popup_ui_flush_all(){
  $core = array('cache', 'cache_block', 'cache_filter', 'cache_page');
  $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
  foreach ($cache_tables as $table) {
    cache_clear_all('*', $table, TRUE);
  }
}


