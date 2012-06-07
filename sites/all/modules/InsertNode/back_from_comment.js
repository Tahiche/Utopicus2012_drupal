/* Code first created by Zewa,
 * fixed up by AlexisWilke so it works with any type of page
 */
/**
* Reply, Edit & Delete buttons for included comments.
*
* Store where the user was when clicking add comment or reply,
* and bring him or her back there afterwards.
*/
function insert_node_destination(link){
  var attr=jQuery(link).attr('href');
  var p=attr.indexOf('#');
  var anchor='';
  if(p!=-1){
    anchor=attr.substr(p);
    attr=attr.substr(0,p);
  }
  attr+=attr.indexOf('?')==-1?'?':'&';
  url=location.pathname.substring(1);
  jQuery(link).attr('href',attr+'destination='+url+anchor);
}
jQuery(document).ready(function() {
  jQuery("a[href*='comment/reply'],a[href*='comment/edit'],a[href*='comment/delete']").click(function(){insert_node_destination(this);});
});
