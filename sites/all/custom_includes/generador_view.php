<?php

//require_once $base_url .path_to_theme() ."/phpclasses/simplehtmldom/simple_html_dom.php";
class GeneradorView {
	
public $viewname;
public $resultado;
	
		
	function __construct($nombreView,$display="default",$args=NULL,$items=NULL){
		// constructor 
		// $G= new GeneradorView('noticias_home','default',array($term_id,3),4);
		$viewname = $nombreView;
		$this->viewname=$viewname;
		$view = views_get_view($viewname);
		
		$view->set_use_ajax(TRUE);

		
		if($args)$view->set_arguments($args);
		//echo "items ".$items;
		if($display) $view->set_display($display);
		
		if($items) {
			$view->display[$display]->display_options['items_per_page']=$items;
		}
		$view->display[$display]->handler->options['display_options']['use_pager'] = TRUE;
		
				
		$view->execute_display($display);
		$campos=$view->result;
		
			
				
		//$vars['scripts'] = drupal_get_js();
		
		$this->vista=$view;
		
		$filas=array();
		
		foreach ($campos as $num=>$result) {
		$myViewRow = new StdClass();
		$myViewRow->fields=array();
		
		foreach ($view->field as $id => $field) {
		$myViewRow->fields[$id] = $view->render_field($id,$num);
		}
		$filas[]=$myViewRow;
		}
		$this->resultado=$filas;
		
		/*$pager_theme = views_theme_functions($view->pager['use_pager'], $view, $view->display_handler->display);
		
    $this->custompager    = theme($pager_theme, $exposed_input, $view->pager['items_per_page'], $view->pager['element']);*/
	
	
	
		//return($filas);
	}

	function renderList($attr){
		// return $filas->renderList(array("ul"=>"noticiashome","li"=>"noticias_element","title"=>"cufon"));
		$ulstyle=$attr['ul']?$attr['ul']:"ul_list";
		$listyle=$attr['li']?$attr['li']:"li_element";
		
		$output="<ul class='$ulstyle'>";
		foreach($this->resultado as $fila){
			$output.="<li class='$listyle'>";
		
		foreach ($fila->fields as $id=>$campo){
			if($attr[$id]) $clase="class='$attr[$id]'";
			else $clase="";
			
			$output.="<div $clase id='$id'>";
			$output.=$campo;
			$output.="</div>";
			
		}
			
			$output.="</li>";
			}
		$output.="</ul>";
		$moo=theme('views_mini_pager');
		$output.=$moo;
		return $output;
		}
	
	
}
?>