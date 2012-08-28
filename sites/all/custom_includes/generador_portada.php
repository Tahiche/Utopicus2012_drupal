<?php

//require_once $base_url .path_to_theme() ."/phpclasses/simplehtmldom/simple_html_dom.php";
class GeneradorHome {
	

	
	var $iniciado=false;
	var $view;
	var $titulo;
	var $imagenProyecto;
	
	function init($nodo){
		
		
		$ranNum=rand(0,count($nodo->field_destacado_home)-1);
				
		$ranNodoArr=$nodo->field_destacado_home[$ranNum];
		
		//echo "<h1>--".$ranNodoArr['safe']['title']."</h1>";
		$this->titulo=$ranNodoArr['view'];
		
		//miKrumo($ranNodoArr);
		$nodeP = new stdClass();
		$nodeP->nid = $ranNodoArr['nid'];
		$nodeP->vid =  $ranNodoArr['nid'];
		//$nodeP->vid = db_result(db_query("SELECT vid FROM {node} WHERE nid = %d",4));    
		$nodeP->type = 'trabajo';    
		// content_storage no funciona en servidor !!. Usamos node_load
		/*$content = content_storage('load', $nodeP);
		
		$imgID=$content['field_imagen_proyecto'][0]['fid'];
		$eventImageArray = field_file_load($imgID);
		$eventImagePath = $eventImageArray['filepath'];
		
		$src = $image["filepath"];
		$dst = imagecache_create_path("imagen_portada_932x450", $eventImagePath);
		//echo "dst=".base_path().$dst;
		$this->imagenProyecto=$dst;
		
		$this->donde=$content['field_donde'][0]['value'];
		*/
		$content =node_load($ranNodoArr['nid']);
		//miKrumo($content);
		$src = $content->field_imagen_proyecto[0]["filepath"];
		$dst = imagecache_create_path("imagen_portada_932x450", $src);
		//echo "dst=".base_path().$dst;
		$this->imagenProyecto=$dst;
		
		$this->donde=$content->field_donde[0]['value'];
		// Ensure existing derivative or try to create it on the fly
		/*if (file_exists($dst) || imagecache_build_derivative($preset['actions'], $src, $dst)) {
		  $this->imagenProyecto=$dst;
		}*/
			$iniciado=true;
	}
	
}
?>