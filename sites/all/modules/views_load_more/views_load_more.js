/**
 * An ajax responder that accepts a packet of JSON data and acts appropriately.
 *
 * The following fields control behavior.
 * - 'display': Display the associated data in the view area.
 */
Drupal.viewsLoadMore = function(target, response) {
	
	/*$("ul.load-more-pager li.pager-next").each(function()
                  {
                      if($(this).children("a").length == 0)
                      {
                          $(this).hide();
                      }
                  });*/


  // Find the current View
  var $view = $(target);
  // How will we identify the current pager in the View?
  var pager_class = '.pager';
  // Collect some information about our View's style plugin settings.
  var settings = Drupal.settings.views_load_more;
  var style_plugin = settings.style_plugin || 'default';
  var view_content_class = settings.css_class || '.view-content';
  var view_row_class = settings.row_class || '.views-row';
  // Check the 'display' for data.
  if (response.status && response.display) {
	  
	 // console.log(" response.status  "+response.status);
    // Grab the new contents.
    var $newView = $(response.display);
	
	
	//console.log($newView);
	
    // Find the new pager element, if it exists within the new contents.
    // If it doesn't exist, we've reached the last page, and this will be empty.
    var $pager = $newView.find(pager_class);
    // Normally views would replace here, but we're going to
    // append instead. At the same time, we want to update
    // the pager to the new pager and re-attached the proper
    // behaviors for the view.
    $view.removeClass('views-processed');
    // Update the Pager with the new pager.
    $view.find(pager_class).html($pager.html());
    // Append the new content to the old view.
    // If we are using a grid or table-driven style plugin,
    // we'll have to check if a tbody container exists.
	
	var $listRows= $(view_row_class);
	
	console.log($listRows);
	
	// no queremos el adjunto !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   console.log("view_row_class");
    console.log(view_row_class);
	
	if($newView.find('.view-content')){
		$newView=$newView.find('.view-content');
		//console.log("había content !!!!!!!!!!!!!!!");
		}
		
    var newRows = $newView.find(view_row_class).not(".adjunto");
	//var newRows = $newView.find(view_row_class);
	
	
	
	//console.log(newRows);
	
	//newRows=newRows.not(".views-row");
	//console.log(newRows);
    // Default tables in views 6.x do not include .views-row on the row class.
    // Therefore, jQuery has trouble identifying rows.
    // If the first query didn't find any results, check if the style plugin uses a table.
    if (newRows.length < 1 && (style_plugin == 'table' || style_plugin == 'grid')) {
      newRows = $('tbody tr', $newView);
    }
	// list,default
	/***********************************************************************************************************************************************/
	if (newRows.length < 1 && (style_plugin == 'list,default' || style_plugin == 'list' || style_plugin == 'default')) {
     // console.log("estamos en if");
	  newRows = $('.grid-div .grid-list', $newView);
    }
	// list,default
	/***********************************************************************************************************************************************/
	

	/*****************************************************************************************/
    if (newRows.length > 0) {
      // Drupal 6 ships with jQuery 1.2.6, making this a little harder
      // than it could be. There is no .parentsUntil() function.
      var path_to_parent = $(newRows)
        .parents()
        .map(function(index, domElement) {
          var value = domElement.tagName.toLowerCase();
          if (domElement.className) {
            value += '.' + domElement.className.replace(' ', '.');
          }
          return value;
        })
        .get().reverse().join(' ');
      // Index 0 will be everything from <html> to the view.
      // Index 1 will be everything from the view's contents down.
      path_to_parent = path_to_parent.split(view_content_class);
	  
	  
	  
      path_to_parent = view_content_class + ' ' + path_to_parent[1];
	  // el original!!!!! 
      //$(path_to_parent, $view).append(newRows);
	  
	  // lo cambio par alos attachment !!!!!
	  //ppio cambio
	 //if($(".view-content .post-columns", $view)){
	 // $(".view-content .post-columns", $view).append(newRows);
	var obj=  $(".view-content .post-columns", $view);
	//}
	
	if(  $(".view-content .post-columns ul.events-list", $view) ) {
		var obj= $(".view-content .post-columns ul.events-list", $view) ;
	}
	
	  console.log(obj);
	  console.log(obj.length);
	  // $(view_content_class).parent().append(newRows);
	  
	   
	  
		 
	 if (obj.length > 0) obj.append(newRows);
	 
	  else{
		  $(path_to_parent, $view).append(newRows);
		  }
	// fin cambio
	
	 // console.log( $(path_to_parent, $view));
	  
	  /****************************************************************** efecto IN ************************/
	  newRows.css("display", "none").slideDown(1500);
    }
    Drupal.attachBehaviors($view);
  }

  if (response.messages) {
    // Show any messages (but first remove old ones, if there are any).
    $view.find('.views-messages').remove().end().prepend(response.messages);
  }
  
};

