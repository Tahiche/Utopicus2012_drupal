<?php 

class DrupentoException extends Exception {}

class DrupentoUserSettingException extends DrupentoException {	
	public function __construct( $message, $code, $previous ) {
	
		if ( function_exists('drupento_debug') ) {
			drupento_debug( $message );
		}

		if ( function_exists('drupento_log_error') ) {
			drupento_log_error( $message );
		}

		$message .= ' - Check your Drupento settings'; 
		return parent::__construct($message, $code, $previous);
	
	}
}
?>
