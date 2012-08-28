<?php if (!function_exists("url")){
	
	function url($string){
		return "/es/".$string;
		}
	
	}
	
	$nav = clean_navigation(menu_tree_page_data('primary-links'));
?>
<ul id="nav">
<?php foreach($nav as $id=>$item){
	//krumo($item);
	//krumo($item['below'] );
	?>
    <li class="style-<?php echo $id+1; ?> withdrop">
    <a href="#"><span><?php echo $item['title']?></span></a>
     <ul class="drop" style="opacity: 0; top: 100%; display: none;"> 
    		<?php foreach($item['below'] as $ids=>$subitem):
		//krumo($subitem);
		?>
       <li><a href="<?php echo url($subitem['href'])?>"><?php echo $subitem['title']?></a></li>
        <?php endforeach; ?>
    </ul>
    </li>
    <?php
	}
	?>
</ul>
