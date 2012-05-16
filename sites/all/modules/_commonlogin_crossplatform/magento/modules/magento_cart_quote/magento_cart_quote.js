Drupal.behaviors.magento_cart = function(context) {
  // IE hack for VIEW, DELIVERY & PAYMENT///
  if ($.browser.msie) {
    $('.block-panier-calculate input[type=radio], input[name=magento_cart_quote_payment_methods], input[name=magento_cart_quote_payment_conditions]').click(function() {
      this.blur();
      this.focus();
    });
  }
  
  //DELIVERY//////////////////////////
  $('.block-shipping-choise-method input[type=radio]').change(function(){
    //$('div.messages').hide();
    var methods = Drupal.settings.magento_cart_quote || {};
    methods = methods.shipping_methods || {};
    var method = $(this).val();
    $('#magento_cart_quote_shipping_method').html(methods[method].price);
    $('#magento_cart_quote_grand_total').html(methods[method].grand_total);
    //$('input[name=delivery_calc]').change();
  });
}