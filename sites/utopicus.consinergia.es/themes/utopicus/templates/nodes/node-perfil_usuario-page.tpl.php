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
								<strong class="number">25</strong>
								<a class="like" href="#">ME GUSTA</a>
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
						<dl class="links">
							<dt>
								<!-- social-networks -->
								<span class="social-networks">
									<a class="facebook" href="#">facebook<span class="mask" style="opacity: 1;">&nbsp;</span></a>
									<a class="twitter" href="#">twitter<span class="mask" style="opacity: 1;">&nbsp;</span></a>
									<a class="igoogle" href="#">iGoogle<span class="mask" style="opacity: 1;">&nbsp;</span></a>
									<a class="mail" href="#">mail<span class="mask" style="opacity: 1;">&nbsp;</span></a>
								</span>
							</dt>
							<dd>
                            <?php print $field_weblink1[0]['view']; ?>
                            <?php print $field_weblink12[0]['view']; ?>
							</dd>
						</dl>
						<?php print  $node->field_presentate[0]['value'] ?>
						<!-- title -->
						<strong class="title etiquetascoworker">Etiquetas</strong>
						<!-- tags-list -->
                        <div id="coworkertags">
						 <?php print $terms_by_vocab[4]; ?>
                         </div>
						<!-- action -->
						<!-- <div class="action">
							<span class="alignleft">
								<a class="prev" href="#">prev</a>
								<span class="text">BUSCAR MAS COWORKERS</span>
							</span>
							<span class="alignright">
								<a class="next" href="#">next</a>
								<span class="text">VISITA EL MOSTRADOR DE MIS SERVICIOS</span>
							</span>
						</div> -->
					</div>
				</div>