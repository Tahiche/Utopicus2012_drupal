
/**
 * Behavior to add "Insert" buttons.
 */
Drupal.behaviors.TokenInsertText = function(context) {
  if (typeof(insertTextarea) == 'undefined') {
    insertTextarea = $('#edit-body').get(0) || false;
  }
  
  // Add the click handler to the insert button.
  $('.token-insert-text-button', context).click(insert);

  function insert() {
    var field = $(this).attr('rel');
    var content = '[' + $('#token-insert-text-select-' + field).val() + ']';
    Drupal.tokeninsert.insertAtCursor($('#edit-' + field), content);
    return false;
  }
};

