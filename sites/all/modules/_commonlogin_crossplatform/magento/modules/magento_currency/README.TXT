// $Id$

Magento Currency
-------------------------------------------------------------------------------

1. OVERVIEW.
"Magento Currency" module allows you to transparently integrate currency conversation
and currency display on the site powered by Adyax's "Magento" package, it has no 
special requirements to use except ones that are applied to Adyax's "Magento" 
package, and will work out of the box for the most sites.

This module is an optional dependence for the "Magento Product" which can utilize 
its features but will work just fine without it.

If enabled, this module allows user to select required currency and
automatically convert any "price" value on the page to appropriate currency
i.e. will convert prices from site's default currency to the desired currency and add 
currency sign.

2. LIMITATIONS.
While there are no special limitations, this module will apply currency conversation only
when price is formatted using theme "magento_product_price" (which is for the most part 
a mandatory way of displaying prices). You also need to make at least one currency
sync to magento (there is a cron task for it).

3. CONFIGURABLE SETTINGS.
There is only one option available for you to configure - it's a currency display sings
and names, you can find this option here [admin/settings/magento/currency/config]
all currencies settings are entered in a form of a list as follows

[cur_code]|[name]|[sign]

where
[cur_code] - is a currency code, that correspond to Magento's currency codes
[name] - is a currency name that will be displayed in currency selection block
[sign] - is a currency sign that will be added to prices in this currency

for example: EUR|Eoru|€

while you can adjust this settings, module comes with predefined configuration that cover all
usable currencies at the time of this writing, so for the most part you may skip any 
configuration.

4. START GUIDE.
After enabling this module you need to do two things to make it work fine:
a) Add currency selection block somewhere on the page (so customers can actually select currency
they want to use)
b) Do a currency sync, this will fetch available currencies list for all stores and their rates
related to store's base currency. To do so you have to run cron job called "magento_currency_sync"
from here [admin/build/cron/status]

You also can look which currency is available for each of the stores and their rates here
[admin/settings/magento/currency/rates], note that you CANNOT edit currency rates or their
availability from drupal side, if you need to change this - thou will have to change this in 
Magento backend. 

Maintainers
-------------------------------------------------------------------------------
Module is fully sponsored and maintained by Adyax.

Current maintainers:
Valeriy Sokolov - vsokolov@adyax.com
Ivan Tsekhmistro - itsekhmistro@adyax.com
