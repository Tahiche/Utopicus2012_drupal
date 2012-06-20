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
	
 $('select#edit-term-node-tid-depth-limited, select.selector-jump').change(function() {
	 // alert("cambia");
	 $('#edit-nombre').val("");
	 $('#edit-freeterm').val("");
	$('#edit-title').val("");
	 //$('form#views-exposed-form-coworkers-grid-page-coworkers').submit();
	$(this.form).submit()
 });

 
 
 
 
 /* Puts the currently highlighted suggestion into the autocomplete field.
 // OVERRIDEEENN
 */
 var valorNombre="";
 if(Drupal.jsAC){
		Drupal.jsAC.prototype.select = function (node) {
		
		valorNombre=$(node).data('autocompleteValue');
		
		this.input.value = $(node).data('autocompleteValue');
		
		
		
			if(jQuery(this.input).hasClass('auto_submit')){ 
			
			//alert("autosubmit???");
			  var selectElement=$('select#edit-term-node-tid-depth-limited');
			  selectElement.val($('option:first', selectElement).val());
			  
			  $('#edit-freeterm').val("");
			  
			 /* console.log($(node)[0].autocompleteValue);
			  console.log($(node)[0]['autocompleteValue']);*/
			  			  
			 $('#edit-nombre').val($(node)[0].autocompleteValue);
			  // form bonito...
			  // pero si hago trigger, el form se submit 2 veces....
			  //selectElement.trigger("change");
			  jcf.customForms.refreshAll();
			  //console.log("form,mmm");

					  
$('form#views-exposed-form-coworkers-grid-page-3, form#views-exposed-form-coworkers-grid-page-coworkers').get(0).submit();
			 //
			 
			// $('').submit();
			/* var forme=$(this.input).get(0).form;
			   forme.submit();*/ 

			 
		  }
		};
 }

//$('form#views-exposed-form-coworkers-grid-page-coworkers')
$('.view-filters form').submit(function(e){ 
  //alert("Submitted");
  if($('#edit-freeterm').val()!="")
  { var selectElement=$('select#edit-term-node-tid-depth-limited');
	  selectElement.val($('option:first', selectElement).val());
  }
  $('div.view-content').block().bind("ajaxComplete", function() {
            $(this).unblock();
			 
    });
			
    

  
})




}
