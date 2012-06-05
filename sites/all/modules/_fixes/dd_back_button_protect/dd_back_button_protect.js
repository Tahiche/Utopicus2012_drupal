$(window).load(function(){	
  //Necessary because webkit browsers do not load document.ready when the back button is clicked.
  
	jQuery.getJSON('/dd_back_button_protect/' + $("#node-form").find('input[name="form_build_id"]').val(), function(json, textStatus) {
    if (json.staleForm) {
      // Hide the form so the user doesn't try to work on it while it reloads
      $("#node-form").hide();
      // @TODO: Need to add this message properly per Drupal coding standards
      $("#content-header").append('<div class="messages warning">This post has been updated. Loading latest version...</div>');
      //Unset any pop-up dialogs by other modules
      window.onbeforeunload = function() {};
      //Reload the form
      window.location.reload(true);
    }
  });
	
});