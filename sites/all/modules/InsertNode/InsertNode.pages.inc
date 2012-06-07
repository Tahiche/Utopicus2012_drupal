<?php
// $Id: InsertNode.pages.inc,v 1.1.2.30 2010/04/21 11:18:29 alexiswilke Exp $
// Author: Mark Burton
// Conversion & enhancements for D6: Alexis Wilke

/**
 * @file InsertNode.pages.inc
 *
 * Defines the functions necessary to replace the [node] tag as you'd expect.
 */

/**
 * Function used to replace the [node:...] tags.
 */
function _InsertNode_replacer($matches) {
  global $_InsertNode_demote_level;
  static $insert_node_count = 0;
  static $node_ids = array();

  // in case there are some extra spaces...
  $nid = str_replace('&nbsp;', '', $matches[1]);

  if (!is_numeric($nid)) {
    if ($nid == '*') {
      // insert current node, if available--useful in blocks
      if (arg(0) != 'node' || !is_numeric(arg(1))) {
        return;
      }
      $nid = arg(1);
    }
    else {
      $path = explode("/", drupal_get_normal_path(trim($nid, "/")));
      if ($path && ($path[0] == 'node') && is_numeric($path[1])) {
        $nid = $path[1];
      }
      else {
        return "<em>" . t("Can't find !s to include!", array("!s" => $matches[1])) . "</em>";
      }
    }
  }

  $node = node_load($nid);
  if ($nid && node_access('view', $node)) {
    // avoid recursivity
    if ($node_ids[$nid]) {
      return "<em>" . t("Node #!nid is being inserted twice by InsertNode. Breaking the recursive loop here.", array('!nid' => $nid)) . "</em>";
    }
    $node_ids[$nid] = TRUE;

    $_InsertNode_demote_level = 0;
    $plain = FALSE;
    $repeat = FALSE;
    $show_teaser = FALSE;
    $output = '';
    $comments = '';
    $fieldset = '';
    $fieldset_title = '';
    preg_match_all('/([a-zA-Z]+)(?:=([^;]*);)?/', $matches[2], $params);

    // First check for overrides
    if (in_array('override', $params[1])) {
      _InsertNode_overrides($node, $params);
    }

    $max = count($params[1]);
    for ($idx = 0; $idx < $max; ++$idx) {
      $p = $params[2][$idx];
      switch ($params[1][$idx]) {
      case 'demote':
        if ($p) {
          $_InsertNode_demote_level = (int)$p;
        }
        else {
          $_InsertNode_demote_level = 1;
        }
        break;

      case 'noterms':
        // nix the terms!
        $node->taxonomy = array();
        break;

      case 'plain':
        $plain = TRUE;
        break;

      case 'repeat':
        $repeat = TRUE;
        break;

      case 'themed':
        $output .= theme('InsertNode_themed', $node);
        break;

      case 'page':
        $output .= theme('InsertNode_page', $node);
        break;

      case 'body':
        $output .= theme('InsertNode_body', $node);
        break;

      case 'content':
        $output .= theme('InsertNode_content', $node);
        break;

      case 'title':
        if ($p) {
          $n = drupal_clone($node);
          $n->title = $p;
          $output .= theme('InsertNode_title', $n);
        }
        else {
          $output .= theme('InsertNode_title', $node);
        }
        break;

      case 'teaser':
        $output .= theme('InsertNode_teaser', $node);
        break;

      case 'more':
        if ($p) {
          $n = drupal_clone($node);
          $n->more = $p;
          $output .= theme('InsertNode_more', $n);
        }
        else {
          $output .= theme('InsertNode_more', $node);
        }
        break;

      case 'link':
        if ($p) {
          $n = drupal_clone($node);
          $n->title = $p;
          $output .= theme('InsertNode_link', $n);
        }
        else {
          $output .= theme('InsertNode_link', $node);
        }
        break;

      case 'edit':
        $output .= theme('InsertNode_edit', $node);
        break;

      case 'created':
        $output .= theme('InsertNode_created', $node);
        break;

      case 'modified':
        // The node may still be pristine (i.e. never was modified)
        if (!$node->modified) {
          // setup a valid modified date, since we don't intend to save the
          // node back anywhere are we're using a copy from the system cache
          // we fix the modified field right here
          $node->modified = $node->created;
        }
        $output .= theme('InsertNode_modified', $node);
        break;

      case 'collapsible':
      case 'collapsed':
        $fieldset = $params[1][$idx];
        $fieldset_title = $p ? $p : $node->title;
        break;

      case 'comments':
        // the comment module is optional
        if (module_exists('comment')) {
          // add the node comments; that function checks permissions
          $comments .= comment_render($node);
        }
        else {
          $comments .= "<em>" . t("Can't find the 'comment' module! <a href=\"/admin/build/modules\">Was it selected</a>?") . "</em>";
        }
        break;

      case 'cckteaser':
        $show_teaser = TRUE;
      case 'cck':
        switch ($p) {
        case 'body':
          // the user wants to see the body as is (without decorations)
          $body = str_replace('<!--break-->', '', $node->body);
          $output .= check_markup($body, $node->format, FALSE);
          if (!$output) {
            // we want to make sure that the default doesn't get inserted
            $output = ' ';
          }
          break;

        case 'teaser':
          // the user wants to see the teaser as is (without decorations)
          $output .= check_markup($node->teaser, $node->format, FALSE);
          if (!$output) {
            // we want to make sure that the default doesn't get inserted
            $output = ' ';
          }
          break;

        case 'title':
          // the user wants to see the title as is (without H3)
          $output .= check_plain($node->title);
          break;

        case 'book_children':
          if ($node->book) {
            $output .= theme('InsertNode_book_children', $node->book);
          }
          if (!$output) {
            // we want to make sure that the default doesn't get inserted
            $output = ' ';
          }
          break;

        case 'book_navigation':
          if ($node->book) {
            $output .= theme('book_navigation', $node->book);
          }
          if (!$output) {
            // we want to make sure that the default doesn't get inserted
            $output = ' ';
          }
          break;

        case 'webform':
          if ($node->webform) {
            theme('webform', $node);
          }
          break;

        default:
          if (module_exists('content')) { // the content module is optional
            // add the CCK content (Note: CCK permissions == Node permissions)
            $field = content_fields($p);
            if (is_array($field)) {
              $output .= content_view_field($field, $node, $show_teaser, FALSE);
            }
          }
          else {
            $comments .= "<em>" . t("Cannot find the 'content' (or CCK) module! <a href=\"/admin/build/modules\">Was it selected</a>?") . "</em>";
          }
          break;

        }
        $show_teaser = FALSE;
        break;

      case 'imagefield':
      case 'imagecache':
        // check for imagefield and imagecache module
        if (module_exists('imagefield') && module_exists('imagecache')) {
          list($field_name, $imagecache_value, $type, $index) = split(':', $p);
          // make sure everything is filled out and the imagecche exsits
          if (!empty($field_name) && !empty($imagecache_value)) {

            // get the index
            $index = empty($index) ? 0 : $index;

            // set the link type
            $type = empty($type) ? 'default' : $type;

            // get the file name
            $image_field = $node->$field_name;
            $filename = $image_field[$index]['filepath'];

            $output .= theme('InsertNode_imagefield', $filename, $imagecache_value, $type);
          }
        }
        break;

      case 'booktree':
        // the booktree module is optional
        if (module_exists('booktree')) {
          // add the booktree if user has permission
          if (user_access('access booktree')) {
            $output .= booktree_render_tree($node->nid);
          }
        }
        else {
          $output .= "<em>" . t("Cannot find the 'booktree' module! <a href=\"/admin/build/modules\">Was it selected</a>?") . "</em>";
        }
        break;

      default:
        $comments .= "<em>" . t("Unknown tag parameter '@param'.", array('@param' => $param[1][$idx])) . "</em>";
        break;

      }
    }
    if (!$output) {
      $output = theme('InsertNode_default', $node);
    }
    if ($comments) {
      $output .= theme('InsertNode_comments', $node, $comments);
    }
    if ($_InsertNode_demote_level != 0) {
      $output = preg_replace_callback('%<[hH]([1-6])(\s+[^>]*?)?>(.*?)</[hH][1-6]>%s',
                                      '_InsertNode_demote', $output);
    }
    if ($fieldset) {
      // put the output inside a field set
      $output = theme('fieldset', array(
        '#collapsible' => TRUE,
        '#collapsed'   => $fieldset == 'collapsed',
        '#title'       => $fieldset_title,
        '#value'       => $output));
    }
    ++$insert_node_count;
    $node_ids[$nid] = FALSE;
    if ($repeat) {
      $output = InsertNode_filter('process', 0, 0, $output);
    }
    if ($plain) {
      // the user does not want another theme() call
      return $output;
    }
    return theme('InsertNode_node', $node, $output, $insert_node_count);
  }

  return '';
}

