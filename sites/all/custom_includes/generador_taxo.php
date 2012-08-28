<?php

//require_once $base_url .path_to_theme() ."/phpclasses/simplehtmldom/simple_html_dom.php";
class GeneradorTaxo {
	
public $listaTerminos;
	
		
	function getVocabulary($numVocab){
	// return $filas->renderList(array("ul"=>"noticiashome","li"=>"noticias_element","title"=>"cufon"));
	$links=taxonomy_get_tree($numVocab); 
	$this->listaTerminos=$links;
	return $this->listaTerminos;
	}
	
	function vocabSlug($numVocab,$attr){
	// por ej:
	// $tax=new GeneradorTaxo();
	// echo $tax->vocabSlug(1,array("span"=>"taxclass","separador"=>" , "))
	$spanstyle=$attr['span']?$attr['span']:"taxo_elem";
	$a_clase=$attr['a']?$attr['a']:"a_taxo";
	$separador=$attr['separador']?$attr['separador']:" - ";
	$output="";
		
	$links=$this->getVocabulary($numVocab);
	
	foreach($links as $item):
	$link=drupal_get_path_alias(taxonomy_term_path($item) );
	
	if(drupal_get_path_alias($_GET['q'])==$link) $a_clase.=" activelink";
	
	$output.="<span class='$spanstyle'>";
	$output.=l($item->name,"$link",array('attributes' => array('class' => $a_clase))  );
	$output.="</span>$separador";
	endforeach;
	
	
	
	return $output;
		}
	
	
	
	
	function vocabList($numVocab,$attr){
	
	$ulstyle=$attr['ul']?$attr['ul']:"ul_list";
	$listyle=$attr['li']?$attr['li']:"li_element";
	$a_clase=$attr['a']?$attr['a']:"link";
	
	$output="<ul class='$ulstyle'>";
	
	$links=$this->getVocabulary($numVocab);
	
	foreach($links as $item):
	$link=drupal_get_path_alias(taxonomy_term_path($item) );

	$active="";
	if(drupal_get_path_alias($_GET['q'])==$link) {
		$active=" activelink";
		//echo $link;
		
	}
	
	
	$output.="<li class='$listyle'>";
	$output.=l($item->name,"$link",array('attributes' => array('class' => $a_clase." ".$active))  );
	$output.="</li>";
	endforeach;
	
	$output.="</ul>";
	
	return $output;
		}
		
	function nodeTermsSlug($termArr,$attr){
	// por ej:
	// $tax=new GeneradorTaxo();
	// echo $tax->vocabSlug(1,array("span"=>"taxclass","separador"=>" , "))
	$spanstyle=$attr['span']?$attr['span']:"taxo_elem";
	$a_clase=$attr['a']?$attr['a']:"a_taxo";
	$separador=$attr['separador']?$attr['separador']:" - ";
	$output="";
		
	$color= (boolean)$attr['colores'];
		
	foreach($termArr as $item):
	if($color){
		$i++;
		$spanstyle.=" color_taxo_safe color_taxo$i";
		$a_clase.=" color_taxo_safe color_taxo$i";
		}
	$link=drupal_get_path_alias(taxonomy_term_path($item) );
	
	if(drupal_get_path_alias($_GET['q'])==$link) $clase.=" activelink";
	
	$output.="<span class='$spanstyle'>";
	$output.=l($item->name,"$link",array('attributes' => array('class' => $a_clase))  );
	$output.="</span>$separador";
	endforeach;
	
	
	
	return $output;
		}
	
	
	
}
?>