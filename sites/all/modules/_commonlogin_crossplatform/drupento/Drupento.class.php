<?php

class Drupento {
	
	public static function Node_load( $nid_or_path ) {

		if(!is_numeric($nid_or_path)){
		
			// TODO: this definitely doesnt work
			if( $nid = drupento_get_nid_from_path($nid_or_path) ){
				// cant determine a nid, so return null
				return null;
			}
		}
		else {
			$nid = $nid_or_path;
		}
		
		return self::Get_node( $nid );
	}
	
	public static function Get_node( $nid ) {
		
		$file = drupento_get_cache_file_path(constant('DRUPENTO_CACHE_TYPE_NODE'), $nid);
		$cache_file = $file['cache_file'];	
		
		try {
			
			if( !file_exists($cache_file) ) {
				
				$request_arr = array();
				$request_arr['type'] = 'node';
				$request_arr['target'] = $nid;
				
				$relevant_files = self::Cache($request_arr);
			
				if( is_array($relevant_files) ) {
					foreach($relevant_files as $file) {
						$serialized_obj = file_get_contents($cache_file);
						$node = unserialize($serialized_obj);
						return $node;
					}
				}
				else {
					throw new DrupentoException('No relevant files found for node cache');
				}
			}
			else{
				$serialized_obj = file_get_contents($cache_file);
				$node = unserialize($serialized_obj);
				return $node;
			}
		} 
		catch (Exception $e) {
			throw $e;
		}
		return null;
	}
	
	public static function Output( $type_or_target, $selector = null, $options = array() ) {
		
		try {
			
			// set the output var if present
			$output = null;
			if( isset($options['output_key']) && $options['output_key']) {
				$output = $options['output_key'];
			}
		
			drupento_debug("[STARTING REQUEST]: type_or_target: " . $type_or_target . ' selector: ' . $selector . ' cache vars: ' . print_r($output['vars'],1), array('location' => __METHOD__) );			
				
			//
			// set up the output parameter if a cache variable exists
			//
			$output = null;
			$output_key = null;
			if( isset($options['vars']) && count($options['vars']) ) {
				// by design, we only accept a single cache variable to be output per element
				foreach($options['vars'] as $name => $value) {
					$options['output_key'] = $name;
					break;
				}
				drupento_debug('Cache var in use: name = ' . $options['output_key'], array('location' => __METHOD__) );
				DrupentoRuntime::Set_variables( $options['vars'] );
			}					
	
			//
			// set up the cache request based on input
			//
			$cache_settings = self::Cache_request_settings_by_output_params($type_or_target, $selector, $options);		
			drupento_debug('Cache settings array: ' . print_r($cache_settings, 1), array('location' => __METHOD__));			

			$file = drupento_get_cache_file_path_by_request_settings($cache_settings);
			self::Render_cache_file( $file['cache_file'], $type_or_target, $selector, $options );
			
			drupento_debug("[FINISHING REQUEST]: type_or_target: " . $type_or_target . ' selector: ' . $selector . ' cache_file: ' . print_r($file, 1), array('location' => __METHOD__) );	
		}
		catch( DrupentoException $e ) {
			throw $e;
		}
		catch( Exception $e ) {
			throw $e;
		}
	}
		
	
	public static function Render_cache_file( $cache_file, $type_or_target, $selector = null, $options = array() ) {

		try {
			if( drupento_cache_file_is_stale($cache_file) || !file_exists($cache_file) ) {
								

				
				$cache_settings = self::Cache_request_settings_by_output_params($type_or_target, $selector, $options);
				drupento_debug('Caching with settings: ' . print_r($cache_settings, 1), array('location' => __METHOD__) );							

				self::Cache($cache_settings);
			}			

			if ( !is_readable($cache_file) ) {
				throw new DrupentoException( "Could not read cache file: {$cache_file}");
			}
			else {
				include($cache_file);
				return true;
			}
			
		} 
		catch (Exception $e) {
			throw $e;
		}
		
	}
	
	/**
	 *
	 * For ease of use, we allow Output() to be called with only two parameters, 
	 * even though the first parameter is sometimes a cache type (e.g. 'callback'), 
	 * and other times it is a uri path (e.g. 'home'). This function figures out what the user meant
	 * and generates a proper cache_settings array from the parameters
	 * 
	 * @param $type_or_target
	 * @param $selector	 
	 * @param $output
	 * @return cache settings array, ready to be passed to DrupalCacheRequest::apply_settings
	 */
	public static function Cache_request_settings_by_output_params( $type_or_target, $selector = null, $options = array() ) {
		
		$request_settings = array();
		
		// set the output var if present
		$output = null;
		if( isset($options['output_key']) && $options['output_key']) {
			$output = $options['output_key'];
		}
		
		switch( $type_or_target ) {
			case 'callback':
				$request_settings['type'] = constant('DRUPENTO_CACHE_TYPE_CALLBACK');
				$request_settings['target'] = $selector;
				break;
			case 'node':
				$request_settings['type'] = constant('DRUPENTO_CACHE_TYPE_NODE');
				$request_settings['target'] = $selector;
				break;
			default:
				if ( $selector ) {
					$request_settings['type'] = constant('DRUPENTO_CACHE_TYPE_DOM_ELEMENT');
					$request_settings['target'] = $type_or_target;
					$request_settings['selector'] = $selector;
					
					if( $output ) {
						$request_settings['output_key'] = $output;
					}
				}
				else {
					$request_settings['type'] = constant('DRUPENTO_CACHE_TYPE_PATH');;
					$request_settings['target'] = $type_or_target;	
				}
				
		}
		return $request_settings;
	}
	