/**
 * \brief Change the value of different parameters.
 *
 * This function changes CCK parameters of the node according to a set
 * of override parameters.
 *
 * The changes will stick in the node object after the filter ends which
 * means that it will apply to other modules using the CCK fields after
 * this filter was applied.
 *
 * \param[in,out] $node The node to modify
 * \param[in,out] $params The parameter with the override entries
 */
function _InsertNode_overrides(&$node, &$params) {
  global $_InsertNode_format;

  // pull out the override keys to the param array
  $override_keys = array_intersect($params[1], array('override'));

  $allowed = variable_get('InsertNode_override_' . $_InsertNode_format, 0);
  $has_cck = module_exists('content');

  // split the overrides to field:value
  foreach ($override_keys as $idx => $ignore) {
    $override = explode(':', $params[2][$idx]);

    if ($allowed) {
      // check to see if this is override cck
      if ($override[0] == 'cck') {
        if ($has_cck) {
          // set the key we are changing
          $key = isset($override[3]) ? $override[3] : 'value';

          // user specified index or 0
          $index = isset($override[4]) ? $override[4] : 0;

          // get the field
          // (note: we cannot take a reference in case it does not exist yet)
          $field = $node->$override[1];

          // update the value
          $field[$index][$key] = $override[2];

          // add to the node object
          $node->$override[1] = $field;
        }
      }
      else {
        // otherwise we are just updating a normal node field
        $node->$override[0] = $override[1];
      }
    }

    // remove the override from the param array
    // so they do not get processed any further
    unset($params[0][$idx]);
    unset($params[1][$idx]);
    unset($params[2][$idx]);
  }

  // Reset the keys so the next loop works
  // Passing only one array into array_merge reindexes the array
  $params[0] = array_merge($params[0]);
  $params[1] = array_merge($params[1]);
  $params[2] = array_merge($params[2]);
}

