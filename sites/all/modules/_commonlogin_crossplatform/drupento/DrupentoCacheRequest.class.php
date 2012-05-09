<?php

class DrupentoCacheRequest {
	public $force_refresh = false;
	protected $_Callbacks = array();
	protected $_Nodes = array();
	protected $_Dom_elements = array();
	protected $_Paths = array();	
	protected $_Relevant_files = array();
	
	public function add_callback( $callback_name, $options = array() ) {
		$this->_Callbacks[] = array( 'name' => $callback_name );
	}
	
	public function add_dom_element( $path, $selector, $options = array() ) {
		$element['path'] = $path;
		$element['selector'] = $selector;
		$element['output_key'] = $options['output_key'];	

		$this->_Dom_elements[] = $element;
	}

	public function add_node( $nid_or_path, $options = array() ) {
		$this->_Nodes[] = array('nid_or_path' => $nid_or_path);
		
	}	
	
	public function add_path($path, $options = array() ) {
		$this->_Paths[] = array('path' => $path);		
	}
	
	/**
	 * 
	 * Sets up the object using an array of options
	 *  
 	 * $settings looks like:	
 	 * 'type' - one of 'path', 'node', 'callback', 'dom_element'
 	 * 'selector' - where relevant, a DOM element to be cached 
 	 * 'target' - might be a callback name, a node id, or a path
 	 * 'output_key' - a key for the variable replacement
 	 */
	public function apply_settings( $settings = array() ) {
		drupento_debug( "Settings to be applied: " . print_r($settings, 1), array('location' => __METHOD__) );
	
		if ( !isset($settings['type']) || !$settings['type'] ) {
			throw new DrupentoException( 'Missing cache type in settings' );
		}
		
		if ( !isset($settings['target']) || !$settings['target'] ) {
			throw new DrupentoException( 'Missing cache target in settings' );
		}

		$type = $settings['type'];
	
		switch( $type ) {
			case 'callback':
				$this->add_callback( $settings['target'] );	
				break;
			case 'node':
				$this->add_node( $settings['target'] );	
				break;
			case 'path':
				$this->add_path( $settings['target'] );
				break;
			case 'dom_element':
				
				if ( !isset($settings['selector']) || !$settings['selector'] ) {
					throw new DrupentoException( 'Missing selector for dom element cache request' );
				}
				
				drupento_debug( "Options passed to add_dom_element: " . print_r($options, 1), array('location' => __METHOD__) );
				// set the output var if present
				if( isset($settings['output_key']) && $settings['output_key']) {
					$options['output_key'] = $settings['output_key'];
				}
				
				$this->add_dom_element( $settings['target'], $settings['selector'], $options );
				break;
			default:
				throw new DrupentoException( 'Invalid type (' . $type . ') for cache request in ' . __METHOD__);				
			
		}
		
	}
	
