// $Id$

Magento Cart module for Drupal 6.x
-------------------------------------------------------------------------------

Provides Basic Magento Shopping Cart.
Implements fuctionality:
 - load Magento Cart from User(for Authorized user)/Session(for Anonymous user)
 - save Magento Cart into User(for Authorized user)/Session(for Anonymous user)
 - update Magento Cart.
 - Cart Steps conception. (Checkout process is splited into substeps). 
 - Adds `Magento Cart block` block. This block displays current Cart status ( amount of Products in Cart ).
 
To add/extend required cart steps, implement hook_magento_cart_steps().
To alter cart steps, implement hook_magento_cart_steps_alter(&$steps).

INSTALLATION:
-------------------------------------------------------------------------------

Install as usual, see http://drupal.org/node/70151 for further information.
A comprehensive installation howto is already on its way.

Maintainers
-------------------------------------------------------------------------------
Module is fully sponsored and maintained by Adyax.

Current maintainers:
Ivan Tsekhmistro - itsekhmistro@adyax.com
Valeriy Sokolov - vsokolov@adyax.com

Previous maintainers:
Andrei Kovalevsky - akovalevsky@adyax.com
Maxim Mironenko - maximm@adyax.com
Ilya Gruzinov - igruzinov@adyax.com
