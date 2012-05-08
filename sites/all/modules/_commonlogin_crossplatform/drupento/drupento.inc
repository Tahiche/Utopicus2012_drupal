<?php
require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'DrupentoException.class.php');
require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'DrupentoCacheRequest.class.php');

define('DRUPENTO_CONFIG_FILE_NAME', 'drupento.conf.php');
define('DRUPENTO_CACHE_FILE_EXTENSION', '.cache.php');
define('DRUPENTO_CACHE_TYPE_PATH', 'path');
define('DRUPENTO_CACHE_TYPE_DOM_ELEMENT', 'dom_element');
define('DRUPENTO_CACHE_TYPE_CALLBACK', 'callback');
define('DRUPENTO_CACHE_TYPE_NODE', 'node');

function drupento_flush_cache() {	
	$goto_on_finish = 'admin/build';
	
	if ( !($cache_path_main = drupento_conf_get('cache_path')) ) {
		drupal_set_message('No cache path set. Check your configuration'). 
		drupal_goto($goto_on_finish);
	}
	
	if ( !($dir_obj = dir($cache_path_main)) ) {
		drupal_set_message("Could not open: {$cache_path_main}"); 
		drupal_goto($goto_on_finish);
	}
	
	//
	// Read subdirectories of the main cache directory
	// 
	while( false !== ($cache_dir_name = $dir_obj->read()) ) {
		
		if ( substr($cache_dir_name, 0, 1) != '.' ) {
			$cache_dir_path = $cache_path_main . DIRECTORY_SEPARATOR . $cache_dir_name;
			if ( is_dir($cache_dir_path) ) {
				if ( !($cache_dir_obj = dir($cache_dir_path)) ) {
					drupal_set_message("Could not open: {$cache_dir_path}", 'error'); 
					continue;
				}
		
				//
				// Read files in the subdirectory
				//
				$messages = array();
 				while( false !== ($cache_file_name = $cache_dir_obj->read()) ) {
					
					if ( substr($cache_file_name, 0, 1) != '.' ) {
						
						$extension = constant('DRUPENTO_CACHE_FILE_EXTENSION');
						$extension_len = strlen($extension);

						$cache_file_path = $cache_dir_path . DIRECTORY_SEPARATOR . $cache_file_name;
						
						//
						// Make sure this is actually a cache file
						//
						if ( $extension && (substr($cache_file_path, 0-$extension_len) == $extension) ) {
							unlink($cache_file_path);		
							$messages[] = "Removed {$cache_file_path}";
						}
					}					
				}
			}
		}		
	}
	
	if(count($messages)) {
		array_unshift($messages, "<h1>Caches Cleared</h1>");
	}
	else {
		array_unshift($messages, "<h1>No Cached Files</h1>");
	}
	
	$hook_messages = module_invoke_all('drupento_cache_clear');
	
	if( count($hook_messages) ) {
		$messages = array_merge($messages, $hook_messages);
	}
	
	return join($messages, '<br />');
}

/**
 * find out what the Drupal base path is.
 * There might be a better way of doing this.
 */
function drupento_get_drupal_base_path() {	
	if ( !function_exists('_drupal_bootstrap') ) {
		trigger_error( 'You cannot call ' . __FUNCTION__ . ' outside of Drupal. use the drupal_base_path configuration setting instead.', E_USER_ERROR);
	}
	
	return realpath(getcwd());
}

/**
 * Get a cache name string from input
 *
 * @return string
 * 
 **/
function drupento_get_parsed_path( $input ) {
	$input = str_replace('#', 'id_', $input);

	if( substr($input, 0, 1 ) == '.' ) {
		$input = 'class_' . substr($input, 1);
	}
	
	return preg_replace('/[^A-Za-z0-9-_]/', '_', $input);
}

/**
 * Retrieve or generate a session id  
 *
 * @return string
 * 
 **/
function drupento_get_session_id( $realm = 'frontend' ) {
	if( isset($_COOKIE[$realm]) && strlen($_COOKIE[$realm]) > 0 ) {
		return $_COOKIE[$realm];
	}
	else {
		// generate a session ID
		$hash = md5(uniqid(rand(), true));
		$session_id = substr($hash, 0, 26);
				
		setcookie($realm, $session_id, time() + variable_get('drupento_cookie_lifetime', 2000000), '/', variable_get('drupento_cookie_domain', ''));
		return $session_id;
	}
}