/**
 * Theme an imagefield using image cache
 */
function theme_InsertNode_imagefield($filename, $imagecache, $type) {
  switch ($type) {
  case 'link':
    $content = theme('imagecache_imagelink', $imagecache, $filename);
    break;

  default: //case 'default':
    $content = theme('imagecache', $imagecache, $filename);
    break;

  }

  return '<div class="insert-node-imagefield">' . $content . '</div>';
}

/**
 * Used to demote headers in the child node.
 */
function _InsertNode_demote($matches) {
  global $_InsertNode_demote_level;

  $level = $matches[1] + $_InsertNode_demote_level;
  if ($level < 1) {
    $level = 1;
  }
  elseif ($level > 6) {
    $level = 6;
  }

  $attributes = $matches[2];
  $title = $matches[3];

  $result = '<h' . $level . $attributes . '>' . $title . '</h' . $level . '>';

  return $result;
}

/**
 * Use this theme by default when no other option was specified (and thus
 * the output is still empty after we're done with the user options.)
 *
 * By default, this is similar to the theme_InsertNode_content() function.
 */
function theme_InsertNode_default($node) {
  return '<div class="insert-node-content">' . node_view($node) . '</div>';
}

/**
 * Theme the $node which usually means create the entire node
 * as if displayed in your page (i.e. title, messages, tabs,
 * body, links, etc.)
 */
function theme_InsertNode_themed($node) {
  return theme('node', $node);
}

/**
 * Theme the page of the specified node including the links.
 *
 * This function calls node_view() as required and encompasses the
 * page in a <div> tag so it can be styled differently if necessary.
 * The page will include the body and the links.
 *
 * \code
 * node_view($node, $teaser = FALSE, $page = TRUE, $links = TRUE)
 * \endcode
 */
function theme_InsertNode_page($node) {
  return '<div class="insert-node-page">' . node_view($node, FALSE, TRUE, TRUE) . '</div>';
}

/**
 * Theme the body of the specified node.
 *
 * This function calls node_view() as required and encompasses the
 * body in a <div> tag so it can be styled differently if necessary.
 *
 * \code
 * node_view($node, $teaser = FALSE, $page = TRUE, $links = FALSE)
 * \endcode
 */
