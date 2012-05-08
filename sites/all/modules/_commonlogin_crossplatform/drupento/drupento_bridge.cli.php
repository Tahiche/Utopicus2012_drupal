<?php 	

define('IN_DRUPENTO_CLI', 1);
define('EXIT_CODE_SUCCESS', 1);
define('EXIT_CODE_FAILURE', 2);

//
// argument trimming is necessary on windows
// because our values include the enclosing quotes otherwise
//
function _drupento_trim_cli_arg($arg) {
	
	if ( $arg ) {
		return trim($arg, '\'\"');
	}
	else {
		return null;
	}
	
}

function _drupento_cli_error_handler( $errno, $errstr, $errfile = null, $errline = null, $errcontext = null ) {
	
	drupento_debug( "PHP Error: errno: {$errno} errstr: {$errstr} errfile: {$errfile} errline: {$errline} errcontext: {$errcontext}" );
	
}	

try {

	//error_reporting(0);
	//ini_set('display_errors', 'Off');		
	$docroot = _drupento_trim_cli_arg($argv[1]);
	$type = _drupento_trim_cli_arg($argv[2]);
	$target = _drupento_trim_cli_arg($argv[3]);
	$selector = isset($argv[4]) ? _drupento_trim_cli_arg($argv[4]) : null;
	$output = isset($argv[5]) ? _drupento_trim_cli_arg($argv[5]) : null;
	
	require_once dirname(__FILE__) . DIRECTORY_SEPARATOR . 'drupento.inc';

	$old_error_handler = set_error_handler( '_drupento_cli_error_handler' );
	drupento_debug( "CLI called with options: docroot: {$docroot} type: {$type} target: {$target} selector: {$selector} output: {$output}" );

	if ( !$docroot || !is_dir($docroot) ) {
		throw new DrupentoException("Docroot {$docroot} does not exist");
	}
	
	chdir($docroot);
	
	require_once 'includes' . DIRECTORY_SEPARATOR . 'bootstrap.inc';
	
	drupal_bootstrap(constant('DRUPAL_BOOTSTRAP_FULL'));
	
	if( !$type ) {
		throw new DrupentoException( 'Missing type argument in Drupento CLI call' ); 
	}

	if( !$target ) {
		throw new DrupentoException( 'Missing target argument in Drupento CLI call' ); 
	}

	$cache_request = new DrupentoCacheRequest();
	
	$request_arr = array();
	$request_arr['type'] = $type;
	$request_arr['target'] = $target;
	$request_arr['selector'] = $selector;
	$request_arr['output_key'] = $output;

	$cache_request->apply_settings($request_arr);
	
	drupento_debug( "About to run cache request from CLI " );
	$cache_request->run();

	set_error_handler( $old_error_handler );
	exit(EXIT_CODE_SUCCESS);
}
catch( Exception $e ) {
	drupento_log_error( $e );
	drupento_debug( $e );
	set_error_handler( $old_errror_handler );
	exit(constant('EXIT_CODE_FAILURE'));
}
?>
