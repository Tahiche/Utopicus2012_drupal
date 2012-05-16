// $Id$

Magento Cart Quote
-------------------------------------------------------------------------------

Integrates with Quote and Order in Magento Provides.
This module implements Cart Checkout Process in Drupal.

Magento Cart Quote implements hook_magento_cart_steps() and defines following 
Magento cart Steps and their implementation:
   1) 'view' => 'My Cart'. 
        Review Cart products: List/Change/Remove products in cart.
   2) 'identification' => 'Identification',
        Ask user to login/register if not login yet.
   3) 'delivery' => 'Billing and Shipping',
        Ask user to Select Billing and Shipping Addresses.
   4) 'shipping_method' => 'Shipping method',
        Ask user to Select Shipping method
   5) 'payment' => 'Payment Information',
        Ask user to Select Payment Method
   6) 'review' => 'Order Review',
        Show Order summary before checkout
   7) 'payment-succeeded' => 'Confirmation',
        Show Succeeded Order confirmation screen.

Magento Cart Quote module provides hooks and helpers funtions to integrate Payment
methods into the checkout process.

For detailed Instructions on Integrating payment module into Magento Cart Quote 
please see https://prj.adyax.com/projects/drupal-magento/wiki/Payment_modules.

INSTALLATION
-------------------------------------------------------------------------------

Install as usual, see http://drupal.org/node/70151 for further information.

OTHER WARNINGS
-------------------------------------------------------------------------------

To add/extend required cart steps, implement hook_magento_cart_steps().
To alter cart steps, implement hook_magento_cart_steps_alter(&$steps).

Maintainers
-------------------------------------------------------------------------------
Module is fully developpped, sponsored and maintained by Adyax.

Current maintainers:
Ivan Tsekhmistro - itsekhmistro@adyax.com
Valeriy Sokolov - vsokolov@adyax.com

Previous maintainers:
Andrei Kovalevsky - akovalevsky@adyax.com
Maxim Mironenko - maximm@adyax.com
