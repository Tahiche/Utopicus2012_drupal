<?php if (!function_exists("url")){
	
	function url($string){
                global $base_root;
		return $base_root."/es/".$string;
		}
	
	}
global $base_root;
if (empty($base_root)){
        $base_root="http://www.utopicus.es";
}

?>
<ul id="nav">
						<li class="style-1 withdrop">
							<a href="#"><span>COWORKING</span></a>
							<ul class="drop" style="opacity: 0; top: 100%; display: none;">
                            <li><a href="<?php echo $base_root; ?>/es/coworking/tour">Tour</a></li>
								<li><a href="<?php echo $base_root; ?>/es/coworking/coworkers">Coworkers</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/coworking/comunidad">Comunidad</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/coworking/espacios">Espacios</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/coworking/tarifas_y_planes">Tarifas y planes</a></li>
							</ul>
						</li>
						<li class="style-2 withdrop">
							<a href="#"><span>FORMACIÓN</span></a>
							<ul class="drop" style="opacity: 0; top: 100%; display: none;">
                             <li><a href="<?php echo $base_root; ?>/es/formacion/tocs">Vídeos TOCS</a></li>
                             <li><a href="<?php echo $base_root; ?>/es/formacion/cursos">Cursos</a></li>
                             <li><a href="<?php echo $base_root; ?>/es/formacion/escuelas">Escuelas</a></li>
                               
                                
							</ul>
						</li>
						<li class="style-3 withdrop">
							<a href="#"><span>actividades</span></a>
							<ul class="drop" style="opacity: 0; top: 100%; display: none;">
                                <li><a href="<?php echo $base_root; ?>/es/secciones/actividades/agenda">Agenda</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/actividades/utopic_gallery">Utopic_Gallery</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/actividades/utopic_sundays">Utopic_Sundays</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/actividades/mmm_us">MMM_US</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/actividades/utopic_tv">Utopic_TV</a></li>
                                <li><a href="<?php echo $base_root; ?>/es/actividades/la_transformadora">Transformadora</a></li>
							</ul>
						</li>
						<!-- <li class="style-4 withdrop">
							<a href="#"><span>mostrador</span></a>
                            <ul class="drop" style="opacity: 0; top: 100%; display: none;">
                                    
							</ul>
						</li>-->
						<li class="style-5 withdrop">
							<a href="#"><span>espacios</span></a>
							<ul class="drop" style="opacity: 0; top: 100%; display: none;">
                                    <li><a href="<?php echo $base_root; ?>/es/espacios/madrid_US_1">Madrid US_1</a></li>
                                    <li><a href="<?php echo $base_root; ?>/es/espacios/madrid_US_2">Madrid US_2</a></li>
                                    <li><a href="<?php echo $base_root; ?>/es/espacios/alquiler_de_espacios">Alquiler de Espacios</a></li>
                                    <li><a href="<?php echo $base_root; ?>/es/espacios/open_us">Open_US</a></li>
							</ul>
						</li>
						<li class="style-6 withdrop">
							<a href="#"><span>_US</span></a>
							<ul class="drop" style="opacity: 0; top: 100%; display: none;">
                                    <li><a href="<?php echo url("_US/el_proyecto") ?>">El proyecto</a></li>
                                    <li><a href="<?php echo url("_US/el_equipo") ?>">El equipo</a></li>
                                    <li><a href="<?php echo url("secciones/_US/noticias") ?>">Noticias</a></li>
                                    <li><a href="<?php echo url("_US/patrocinadores") ?>">Patrocinadores</a></li>
                                    <li><a href="<?php echo url("_US/sala_de_prensa") ?>">Sala de prensa</a></li>
                                    <li><a href="<?php echo url("_US/contacto") ?>">Contacto</a></li>
							</ul>
						</li>
					</ul>