/**
 * Helper function used to get configuration options while in different scopes
 *
 * @return mixed 
 */
function drupento_conf_get($variable) {	
	if( class_exists('DrupentoConf') ) {
		// in magento
		return DrupentoConf::Get($variable);
	}
	else {
		// in drupal
		if( !$ret = variable_get('drupento_' . $variable, NULL) ) {
			$ret = variable_get($variable, NULL);
		}
		return $ret;
	}
}

/**
 * Gets the Drupento configuration options and adds this modules path to the array 
 *
 * @return array
 *
 **/
function _drupento_get_module_configuration( $admin_form ) {
	$settings = array();
	
	foreach( $admin_form as $variable_name => $options ) {
		// since we are re-using the form, our default values will always 
		// be either the default of the field, or the value set
		// also, we dont want values like #weight or #type
		if( !preg_match('/^[#]/', $variable_name) ) {	
			$settings[$variable_name] = $options['#default_value']; 
		}
	}
	
	$settings['drupento_module_path'] = dirname(__FILE__);
	$settings['drupento_drupal_base_path'] = drupento_get_drupal_base_path();
	
	// ok to user $_SERVER['HTTP_HOST'] here since we are certainly in drupal when the 
	// settings are being saved/written
	$settings['drupento_drupal_base_url'] = $_SERVER['HTTP_HOST'];	
	
	return $settings;	
}

/**
 * Generates a string from the configuration options
 *
 * @return string
 * 
 **/
function _drupento_get_module_configuration_syntax( $settings ) {	
	$syntax = "array( \n";
	
	foreach($settings as $key => $value) {
		$key = str_replace('drupento_', '', $key);
		$syntax .= "\t\t'{$key}' => '{$value}',\n";
	}
	
	$syntax .= "\t);";
	return $syntax;
}

/**
 * Writes the module configuration options to a file in the Magento install path
 *
 * @return void
 * @author Mike
 **/
function _drupento_write_mage_configuration_file( $admin_form ) {
	$settings = _drupento_get_module_configuration($admin_form);
	$settings_array_syntax = '$Conf = ' . _drupento_get_module_configuration_syntax( $settings );
	// variable names
	$variable = '$variable';	
	$conf_variable = '$Conf[$variable]';
	
	$file = <<<EOL
<?php 
define('DRUPENTO_BOOTSTRAPPED', 1);
define('DRUPENTO_BOOTSTRAP_PATH', dirname(__FILE__));

require_once(DrupentoConf::Get('module_path') . DIRECTORY_SEPARATOR . 'drupento.inc');
require_once(DrupentoConf::Get('module_path') . DIRECTORY_SEPARATOR . 'Drupento.class.php');

class DrupentoConf {
	static {$settings_array_syntax}
	
  	public static function Get($variable) {
		if( isset(self::{$conf_variable}) ) {
			return self::{$conf_variable};
		}
		else {
			return null;	
		}
  	}
}
?>
EOL;
	
	$magento_base_path = $settings['drupento_magento_install_path'];
	$config_file = $magento_base_path . DIRECTORY_SEPARATOR . constant('DRUPENTO_CONFIG_FILE_NAME');
	
	$handle = fopen($config_file, 'w');
	
	if( !fwrite($handle, $file) ) {
		watchdog('error', 'Config file could not be written');
		return false;
	}
	else {
		fclose($handle);	
		return true;		
	}
	
	return false;	
}

function drupento_get_cache_file_path_by_request_settings( $cache_settings ) {
	$selector = null;
	if ( isset($cache_settings['selector']) && $cache_settings['selector'] ) {
		$selector = $cache_settings['selector'];
	}

	return drupento_get_cache_file_path( $cache_settings['type'], $cache_settings['target'], $selector );
}

/**
 * Gets a cache file path based on configuration params
 *
 * @return string
 * 
 **/
