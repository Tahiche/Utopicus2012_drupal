<?php
/**
 * @file
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $display_submitted: whether submission information should be displayed.
 * - $submitted: Themed submission information output from
 *   theme_node_submitted().
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 *   The following applies only to viewers who are registered users:
 *   - node-by-viewer: Node is authored by the user currently viewing the page.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $build_mode: Build mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $build_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * The following variable is deprecated and will be removed in Drupal 7:
 * - $picture: This variable has been renamed $user_picture in Drupal 7.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see zen_preprocess()
 * @see zen_preprocess_node()
 * @see zen_process()
 */
 // krumo($variables);
?>
<?php //krumo($variables); ?> 


<div class="profile-section">
					<!-- aside -->
					<div class="aside">
						
                        <?php 
								if($node->picture)$imgpath=$node->picture;
								else $imgpath=$node->field_userpic[0]['filepath'];
								//krumo(imagecache_presets());
								$elemento=array(
								'fid'=>$node->nid,
								'nid'=>$node->nid,
								'filepath'=>$imgpath,
								'data'=>array('alt'=>$node->title,'title'=>$node->title)
								);
								//image223_gray 
								$opciones=array("nolink"=>TRUE);
								print theme('imagecache',"Imagen_template_327x327" ,$imgpath,$node->title,$node->title );
						?>

						<!-- photo-info -->
						<dl class="photo-info">
						<!-- <dt>
								<img width="26" height="25" alt="image discription" src="<?php print "/".path_to_theme() ?>/images_dummy/img44.gif">
								<span>karma</span>
							</dt> -->
							<dd>
                            <?php // print theme ('plus1_widget',$node); 
							//print plus1_jquery_widget($node,NULL,TRUE);
							print $node->content['plus1_widget']['#value'];
							?>
							</dd>
						</dl>
					</div>
					<!-- holder -->
					<div class="holder">
						<!-- heading -->
						<div class="heading">
							<h2><?php print $node->title;  ?></h2>
						</div>
						<!-- areas -->
						<dl class="areas">
							<dt>Áreas profesionales:</dt>
							<dd>
                            <?php print $terms_by_vocab[3]; ?>
                             
                            </dd>
						</dl>
						<!-- links -->
						<dl class="sociales links">
							<dt>
								<!-- social-networks -->
								<ul class="social-networks">
                                <?php if($field_facebook[0]['url']):?>
                                <li>
									<a class="facebook" href="<?php echo $field_facebook[0]['url'] ?>">facebook<span class="mask" style="opacity: 1;">&nbsp;</span></a>
                                </li>
								<?php endif; ?>
                                <?php if($field_twitter[0]['url']):?>
                                <li>
									<a class="twitter" href="<?php echo $field_twitter[0]['url'] ?>">twitter<span class="mask" style="opacity: 1;">&nbsp;</span></a>
                                    </li>
                                    <?php endif; ?>
                                <?php if($field_linkedin[0]['url']):?>
                                <li>
									<a class="LinkedIn" href="<?php echo $field_linkedin[0]['url'] ?>">LinkedIn<span class="mask" style="opacity: 1;">&nbsp;</span></a>
                                   </li>
                                <?php endif; ?>
                                <?php 
								// no tenemos forma de saber si ha habilitado contact form o no.
								// si la ha habilitado (menu_valid_path) , se ha creado un path al form (true)... si no, devuelve false

								if( _contact_user_tab_access($node) && menu_valid_path(array('link_path' => 'user/'.arg(1).'/contact')) ):
								//313
								// Anonymous users cannot use or have contact forms.
								// contact.module 120
								// _contact_user_tab_access
								
								?>
                                <li>
									<a class="mail" href="<?php echo url('user/'.arg(1).'/contact'); ?>">mail<span class="mask" style="opacity: 1;">&nbsp;</span></a>
                                    </li>
                                <?php endif; ?>
                                    
								</ul>
                                
							</dt>
							<dd>
                            <?php print $field_weblink1[0]['view']; ?>
                            <?php print $field_weblink12[0]['view']; ?>
							</dd>
						</dl>
                        <div id="presentate">
						<?php print  $node->field_presentate[0]['value'] ?>
                        </div>
						<!-- title -->
						<strong class="title etiquetascoworker">Etiquetas</strong>
						<!-- tags-list -->
                        <div id="coworkertags">
						 <?php print $terms_by_vocab[4]; ?>
                         </div>
						 
						<div class="action">
                        <span class="alignleft">
								<a class="prev" href="<?php echo url ("coworking/coworkers"); ?>">prev</a>
								<span class="text">BUSCAR MAS COWORKERS</span>
							</span>                            
														
						</div>
					</div>
				</div> <!-- fin rofile section -->
                
                
                
                
                
<div class="about-tabs">
					<!-- tabset -->
					<ul class="tabset">
						<li ><a class="tab" href="#tab1">ENTREVISTA</a></li>
						<li><a class="tab" href="#tab2">VIDEOCÁPSULAS</a></li>
					</ul>
					<!-- tab-content -->
					<div class="tab-content">
							<div id="tab1" style="display: block;">
							<?php 
							global $user;
							if($user->uid) print $group_perfil_privado_rendered ;
							else print t("Contenido exclusivo para coworkers. ¿Eres coworker? ")."<u><b>".l(t('Login'), 'user/login')."</b></u>";
							?>
								
						</div>
						<div id="tab2" style="display: none;"><?php 
						print views_embed_view('view_grid', 'tocs_by_user',arg(1));
						
						?></div>
						
					</div>
				</div>