	public function run( $options = array() ) {
		try { 
			drupento_debug( __METHOD__ . ' Called with options: ' . print_r($options, 1), array('location' => __METHOD__) );
	
			$this->_Relevant_files = array();
	
			if ( isset($options['force_refresh']) ) {
				$force_refresh = $options['force_refresh'];
			}
			else {
				$force_refresh = $this->force_refresh;
			}
	
			//
			// Cache whole paths
			//
			foreach( $this->_Paths as $path_info ) {
				
				// cache the whole page first
				drupento_debug( 'Found path request: ' . print_r($path_info, 1), array('location' => __METHOD__) );
				
				$cache_file_info = drupento_get_cache_file_path( constant('DRUPENTO_CACHE_TYPE_PATH'), $path_info['path'] );
				$cache_file = $cache_file_info['cache_file'];
				
				$this->_Relevant_files[] = $cache_file;
				
				if ( drupento_cache_file_is_stale($cache_file) || !file_exists($cache_file) || $force_refresh ) {
					drupento_debug( 'Re-caching stale or nonexistent file: ' . $cache_file, array('location' => __METHOD__) );
					drupento_cache_uri_path_html($path_info['path']);
				}
				else {
					drupento_debug( 'Cache file: ' . $cache_file . ' already exists and is fresh; no need to re-cache ', array('location' => __METHOD__) );	
				}
			}
				
			//
			// Cache DOM Elements
			//
			foreach( $this->_Dom_elements as $element ) {
				drupento_debug( 'Found DOM element Request: ' . print_r($element, 1), array('location' => __METHOD__) );
				
				//
				// cache the whole page first
				//
				$cache_file_info = drupento_get_cache_file_path( constant('DRUPENTO_CACHE_TYPE_PATH'), $element['path'] );
				$cache_file = $cache_file_info['cache_file'];
				
				// (don't add relevant_file here)
				
				if ( drupento_cache_file_is_stale($cache_file) || !file_exists($cache_file) || $force_refresh ) {
					drupento_cache_uri_path_html($element);
				}
				
				//
				// cache the element 
				//
				$cache_file_info = drupento_get_cache_file_path( constant('DRUPENTO_CACHE_TYPE_DOM_ELEMENT'), $element['path'], $element['selector'] );
				$cache_file = $cache_file_info['cache_file'];
				
				$this->_Relevant_files[] = $cache_file;
				
				if ( drupento_cache_file_is_stale($cache_file) || !file_exists($cache_file) || $force_refresh ) {
					drupento_debug( 'Re-caching stale or nonexistent file: ' . $cache_file, array('location' => __METHOD__) );
					drupento_cache_element($element);
				}
				else {
					drupento_debug( 'Cache file: ' . $cache_file . ' already exists and is fresh; no need to re-cache ', array('location' => __METHOD__) );
				}
			}
			//
			// Cache callbacks
			//
			foreach( $this->_Callbacks as $callback ) {
				drupento_debug( 'Found Callback Request: ' . print_r($callback, 1), array('location' => __METHOD__) );
				
				$cache_file_info = drupento_get_cache_file_path( constant('DRUPENTO_CACHE_TYPE_CALLBACK'), $callback['name'] );
				$cache_file = $cache_file_info['cache_file'];
				
				$this->_Relevant_files[] = $cache_file;
				
				if ( drupento_cache_file_is_stale($cache_file) || !file_exists($cache_file) || $force_refresh ) {
					drupento_debug( 'Re-caching stale or nonexistent callback file: ' . $cache_file, array('location' => __METHOD__) );
					drupento_cache_user_defined_function( $callback['name'] );
				}
				else {
					drupento_debug( 'Cache file: ' . $cache_file . ' already exists and is fresh; no need to re-cache ', array('location' => __METHOD__) );
				}
			}
		
			//
			// Cache Nodes
			//
			foreach( $this->_Nodes as $node_info ) {
	
				drupento_debug( 'Found Node request: ' . print_r($path_info, 1), array('location' => __METHOD__) );
	
				$nid_or_path = $node_info['nid_or_path'];
				
				if(!is_numeric($nid_or_path)){
					// the $nid might be a path...
					$node_path = $nid_or_path;
					$nid = drupento_get_nid_from_path($node_path);
				}
				else {
					$nid = $nid_or_path;
				}
					
				if(!$nid){

					throw new DrupentoException("Couldn't get node for nid or path: {$node_info['nid_or_path']}");
				}

				$cache_file_info = drupento_get_cache_file_path( constant('DRUPENTO_CACHE_TYPE_NODE'), $nid );
				$cache_file = $cache_file_info['cache_file'];
			
				$this->_Relevant_files[] = $cache_file;

				if ( drupento_cache_file_is_stale($cache_file) || !file_exists($cache_file) || $force_refresh ) {
					drupento_debug( 'Re-caching stale or nonexistent file: ' . $cache_file, array('location' => __METHOD__) );
					drupento_cache_node_by_nid($nid);
				}
				else {
					drupento_debug( 'Cache file: ' . $cache_file . ' already exists and is fresh; no need to re-cache ', array('location' => __METHOD__) );
				}
			}
		}
		catch( Exception $e ) {
			throw $e;
		} 
	}
	
	public function get_relevant_files() {
		return $this->_Relevant_files;
	}
}
?>