	public static function Cache( $request_settings = array() ) {
	
		try { 
			
			if ( defined('IN_DRUPENTO_CLI') && constant('IN_DRUPENTO_CLI') >0 ) {
				//
				// If we're already inside a Drupento cli shell,
				// No need to try shelling again.
				//
				drupento_debug( 'Already in CLI; doing direct cache', array('location' => __METHOD__) );
				
				$cache_request = new DrupentoCacheRequest();
				$cache_request->apply_settings($request_settings);
				$cache_request->run();
				return $cache_request->get_relevant_files();
			}
			else {			

				// run the shell script
				
				$drupal_base_path = drupento_conf_get('drupal_base_path');
				
				if ( !$drupal_base_path || !is_dir($drupal_base_path) ) {
					throw new DrupentoException("Drupal base path {$drupal_base_path} does not exist or is incorrectly set");
				}
				
				$cli_cache_script = drupento_conf_get('module_path') . DIRECTORY_SEPARATOR . 'drupento_bridge.cli.php';
		
				
				$php = drupento_conf_get('php_cli_binary_path', 'php');
				
				$cmd = escapeshellarg($php) 
						. ' ' . escapeshellarg($cli_cache_script)
						. ' ' . escapeshellarg($drupal_base_path) 
						. ' ' . escapeshellarg($request_settings['type'])
						. ' ' . escapeshellarg($request_settings['target']);
				
				// add the selector to the cli args
				if ( isset($request_settings['selector']) && $request_settings['selector'] ) {
					$cmd .= ' ' . escapeshellarg($request_settings['selector']);
				}
				
				// add the output var to the cli args
				if ( isset($request_settings['output_key']) && $request_settings['output_key'] ) {
					$cmd .= ' ' . escapeshellarg($request_settings['output_key']);
				}

				//
				// On windows, we need to surround the entire command in quotes for some reason
				//
				if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
					$cmd = '"' . $cmd . '"';
				}
				
				$output = array();
				
				drupento_debug( 'Running CLI command: ' . $cmd, array('location' => __METHOD__) );
				
				exec($cmd, $output, $cache_status);
				
				$cache_file_info = drupento_get_cache_file_path_by_request_settings($request_settings);
				$cache_file = $cache_file_info['cache_file'];
				
				drupento_debug( 'Checking for successful cache of file: ' . $cache_file, array('location' => __METHOD__) );
				if( $cache_status !== 1 || !file_exists($cache_file) ) {
					throw new DrupentoException("The cache file {$cache_file} could not be generated. Check the Drupento configuration options and file permissions. EXIT CODE: ({$cache_status})\nCommand: $cmd\nOutput:" . print_r($output, 1)) ;
				}
				
				return array( $cache_file );
				
			}
		}
		catch( Exception $e ) {
			throw $e;
		}
	}
	
	public static function Api_get( $call, $args = array(), $debug = false ) {
		
		try {
			// we always pass the session id, to support authentication and cart api calls
			$session_id = drupento_get_session_id();
			
			$client = new SoapClient(drupento_conf_get('api_url') . '&SID=' . $session_id, array('trace' => 1));
			$session = $client->login( variable_get('drupento_api_user', ''), variable_get('drupento_api_password', ''));

			$result = $client->call($session, $call, $args);
		
			return $result;
		}
		catch( Exception $e ) {
			throw $e;
		}
		
	}
	
	/**
	 * Alias of Api_get
	 */
	public static function Call( $call, $args = array(), $debug = false ) {
		return self::Api_get( $call, $args, $debug );
	}
}

class DrupentoRuntime {
	
	static $Cache_vars = array();
	
	public static function Show_variable( $name ) {
		require_once(drupento_conf_get('callback_config_file'));
		
		$var = self::Get_variable($name);
		
		if( substr($var, -1, 1) == ')' ) {
			print eval($var . ';');
		}
		else {
			print $var;
		}
	}

	public static function Get_variable( $name ) {
		if( isset(self::$Cache_vars[$name]) ) {
			return self::$Cache_vars[$name];	
		}	
	}
	
	public static function Set_variable( $name, $value ) {	
		self::$Cache_vars[$name] = $value;	
	}
	
	public static function Set_variables( $variables = array() ) {
		foreach($variables as $name => $value ) {
			self::Set_variable( $name, $value);
		}
	}
	
}

?>
