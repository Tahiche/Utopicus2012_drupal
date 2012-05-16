// $Id$

Magento Stock
-------------------------------------------------------------------------------

DESCRIPTION
-------------------------------------------------------------------------------

Magento Stock module provides integration of Magento Inventory with Drupal.
What it actually does is retrieve Stock data for Magento Product from Magento 
on Products synchronization and on Cart viewing and save it in a local Drupal's 
storage. The Stock information is added into Product node data on node view 
(this Stock information can be used for product theming or any other purpose).

INSTALLATION
-------------------------------------------------------------------------------

Install as usual, see http://drupal.org/node/70151 for further information.

OPTIONAL
-------------------------------------------------------------------------------

This module provides some useful functions for manipulating Product's Stock data
  - magento_stock_get_stock_by_product_id($product_id)
      Retrieves current Stock information from local storage by Magento's product_id
  - magento_stock_add_stock_to_product(&$node)
      Adds Stock information into the Magento Product's $node->stock.
  - magento_stock_api_get_remote_stock($product_ids)
      Magento API function to get Stock information for the product(s) from remote 
      Magento site.
      
OTHER WARNINGS
-------------------------------------------------------------------------------

Currently there is no module in Magento Package that depends on this module, or 
its functionality so you could disable this module unless you are going to 
use/display Stock information in Drupal.

Maintainers
-------------------------------------------------------------------------------
Module is fully developpped, sponsored and maintained by Adyax.

Current maintainers:
Ivan Tsekhmistro - itsekhmistro@adyax.com
Valeriy Sokolov - vsokolov@adyax.com
