/*
    $.fn.clearForm = function() {
      return this.each(function() {
        var type = this.type, tag = this.tagName.toLowerCase();
        if (tag == 'form')
          return $(':input',this).clearForm();
        if (type == 'text' || type == 'password' || tag == 'textarea')
          this.value = '';
        else if (type == 'checkbox' || type == 'radio')
          this.checked = false;
        else if (tag == 'select')
          this.selectedIndex = -1;
      });
    };

*/

// JavaScript Document
Drupal.behaviors.exposedfilter_mod = function (context) {
 $('select#edit-term-node-tid-depth-limited').change(function() {
	 // alert("cambia");
	 $('#edit-nombre').val("");
	 $('form#views-exposed-form-coworkers-grid-page-coworkers').submit();
 });
 /* Puts the currently highlighted suggestion into the autocomplete field.
 // OVERRIDEEENN
 */
Drupal.jsAC.prototype.select = function (node) {
this.input.value = $(node).data('autocompleteValue');
  
  if(jQuery(this.input).hasClass('auto_submit')){ 
	  var selectElement=$('select#edit-term-node-tid-depth-limited');
	  selectElement.val($('option:first', selectElement).val());
	  // form bonito...
	  selectElement.onChange();
	  // submit the form
     $('form#views-exposed-form-coworkers-grid-page-coworkers').submit();
	 
  }
};

$('form#views-exposed-form-coworkers-grid-page-coworkers').submit(function(e){
  //alert("Submitted");
  $('div.view-content').block({ 
            message: '<h1>Processing</h1>', 
            css: { 
		    border: 'none', 
            padding: '15px', 
            backgroundColor: '#000', 
            '-webkit-border-radius': '10px', 
            '-moz-border-radius': '10px', 
            opacity: .5, 
            color: '#fff'
			} 
            }).bind("ajaxComplete", function() {
            $(this).unblock();
			 
    });
			
    

  
})




}
