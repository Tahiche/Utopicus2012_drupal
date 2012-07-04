<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/all.js#xfbml=1&appId=137450526276416";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>

<?php
global $theme_pathabsoluto;
// $theme_path -> var global definida en page-header
?>
<div id="footer">
		<h2>ESPACIOS UTOPIC_US</h2>
		<!-- footer-area -->
		<div class="footer-area">
			<!-- box -->
			<div class="box">
            
<?php 
$imgpath=path_to_theme()."/images/fotospieweb/_us1.jpg";
//krumo(imagecache_presets());
$elemento=array(
'fid'=>"001",
'nid'=>"001",
'filepath'=>$imgpath,
'data'=>array('alt'=>"madrid_US_1",'title'=>"madrid_US_1")
);

$opciones=array("customlink"=>"espacios/madrid_US_1");
//print "XXXXX".$imgpath.theme('imageformatters_grayover',$elemento,"FooterImg_210x85" , $opciones );
?>

				<?php echo theme('imageformatters_grayover',$elemento,"FooterImg_210x85" , $opciones ); ?>
				<strong class="title">Madrid Espacio_US1</strong>
				<address style="height: 38px;">C/ Concepción Jerónima, 22<br>28012 Madrid.</address>
				<dl>
					<dt>Tel:</dt>
					<dd>+34 913 663 252</dd>
				</dl>
			</div>
			<!-- box -->
			<div class="box">
            

<?php 
$imgpath=path_to_theme()."/images/fotospieweb/IMG_2956.JPG";
//krumo(imagecache_presets());
$elemento=array(
'fid'=>"001",
'nid'=>"001",
'filepath'=>$imgpath,
'data'=>array('alt'=>"madrid_US_2",'title'=>"madrid_US_2")
);

$opciones=array("customlink"=>"espacios/madrid_US_2");
//print "XXXXX".$imgpath.theme('imageformatters_grayover',$elemento,"FooterImg_210x85" , $opciones );
?>

				<?php echo theme('imageformatters_grayover',$elemento,"FooterImg_210x85" , $opciones ); ?>
				<strong class="title">Madrid Espacio_US2</strong>
				<address style="height: 38px;">C/ Duque de Rivas, 5 28012 Madrid.</address>
				<dl>
					<dt>Tel:</dt>
					<dd>+34 913 89 66 90</dd> 
				</dl>
			</div>
			<!-- box -->
			<div class="box">
            
<?php 
$imgpath=path_to_theme()."/images/fotospieweb/IMG_2966.JPG";
//krumo(imagecache_presets());
$elemento=array(
'fid'=>"001",
'nid'=>"001",
'filepath'=>$imgpath,
'data'=>array('alt'=>"open_US",'title'=>"open_US")
);

$opciones=array("customlink"=>"espacios/open_us");
//print "XXXXX".$imgpath.theme('imageformatters_grayover',$elemento,"FooterImg_210x85" , $opciones );
?>
				              
                <?php echo theme('imageformatters_grayover',$elemento,"FooterImg_210x85" , $opciones ); ?>
                
				<strong class="title">Open_US</strong>
				<address style="height: 38px;">Abre un espacio Utopic_Us en tu ciudad.</address>
				<a class="more" href="/es/espacios/open_us">Más información</a>
			</div>
			<!-- facebook-box -->
			<div class="facebook-box">
				<!-- facebook -->
				<div class="facebook">
                <!-- <img width="210" height="60" alt="facebook" src="<?php print $theme_pathabsoluto; ?>/images/img-facebook.gif">-->
                <!-- Facebook code -->
<div class="fb-like" data-href="http://www.utopicus.es" data-send="false" data-layout="button_count" data-width="450" data-show-faces="false"></div>
                <!-- //Facebook code -->
                
                </div>
				<strong class="title">Suscríbete a nuestro boletín</strong>
                
                <?php 

				// funcion en template.php, para que siempre muestre los campos, aún registrado.
				$blockChimp =  utopicus_mailchimp_block('view','a66e7d45f0');
				
				print $blockChimp['content'];
				
				// miKrumo($blockChimp);
				?>
                
				
		</div>
		</div>
		<!-- sidemap-area -->
		<div class="sidemap-area">
			<!-- banner -->
			<a class="banner" href="#"><img width="125" height="125" alt="banner" src="<?php print $theme_pathabsoluto; ?>/images/img-banner.gif"></a>
             <div id="slide_patrocinios" class="banner">
    <img src="<?php print $theme_pathabsoluto; ?>/images/logos/heineken_logo.png" alt="HEINEKEN" class="active" />
    <img src="<?php print $theme_pathabsoluto; ?>/images/logos/edisa.png" alt="EDISA" />
