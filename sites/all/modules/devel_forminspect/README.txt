Form inspect is a small module that enables developers to quickly view the contents of a form array.

Form inspect no longer depends on the devel module (http://drupal.org/project/devel). If you have devel installed and
configured to use krumo (http://krumo.sourceforge.net/), Form inspect will also use krumo.

Configuration
=============
1 - After enabling the module, go to Administer >> Site configuration >> Form inspect (admin/settings/forminspect) and 
    - enable 'Display form information'.
    - optionally enter form ids of forms that should be ignored. Enter one form id per line. * is a wildcard character.
2 - Form inspect requires the following access permissions Administer >> User administration >> Access control (admin/user/access):
    - the 'access devel information' permission.

Author
======
Heine Deelstra <hdeelstra@gmail.com>

Please use the project's issue tracker for support requests.