function drupento_get_cache_file_path( $type, $target, $selector = null, $options = array() ) {		
	try { 
	    drupento_debug( 'type: ' . $type . ' target: ' .  $target . ' selector: ' .  $selector, array('location' => __FUNCTION__) );
		$extension = constant('DRUPENTO_CACHE_FILE_EXTENSION');	
		$cache_base_path = drupento_conf_get('cache_path');

		if( !$cache_base_path ) {
			throw new DrupentoUserSettingException('No cache path specified.');
		}
	
		//
		// get the correct filename function
		//
		$name_func = 'drupento_get_parsed_path';
		$this_cache_path = $cache_base_path . DIRECTORY_SEPARATOR . drupento_get_parsed_path($type);
		drupento_make_cache_directory_if_not_exists($this_cache_path);
		$cache_filename = drupento_get_parsed_path($type) . '.' . drupento_get_parsed_path($target);
		
		if ( $selector && $type != constant('DRUPENTO_CACHE_TYPE_CALLBACK') ) {
			$cache_filename .= '.' . drupento_get_parsed_path($selector);
		}
		
		$cache_filename .= $extension;
		
		$cache_file = $this_cache_path . DIRECTORY_SEPARATOR . $cache_filename;
		$tmp_file = $cache_file . '.tmp';
		
		$cache = array(
			'cache_filename' => $cache_filename,
			'cache_file' => $cache_file,
			'tmp_file' => $tmp_file
		);
		
		return $cache;
	}
	catch( Exception $e ) {
		throw $e;
	}
}

/**
 * Creates a cache directory during caching if the dir does not exist
 *
 * @return boolean
 * 
 **/
function drupento_make_cache_directory_if_not_exists( $this_cache_path ) {
	try {
		// if the directory does not exist, create it
		if( !file_exists($this_cache_path) ) {
			$cache_dir_umask = drupento_conf_get('cache_dir_umask');	
			
			if( !$cache_dir_umask) {
				$cache_dir_umask = 0777;
			}
			else {
				$cache_dir_umask = octdec( (int) $cache_dir_umask );
			}
			
			// TODO: figure out why casting umask is not working
	
			// 
			// Note: mkdir has its errors suppressed so that we can get
			// our own error message (Magento quits on notices, which 
			// is what mkdir will return if it can't complete. 
			// however, the error displayed isn't as specific as ours)
			//
			if( !@mkdir($this_cache_path, $cache_dir_umask) ) {
				throw new DrupentoException( 'The cache directory ' . $this_cache_path . ' could not be created. Check the permissions on the cache_path set in the Drupento settings');
			}
			return true;
		}	
	}
	catch( Exception $e ) {
		throw $e;
	}
}

/**
 * Caches user-defined api(or otherwise) output from the mage config file
 *
 * @param
 * @return
 *
 **/
function drupento_cache_user_defined_function($func) {
	try {
		
		if ( !($callback_config_file = variable_get('drupento_callback_config_file', null)) ) {
			throw new DrupentoUserSettingException( 'Callback config file doesn\'t appear to be set. Check Drupento settings.' );
		}	
		
		require_once($callback_config_file);
	
		if( function_exists($func) ) {
			if ( !($cache_path = drupento_get_cache_file_path(constant('DRUPENTO_CACHE_TYPE_CALLBACK'), $func)) ) {
				throw new DrupentoException( "Couldn't generate cache file for callback: " . $func);
			}
			else {
				$html = $func();
				drupento_write_cache_file($cache_path, $html);
			}
		}
		else {
			throw new DrupentoException( "user-specified callback doesn't seem to exist: " . $func);
		}
		
		return $cache_path;
	}
	catch( Exception $e ) {
		throw $e;
	}
}

/**
 * Gets the raw html ouput from a drupal path
 *
 * @return string
 * 
 **/
function drupento_get_html_from_uri_path( $uri_path ) {
	// in case the path is an alias, get the source
	$source_uri_path = drupal_lookup_path('source', $uri_path);

	menu_set_active_item($source_uri_path);
	$raw_output = menu_execute_active_handler( $source_uri_path );
	$html = theme('page', $raw_output);
	return $html;
}
 
function drupento_cache_file_is_stale( $file ) {
	if( drupento_cache_file_is_already_cached( $file ) ) {

		$cache_lifetime = drupento_conf_get('cache_lifetime', 60);
		
		if ( $cache_lifetime === 0 ) {
			return false;
		}
		else {
			$diff = time() - filemtime($file);
			$minutes = $diff / 60;
			if( $minutes >= $cache_lifetime ) {
				return true;
			}
		}
	}
	return false;
}

function drupento_cache_file_is_already_cached( $file ) {
	return file_exists($file);
}

