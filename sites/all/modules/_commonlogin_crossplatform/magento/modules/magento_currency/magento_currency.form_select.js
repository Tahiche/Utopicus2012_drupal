// $Id$
/**
 *
 */
// Namespace.
var magento_currency = magento_currency || {};

$(document).ready(function () {
  magento_currency.onReady();
});

/**
 * OnReady handler, auto submit currency selection form on block value change.
 */
magento_currency.onReady = function () {
  $('#magento-currency-form-select select').change(function () {
    $('#magento-currency-form-select').submit();
  });
}