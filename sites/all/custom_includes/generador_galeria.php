<?php

//require_once $base_url .path_to_theme() ."/phpclasses/simplehtmldom/simple_html_dom.php";
class GeneradorGaleria {
	
public $pics=array();

	
		
	function __construct($picsArray){
	foreach($picsArray as $id=>$pic){
	//echo $pic['filepath'];
	$this->pics[]=theme('imagecache','galleria_pic',$pic['filepath'],$pic['data']['alt'],$pic['data']['alt']);
		}
		
	}

	function pintar(){
		$output="<div id='galleria' class='images_galleria'>";
		
		foreach($this->pics as $pic){
		$output.=$pic;
		}
		$output.="</div>";
		$output.="<script>$('.images_galleria').galleria({	
		image_crop: false,
		
		transition: 'fade',
		
		autoplay: 5000,
		show_info:false,
		idle_time:2000
     
   		});</script>";
		return $output;
		}
	
	
}
?>