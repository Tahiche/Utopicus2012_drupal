<?php
// $Id: index.php,v 1.21.2.2 2010/12/10 20:31:26 damz Exp $
/**
 * @file
 * API Documentation index file.
 */

/** @mainpage Developer Documentation
 * Drupal-Magento is a set of modules that Magento functionality into Drupal. 
 * Magento is the best open source e-commerce platform in the world, which gives user 
 * an exhostive set of features out of the box. However, Drupal is the best CMS in the world, 
 * that provides unique abilities to build, present and manage the content of the web-site.
 * Our goal is to combine strenghts of both platforms providing to user 
 * a powerfull e-commerce platform with front-end that can be easily themed and managed.
 *
 * DEMO sites:
 *   - @link http://demo.drupal-magento.com/drupal/ Drupal @endlink (login/password: demo/password1234)
 *   - @link http://demo.drupal-magento.com/magento/ Magento @endlink (login/password: demo/password1234)
 *
 * Follow us on Twitter to find out latest news and announcement: @link http://twitter.com/#!/DrupalMagento @DrupalMagento @endlink
 *
 * Drupal-Magento modules provide:
 * - Synchronization of Magento products to Drupal (currently supported type of products are: simple products, configurable products). All Magento attributes are dynamically synchronized into Drupal CCK fields
 * - Custom product options
 * - Up-sells, cross-sells, linked products
 * - Synchronization of Magento categories into Drupal taxonomies
 * - Synchronization of Drupal users to Magento customers, synchronization of addresses
 * - Synchronization of currencies and currency conversion rates
 * - Stock management
 * - Full-featured shopping cart
 * - Fully themable and customizable checkout process
 * - Coupon codes
 * - Payment modes API
 * - Shipping methods API (support of all simple Magento shipping modes works out of the box)
 * - Our modules support a big amount of features, however, there are lots of those that are not supported (for example we support only sync. of category title and description, without other category attributes). To solve this issue our modules provide an API which allows developers to easily hook into the process of synchronization or write your own modules to synchronize data.
 * 
 * Important! This module requires Magento extension: 
 * @link http://www.magentocommerce.com/extension/1020/drupal Magento extension @endlink. 
 * To install extension follow the standard Magento extension installation procedure 
 * (make sure to set preferred release version to Beta in Magento Connect 
 * before installing the extension).
 * 
 * Below is a table of versions compatibility between Drupal module and Magento extension:
 * - 6.x-2.x => 2.0.x
 * - 6.x-1.4 => 1.3.4
 * - 6.x-1.3 => 1.3.2
 * 
 * Magento versions supported by drupal-magento modules v2.x:
 * - 1.4.2
 * - 1.5.x
 * - 1.6.x
 * 
 * Related modules
 * - Check / Money order payment module
 * - Magento Devel - a must-have module for drupal-magento module developer
 * - Attribute filter - helper module which can increase synchronization speed for big number of products.
 *
 * Credits.
 * Original idea & sponsoring :  @link http://www.adyax.com/en Adyax @endlink, Gold Acquia Partner & Biggest european Drupal team.
 * 
 * Live sites.
 * Here is a list of live sites using Drupal+Magento, if you have more, let us know ! :
 * - @link http://www.editions-cigale.com @endlink
 * - @link http://www.bestofshopping.tv @endlink
 * - @link http://www.french-place.com @endlink
 *
 * Components of Drupal-Magento modules package
 * - @link /api/constants Constants @endlink
 * - @link /api/globals Global variables @endlink
 * - @link /api/files Files @endlink
 * - @link /api/functions Functions @endlink
 *
 */
