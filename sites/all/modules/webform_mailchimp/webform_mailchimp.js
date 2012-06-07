Drupal.behaviors.webformMailchimpFieldSettings = function(context) {
  $('#edit-extra-use-existing-email-field', context).change(function(){
    var sel = $("#edit-extra-use-existing-email-field option:selected");
    if (sel[0]["value"] != 'mailchimp_field') {
      $('#field_settings').show();
    }
    else {
      $('#field_settings').hide();
    }
  });
}

$(document).ready(function() {
  if ($("#edit-extra-use-existing-email-field option:selected")[0]['value'] != 'mailchimp_field') {
    $('#field_settings').show();
  }
  else {
    $('#field_settings').hide();
  }
});