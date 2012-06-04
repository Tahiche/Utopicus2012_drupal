  Drupal.behaviors.side_bar_ajax = function() {
  // alert("side_bar_ajax");
  
   var form=$(".view-filters form.views-processed");
   //console.log(form);
   
    $(".term_ajaxlink a").click(function(event){
		event.preventDefault();
		
		//var termino=$(this).attr('href').split('/').pop();
		var termino=$(this).attr('title')!== undefined ?$(this).attr('title'):$(this).text();
		//console.log($(this).attr('title'));
		var elementinputname=$(this).attr('element');
		
		var input=$('input[name="'+elementinputname+'"]', form);
		input.val(termino);
		//console.log(input);
		form.submit();
//href.split('/').pop();
       /* $('#edit-nombre').val("");
	 $('form#views-exposed-form-coworkers-grid-page-coworkers').submit();*/
	 
	 
		/*console.log(form);*/
		});
		
		
		
	
	form.submit(function(e){
    //alert("Submitted");
	var elementBlock= $("#content","div.view").length>0 ? $("#content","div.view").eq(0):$('div.view-content');
	//console.log(elementBlock);
  elementBlock.block().bind("ajaxComplete", function() {
            $(this).unblock();
			 
    }); })
	
	
  };

