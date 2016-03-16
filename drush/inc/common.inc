<?php

/**
 * @file
 * DLTS Viewer drush
 */

function add_language($requested_language) {
  $list_enabled_languages = locale_language_list();
  $enabled_languages = array_keys($list_enabled_languages);
  if (in_array($requested_language, $enabled_languages)) {
    return TRUE;
  }
  require_once DRUPAL_ROOT . '/includes/iso.inc';
  // https://api.drupal.org/api/drupal/includes%21iso.inc/function/_locale_get_predefined_list/7
  $predefined_list = _locale_get_predefined_list();
  // https://api.drupal.org/api/drupal/includes%21locale.inc/function/locale_add_language/7    
  $language_to_install = $predefined_list[$requested_language];
  if (!$language_to_install) {
    drush_log(t('At this moment there is only support for auto-install languages defined by Drupal iso.inc' ), 'error');
    return FALSE;
  }
  locale_add_language($requested_language);
  return TRUE;
}
