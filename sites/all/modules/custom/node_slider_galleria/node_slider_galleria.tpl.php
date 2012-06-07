<?php 
// miKrumo($pics);
/*miKrumo($image_preset);*/

if(!$image_preset) $image_preset="Imagen_pag_922x394";

// solo hay una foto... 
 if(count($pics) ==1):
 echo theme('imagecache',$image_preset,$pics[0]['filepath'],$pics[0]['data']['title'],$pics[0]['data']['title']);
 ?>

<?php else:?>
 
<div id='galleria' class='images_galleria'>
<?php 


		foreach($pics as $pic){
			// pic_45x45
		$largepic_path=imagecache_create_url($image_preset, $pic['filepath'], $bypass_browser_cache = FALSE);
echo '<a href="'.$largepic_path.'">'.theme('imagecache','slider_thumbnail',$pic['filepath'],$pic['data']['title'],$pic['data']['title']).'</a>';
        //echo theme('imagecache',$image_preset,$pic['filepath'],$pic['data']['title'],$pic['data']['title']);
		}?>
</div>
 <link id="galleria-theme" rel="stylesheet" href="/<?php print drupal_get_path('module', 'node_slider_galleria')?>/themes/fullscreen/galleria.fullscreen.css">
 <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.4/jquery.min.js"></script>



<script src="/<?php print drupal_get_path('module', 'node_slider_galleria')?>/galleria.js"></script>


    <script>Galleria.loadTheme("/<?php print drupal_get_path('module', 'node_slider_galleria')?>/themes/fullscreen/galleria.fullscreen.js");</script>
           
      <script type="text/javascript">
 	var $jq144 = jQuery.noConflict(true);</script>     
               
		<script>
	// Code that uses other library's $ can follow here.
	$jq144('.images_galleria').galleria({	
		image_crop: false,
		transition: 'fade',
		autoplay: 5000,
		show_info:false,
		idle_time:2000
     
   		});</script>
 <?php endif;?>
        