// $Id$

Magento Shipping
-------------------------------------------------------------------------------

DESCRIPTION
-------------------------------------------------------------------------------

Allows to use allowed (in Magento) shipping methods for current Cart state.

What this module actually does is implements hook_magento_shipping_methods().
This hook implementation returns array of allowed shipping methods using shipping data information
from remote Magento (the data is stored in the Magento Cart).

See magento_shipping_magento_shipping_methods() documentation for details.

OTHER WARNINGS
-------------------------------------------------------------------------------

Magento Cart Quote module won't allow to process to payment and create an order
unless this module (or any other module that implements hook_magento_shipping_methods) 
will be enabled.

You have to configure allowed Shipping Methods in Magento.

Maintainers
-------------------------------------------------------------------------------
Module is fully developpped, sponsored and maintained by Adyax.

Current maintainers:
Ivan Tsekhmistro - itsekhmistro@adyax.com
