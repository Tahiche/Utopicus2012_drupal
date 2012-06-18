Drupal.behaviors.LoginToboggan = function (context) {
	//console.log("LoginToboggan");
  $("#toboggan-login:not(.toboggan-login-processed)", context).each(
    function() {
	 console.log($(this));
      $(this).addClass('toboggan-login-processed').hide();
      Drupal.logintoboggan_toggleboggan();
    }
  );
  
  
  
 // Drupal.logintoboggan_toggleboggan();
};

Drupal.logintoboggan_toggleboggan = function() {
//console.log("logintoboggan_toggleboggan");

  $("#toboggan-login-link").click(
    function () {
      $("#toboggan-login").slideToggle("fast");
      this.blur();
      return false;
    }
  );
  
   
  
  
};