function drupento_cache_node_by_nid( $nid,  $options = array() ) {
	try { 
		$cache = drupento_get_cache_file_path(constant('DRUPENTO_CACHE_TYPE_NODE'), "{$nid}");
		$node = node_load($nid);
		$title = $node->title;
		drupento_debug( 'Caching node ' . $title . '(' . $nid . ') using cache: ' . print_r($cache, 1), array('location' => __FUNCTION__) );		
		
		$node = serialize(node_load($nid));
		drupento_write_cache_file($cache, $node);

		return true;
	}
	catch( Exception $e ) {
		throw $e;
	}			
}

function drupento_get_nid_from_path($path) {
	$real_path = drupal_get_normal_path($path);
	$real_path_parts = explode('/', $real_path);
	
	foreach( $real_path_parts as $value ) {
		if($value == 'node') {
			$nid = next($real_path_parts);
		}	
	}
	
	if( !is_numeric($nid) ) {
		return false;
	} 
	
	return $nid;
}

function drupento_cache_uri_path_html( $options ){
	try { 
		
		$uri_path = $options['path'];
		
		//
		// get the file details
		//
		$cache = drupento_get_cache_file_path(constant('DRUPENTO_CACHE_TYPE_PATH'), $uri_path);

		// get the full html from the uri_path
		$html = drupento_get_html_from_uri_path( $uri_path );
		drupento_write_cache_file($cache, $html);

		return true;
	}
	catch( Exception $e ) {
		throw $e;
	}
}

function drupento_cache_element( $options ) {
	try {
		drupento_debug('Caching element with options: ' . print_r($options, 1), array('location' => __FUNCTION__));
		$selector = $options['selector'];
		$uri_path = $options['path'];
		
		//$standard_simplehtmldom_path = drupento_get_standard_simplehtmldom_path();
		
		if ( !($simplehtmldom = drupento_conf_get('simplehtmldom_path')) ) {
			$simplehtmldom = drupento_get_standard_simplehtmldom_path();
		}
		
		if( !file_exists($simplehtmldom) || !require_once($simplehtmldom) ) {
			throw new DrupentoException('SimpleHTMLDOM cannot be found. Ensure that it exists at ' . $simplehtmldom . ' or in the common \'sites/all/libraries\'');
		}
		
		//
		// NOTE: The full page must have already been cached for this to work
		//
		$cache = drupento_get_cache_file_path(constant('DRUPENTO_CACHE_TYPE_DOM_ELEMENT'), $uri_path, $selector);
	
		$path_cache = drupento_get_cache_file_path(constant('DRUPENTO_CACHE_TYPE_PATH'), $uri_path);
		$raw_markup = file_get_html( $path_cache['cache_file'] );
		//$raw_markup = str_get_html(drupento_get_html_from_uri_path( $uri_path ));				
		
		// NOTE: we are only caching the FIRST occurring element (eg if this is a class selector)
		// TODO: Add this to documentation

		$html = $raw_markup->find($selector, 0);
	
		// make sure we have content
		if(!$html) {
			// the selector may be invalid
			throw new DrupentoException('The caching of a selector (element) returned nothing. Your selector may be invalid: ' . $uri_path . ':' . $selector);
		}
	
		// TODO: separate this out
		// perform a replacement on the inner contents of the element
		if( isset($options['output_key']) && $options['output_key'] != '') {
			$innerhtml = $raw_markup->find("{$selector} *", 0);
			
			$replacement_output = trim($options['output_key']);
			if( substr($replacement_output, -1, 1) == ')' ) {
				// we have a function callback
				// TODO: might want to wrap this in a function_exists() call in the cached file,
				// or it could be done here if we force the config file to be in scope by requiring it
				$output = "<?php {$replacement_output};?>";
			}
			else {
				// we have a variable to insert
				$output = '<?php DrupentoRuntime::Show_variable(\'' . $replacement_output . '\') ?>';
			}
			
			drupento_debug('Replacing ' . $innerhtml . ' with ' . $output, array('location' => __FUNCTION__));
			
			// TODO: implement an option to choose between caching an element (active),
			// or the contents of the element (disabled)
			$html = $output;
			//$html = $raw_markup->find("{$selector}", 0)->innertext;
			//$html = str_replace($innerhtml, $output, $html);
		} 
	
	
		drupento_write_cache_file($cache, $html);
		
		return true;
	}
	catch( Exception $e ) {
		throw $e;
	}
}