function theme_InsertNode_body($node) {
  $node->links = NULL;
  return '<div class="insert-node-body">' . node_view($node, FALSE, TRUE, FALSE) . '</div>';
}

/**
 * Theme the body of the specified node using the default node_view()
 * flags. This means you will get the whole body, not paged, and
 * with links at the bottom (or wherever your theme puts your links.)
 *
 * \code
 * node_view($node, $teaser = FALSE, $page = FALSE, $links = TRUE)
 * \endcode
 */
function theme_InsertNode_content($node) {
  return '<div class="insert-node-content">' . node_view($node) . '</div>';
}

/**
 * Theme the title of the node within an H3 header.
 */
function theme_InsertNode_title($node) {
  return '<h3 class="insert-node-title">' . l($node->title, "node/$node->nid") . '</h3>';
}

/**
 * Theme the teaser of the node.
 */
function theme_InsertNode_teaser($node) {
  $node->links = NULL;
  return '<div class="insert-node-teaser">' . node_view($node, TRUE, FALSE, FALSE) . '</div>';
}

/**
 * Theme a link to the node with the text "Read more..." possibly translated.
 */
function theme_InsertNode_more($node) {
  if (!$node->more) {
    $node->more = t('Read more...');
  }
  return '<span class="insert-node-more">' . l($node->more, "node/$node->nid") . '</span>';
}

/**
 * Theme a link to the node using the node title as the label.
 */
function theme_InsertNode_link($node) {
  return '<span class="insert-node-link">' . l($node->title, "node/$node->nid") . '</span>';
}

/**
 * Theme a link to the node using the word 'edit' as the label.
 */
function theme_InsertNode_edit($node) {
  return '<span class="insert-node-edit">' . l(t('edit'), "node/$node->nid/edit") . '</span>';
}

/**
 * Theme the creation date of the node.
 */
function theme_InsertNode_created($node) {
  return '<span class="insert-node-date">' . format_date($node->created) . '</span>';
}

/**
 * Theme the last modification date of the node.
 * Note that the date may be equal to the creation date if the node
 * has not yet been modified.
 */
function theme_InsertNode_modified($node) {
  return '<span class="insert-node-date">' . format_date($node->modified) . '</span>';
}

/**
 * Theme the comments of the node. This function receives the node
 * object and a string with the comments. It is up to you to re-generate
 * the comments in a different way than the default if needed.
 */
function theme_InsertNode_comments($node, $comments) {
  return '<div class="insert-node-comments">' . $comments . '</div>';
}

/**
 * Theme the final result where $output represents all the node
 * data concatenated as specified by the user.
 */
function theme_InsertNode_node($node, $output, $insert_node_count) {
  // do we have to insert a <div>?
  // TODO: Let users define the list of tags
  $classes = ' class="insert-node insert-node' . $insert_node_count . ' insert-nid' . $node->nid . '"';
  if (preg_match('/<(div|h[1-6]|p\>|ol|ul|li|dl|dt|dd|table|tr|td|blockquote|pre|embed|object|caption)/', $output)) {
    return '<div' . $classes . '>' . $output . '</div>';
  }
  // we don't have to, use a <span> instead so it can be inlined
  return '<span' . $classes . '>' . $output . '</span>';
}

/**
 * Use this theme by default when no other option was specified (and thus
 * the output is still empty after we're done with the user options.)
 *
 * By default, this is similar to the theme_InsertNode_content() function.
 */
function theme_InsertNode_webform($node) {
  return '<div class="insert-webform-content">' . webform_view($node, FALSE, TRUE) . '</div>';
}

/**
 * Process variables for InsertNode-book-children.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $book_link
 *
 * \see InsertNode-book-children.tpl.php
 */
function template_preprocess_InsertNode_book_children(&$variables) {
  $book_link = $variables['book_link'];

  // Provide extra variables for themers. Not needed by default.
  $variables['book_id'] = $book_link['bid'];
  $variables['book_title'] = check_plain($book_link['link_title']);
  $variables['book_url'] = 'node/' . $book_link['bid'];
  $variables['current_depth'] = $book_link['depth'];

  $variables['tree'] = '';
  if ($book_link['mlid']) {
    $variables['tree'] = book_children($book_link);
  }
}

// vim: ts=2 sw=2 et syntax=php