</div>
			<!-- box -->
			<div class="box">
				<strong class="title"><a href="#">COWORKING</a></strong>
				<ul>
                	<li><a href="/es/coworking/tour/que-es">Tour</a></li>
								<li><a href="/es/coworking/coworkers">Coworkers</a></li>
                                <li><a href="/es/coworking/comunidad">Comunidad</a></li>
                                <li><a href="/es/coworking/espacios">Espacios</a></li>
                                <li><a href="/es/coworking/tarifas_y_planes">Tarifas y planes</a></li>
				</ul>
			</div>
			<!-- box -->
			<div class="box">
				<strong class="title"><a href="#">FORMACION</a></strong>
				<ul>
					<li><a href="/es/formacion/tocs">Videocápsulas</a></li>
                             <li><a href="/es/formacion/cursos">Cursos</a></li>
                             <li><a href="/es/formacion/escuelas">Escuelas</a></li>
				</ul>
			</div>
			<!-- box -->
			<div class="box">
				<strong class="title"><a href="#">ACTIVIDADES</a></strong>
				<ul>
					<li><a href="/es/secciones/actividades/agenda">Agenda</a></li>
                                <li><a href="/es/actividades/utopic_gallery">Utopic_Gallery</a></li>
                                <li><a href="/es/actividades/utopic_sundays">Utopic_Sundays</a></li>
                                <li><a href="/es/actividades/mmm_us">MMM_US</a></li>
                                <li><a href="/es/actividades/utopic_tv">Utopic_TV</a></li>
                                <li><a href="/es/actividades/la_transformadora">Transformadora</a></li>
					
				</ul>
			</div>
			<!-- box -->
			<div class="box">
				<strong class="title"><a href="#">MOSTRADOR</a></strong>
				<ul>
					
				</ul>
			</div>
			<!-- box -->
			<div class="box">
				<strong class="title"><a href="#">ESPACIOS</a></strong>
				<ul>
					<li><a href="/es/espacios/madrid_US_1">Madrid US_1</a></li>
                                    <li><a href="/es/espacios/madrid_US_2">Madrid US_2</a></li>
                                    <li><a href="/es/espacios/alquiler_de_espacios">Alquiler de Espacios</a></li>
                                    <li><a href="/es/espacios/open_us">Open_US</a></li>
				</ul>
			</div>
			<!-- box -->
			<div class="box">
				<strong class="title"><a href="#">_US</a></strong>
				<ul>
					<li><a href="<?php echo url("_US/el_proyecto") ?>">El proyecto</a></li>
                                    <li><a href="<?php echo url("_US/el_equipo") ?>">El equipo</a></li>
                                    <li><a href="<?php echo url("_US/secciones/noticias") ?>">Noticias</a></li>
                                    <li><a href="<?php echo url("_US/patrocinadores") ?>">Patrocinadores</a></li>
                                    <li><a href="<?php echo url("_US/sala_de_prensa") ?>">Sala de prensa</a></li>
                                    <li><a href="<?php echo url("_US/contacto") ?>">Contacto</a></li>
				</ul>
			</div>
		</div>
		<!-- footer-bar -->
		<div class="footer-bar">
			<!-- footer-nav -->
			<ul class="footer-nav">
				<li><a href="/es/legal/condiciones-de-uso">Condiciones de uso</a></li>
				<li><a href="/es/legal/politica-de-privacidad">Política de privacidad</a></li>
                <li><a href="/es/legal/aviso-legal">Aviso Legal</a></li>
				<li><a href="/sitemap">Mapa del sitio</a></li>
			</ul>
			<div class="alignright">
				<!-- social-networks -->
				<ul class="social-networks">
					<li><a class="facebook" href="http://www.facebook.com/utopicus">facebook<span class="mask">&nbsp;</span></a></li>
					<li><a class="twitter" href="http://www.twitter.con/utopic_US">twitter<span class="mask">&nbsp;</span></a></li>
					<li><a class="igoogle" href="https://plus.google.com/u/0/117771370417955141765/posts">Google+<span class="mask">&nbsp;</span></a></li>
					<li><a class="vimeo" href="http://www.vimeo.com/utopicus">vimeo<span class="mask">&nbsp;</span></a></li>
					<li><a class="flickr" href="http://www.flickr.com/photos/utopicus/sets/">flickr<span class="mask">&nbsp;</span></a></li>
					<li><a class="rss" href="/es/rss.xml">rss<span class="mask">&nbsp;</span></a></li>
				</ul>
				<!-- banner -->
                
               

				<a class="banner" href="#" ><img width="157" height="33" alt="Ministerio de cultura" src="<?php print $theme_pathabsoluto; ?>/images/logos/ministerio_cuadrado.png"></a>
			</div>
		</div>
		<span class="copyright">&copy; 2012 Utopicus Innovación Cultural SL - Todos los derechos reservados</span>
        <span class="ministerio_subv">Actividad subvencionada por el Ministerio de Cultura.</span>
	</div> 
    <!-- FIN FOOTER -->
    
<!-- <iframe id="font-resize-frame-336" class="font-resize-helper" style="width: 100em; height: 10px; position: absolute; border-width: 0px; top: -9999px; left: -9999px;"></iframe>

<div id="fancybox-tmp"></div>

<div id="fancybox-loading"><div></div></div>

<div id="fancybox-overlay"></div>

<div id="fancybox-wrap"><div id="fancybox-outer"><div id="fancybox-bg-n" class="fancybox-bg"></div><div id="fancybox-bg-ne" class="fancybox-bg"></div><div id="fancybox-bg-e" class="fancybox-bg"></div><div id="fancybox-bg-se" class="fancybox-bg"></div><div id="fancybox-bg-s" class="fancybox-bg"></div><div id="fancybox-bg-sw" class="fancybox-bg"></div><div id="fancybox-bg-w" class="fancybox-bg"></div><div id="fancybox-bg-nw" class="fancybox-bg"></div><div id="fancybox-content"></div><a id="fancybox-close"></a><div id="fancybox-title"></div><a id="fancybox-left" href="javascript:;"><span id="fancybox-left-ico" class="fancy-ico"></span></a><a id="fancybox-right" href="javascript:;"><span id="fancybox-right-ico" class="fancy-ico"></span></a></div></div> -->
