<?php 

require_once dirname(__FILE__) . DIRECTORY_SEPARATOR . 'drupento.inc';
require_once dirname(__FILE__) . DIRECTORY_SEPARATOR . 'drupento_auth.module';
define('IN_DRUPENTO_CLI', 1);
define('EXIT_CODE_SUCCESS', 1);
define('EXIT_CODE_FAILURE', 2);

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

function init() {
	global $argv;

	$docroot = _drupento_trim_cli_arg($argv[1]);	

	if ( !$docroot || !is_dir($docroot) ) {
		throw new DrupentoException("Docroot {$docroot} does not exist");
	}

	// change to the drupal docroot and bootstrap
	chdir($docroot);
	require_once 'includes' . DIRECTORY_SEPARATOR . 'bootstrap.inc';
	drupal_bootstrap(constant('DRUPAL_BOOTSTRAP_FULL'));	
	
	// SECURITY - do not allow this script to be run from cgi/apache
	if( php_sapi_name() !== 'cli' ) {
		throw new DrupentoException("Attempted to cross the auth bridge outside of CLI");	
	}
}

function drupento_exit($_STATUS_CODE) {
	// reset the error handler and exit with success
	set_error_handler("{$old_error_handler}");
	exit(constant($_STATUS_CODE));	
}

try {
	init();

	// manage the error handler - this lets us log to the drupento log properly
	$old_error_handler = set_error_handler('_drupento_cli_error_handler');
	
	$op = _drupento_trim_cli_arg($argv[2]);
	$email = _drupento_trim_cli_arg($argv[3]);
	$password = _drupento_trim_cli_arg($argv[4]);
	$old_email = _drupento_trim_cli_arg($argv[5]);
		
	switch( $op ) {
		case "delete":
			$del_user = user_load(array('mail' => $email));
			if( $del_user && $del_user->uid > 1 ) {
				user_delete(array(), $del_user->uid);
				drupento_exit('EXIT_CODE_SUCCESS');
			}
			break;
			
		case "login":
			global $user;
			drupento_debug( "About to authenticate from CLI {$email}" );
			user_external_login_register($email, 'drupento_auth');
			
			if( $user->uid ) {
				// if drupento_auth is not yet responsible for auth, insert a new auth mapping for this user
				if( !drupento_auth_is_responsible_for_authenticating( $email ) ) {
					db_result(db_query('INSERT INTO {authmap} VALUES(NULL, %d, "%s", "drupento_auth")', $user->uid, $email));
				}
			
				// save the email
				$update = db_query('UPDATE {users} SET mail = "%s", pass = "%s" WHERE uid = %d ', $email, md5($password), $user->uid);		
				session_write_close();
				
				// echo the key to allow magento to begin the session
				$key = db_result(db_query('SELECT sid from {sessions} WHERE uid = %d ORDER BY timestamp DESC LIMIT 1', $user->uid));
				echo $key;
				
				// log the new session in drupal
				drupento_log_message('Session opened for ' . $user->mail);
				drupento_exit('EXIT_CODE_SUCCESS');
			}
			else {
				drupento_exit('EXIT_CODE_FAILURE');
			}
			
			// Failsafe
			drupento_exit('EXIT_CODE_FAILURE');
			break;
			
		case "update":
			if( $password && $password != '' ) {
				echo "updating everything";
				$update = db_query('UPDATE {users} SET mail = "%s", name = "%s", pass = "%s" WHERE name = "%s"', $email, $email, md5($password), $old_email);
			}
			else {			
				echo "updating email only";
				$update = db_query('UPDATE {users} SET mail = "%s", name = "%s" WHERE name = "%s"', $email, $email, $old_email);
			}
			
			$update = db_query('UPDATE {authmap} SET authname = "%s" WHERE authname = "%s"', $email, $old_email);

			drupento_exit('EXIT_CODE_SUCCESS');
			break;		
	}
}
catch( Exception $e ) {
	drupento_log_error($e);
	drupento_debug($e);
	drupento_exit('EXIT_CODE_FAILURE');
}
?>
