// $Id

/**
 * Wysiwyg plugin button implementation for token_insert plugin.
 */
Drupal.wysiwyg.plugins.token_insert_wysiwyg = {
  /**
   * Return whether the passed node belongs to this plugin.
   *
   * @param node
   *   The currently focused DOM element in the editor content.
   */
  isNode: function(node) {
    return ($(node).is('img.token_insert_wysiwyg-token_insert_wysiwyg'));
  },

  /**
   * Execute the button.
   *
   * @param data
   *   An object containing data about the current selection:
   *   - format: 'html' when the passed data is HTML content, 'text' when the
   *     passed data is plain-text content.
   *   - node: When 'format' is 'html', the focused DOM element in the editor.
   *   - content: The textual representation of the focused/selected editor
   *     content.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  invoke: function(data, settings, instanceId) {
    Drupal.wysiwyg.plugins.token_insert_wysiwyg.insert_form(data, settings, instanceId);    
  },
  
  
  insert_form: function (data, settings, instanceId) {
      form_id = Drupal.settings.token_insert_wysiwyg.current_form;

      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=token_insert_wysiwyg/insert/' + form_id;
			
      dialogIframe = Drupal.jqui_dialog.iframeSelector();
			btns = {};
      btns[Drupal.t('Insert token')] = function () {
   			
        var token = $(dialogIframe).contents().find('#edit-insert').val();
      	var editor_id = instanceId;
      	
      	token = '[' + token + ']';  
        Drupal.wysiwyg.plugins.token_insert_wysiwyg.insertIntoEditor(token, editor_id);
    
        $(this).dialog("close");
        
      };

      btns[Drupal.t('Cancel')] = function () {
        $(this).dialog("close");
      };
      
      Drupal.jqui_dialog.open({
        url: aurl,
        buttons: btns,
        width: 800,
        title: 'Insert Token'
      });
    },
    
  insertIntoEditor: function (token, editor_id) {
      Drupal.wysiwyg.instances[editor_id].insert(token);
    },

  /**
   * Prepare all plain-text contents of this plugin with HTML representations.
   *
   * Optional; only required for "inline macro tag-processing" plugins.
   *
   * @param content
   *   The plain-text contents of a textarea.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  attach: function(content, settings, instanceId) {
    content = content.replace(/<!--token_insert_wysiwyg-->/g, this._getPlaceholder(settings));
    return content;
  },

  /**
   * Process all HTML placeholders of this plugin with plain-text contents.
   *
   * Optional; only required for "inline macro tag-processing" plugins.
   *
   * @param content
   *   The HTML content string of the editor.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  detach: function(content, settings, instanceId) {
    var $content = $('<div>' + content + '</div>');
    $.each($('img.token_insert_wysiwyg-token_insert_wysiwyg', $content), function (i, elem) {
      //...
    });
    return $content.html();
  },

  /**
   * Helper function to return a HTML placeholder.
   *
   * The 'drupal-content' CSS class is required for HTML elements in the editor
   * content that shall not trigger any editor's native buttons (such as the
   * image button for this example placeholder markup).
   */
  _getPlaceholder: function (settings) {
    return '<img src="' + settings.path + '/images/spacer.gif" alt="&lt;--token_insert_wysiwyg-&gt;" title="&lt;--token_insert_wysiwyg--&gt;" class="token_insert_wysiwyg-token_insert_wysiwyg drupal-content" />';
  }
};