function drupento_write_cache_file($cache, $html) {
	try {
		// write to a temp file first
		if ( !($handle = fopen($cache['tmp_file'], 'w+')) ) {
			throw new DrupentoException( 'fopen() failed when writing cache file: ' . $cache['tmp_file'] );
		}
		
		if( !fwrite($handle, $html) ) {
			throw new DrupentoException( 'fwrite() failed when writing cache file: ' . $cache['tmp_file'] );
		}
	
		// if that went well, move the cache file into place
		if ( !@rename($cache['tmp_file'], $cache['cache_file']) ) {
	
			// if we cant rename, delete the file if it already existed (this is needed on Windows,
			// because it cannot overwrite files with rename() ?
			@unlink( $cache['cache_file'] );
			if ( !copy($cache['tmp_file'], $cache['cache_file']) ) {
				throw new DrupentoException('There was an error moving a cache file into place' . $cache['cache_file'] );
			}
		}
		
		// remove the temp file, in case it still exists
		if( file_exists($cache['tmp_file'])) {
			unlink($cache['tmp_file']);
		}
		return true;
	}
	catch( Exception $e ) {
		throw $e;
	}
}

function drupento_get_standard_simplehtmldom_path() {
	
	$library_path = str_replace('modules' . DIRECTORY_SEPARATOR . 'drupento', 'libraries', realpath(dirname(__FILE__)));
	$standard_simplehtmldom_path = $library_path . DIRECTORY_SEPARATOR . 'simplehtmldom' . DIRECTORY_SEPARATOR . 'simple_html_dom.php';

	return $standard_simplehtmldom_path;
}

function drupento_debug($msg_or_exception, $options = array() ) {
	// TODO: currently, logging cannot be turned off.
	// There needs to be a setting for this, but the scopes in which 
	// this function is used makes accessing that setting difficult
	$message = drupento_get_log_message($msg_or_exception);
	
	$timestamp = date('Y-m-d h:i:s', time());
	
	$message = '[' . $timestamp . '][' . php_sapi_name() . ']: ' . $message;
	
	if ( isset($options['location']) && $options['location'] ) {
		$message .= ' Location: ' . $options['location'];
	}
	
	$logfile = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'drupento.log';
	$logfile = variable_get('drupento_log_file', $logfile);
	
	file_put_contents( $logfile, $message . "\n", FILE_APPEND );

}

function drupento_get_log_message( $msg_or_exception, $options = array() ) {
	if ( $msg_or_exception instanceof Exception ) {
		$e = $msg_or_exception;
		$message = $e->getMessage() . "\n" . $e->getTraceAsString(); 
	}
	else if(is_array($msg_or_exception)) {
		$message = "\n" . print_r($msg_or_exception, 1);
	}
	else {
		$message = $msg_or_exception;
	}

	return $message;
}	

function drupento_log_error( $msg_or_exception, $options = array() ) {
	$message = drupento_get_log_message($msg_or_exception);

	if ( !isset($options['type']) ) {
		$options['type'] = 'error';
	}

	return drupento_log_message($message);
}

function drupento_log_message( $message, $options = array() ) {
	try { 
		$type = ( isset($options['type']) && $options['type'] ) ? $options['type'] : 'drupento';
		
		//
		// Not sure why, but 'watchdog' by itself
		// ignores the dblog when in cli mode, 
		// so we call the dblog hook explicitly.
		//
		if ( php_sapi_name() == 'cli' ) {
			
			if ( function_exists('dblog_watchdog') ) {	
		      
				$log_setup = array(
					'type'        => $type,
					'message'     => $message,
					'variables'   => null,
					'severity'    => WATCHDOG_NOTICE,
					'link'        => null,
					'user'        => null,
					'request_uri' => null,
					'referer'     => null,
					'ip'          => ip_address(),
					'timestamp'   => time(),
				);

				dblog_watchdog($log_setup);
			}
			
		}
		else {
			if ( function_exists('watchdog') ) {
				watchdog( $type, $message);
			}
			else {
				// TODO:
				// custom log? what happens from the magento side?
			}
		}
	}
	catch( Exception $e ) {
		// Logging the error caused an error... we need to bail
		trigger_error( $e->getMessage() . "\n" . $e->getTraceAsString(), E_USER_ERROR );
		exit;
	}	
}
?>
