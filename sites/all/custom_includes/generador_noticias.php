<?php

//require_once $base_url .path_to_theme() ."/phpclasses/simplehtmldom/simple_html_dom.php";
class GeneradorNoticias {
	
	var $iniciado=false;
	var $view;
	var $titulo;
	var $imagenProyecto;
	var $contenido;
	
	var $output;
	
	
		
	function init(){
		$filas=new GeneradorView('noticias_home','default',NULL,3);
		return $filas->renderList(array("ul"=>"noticiashome","li"=>"noticias_element"));
	}
		
}
?>