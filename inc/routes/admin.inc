<?php

function _dlts_viewer_menu_admin() {

  $menu = array(
    /**
     * Admin page to administer the fileServer and the
     * fileServer stream wrapper
     */
    'admin/config/media/viewer' => array(
      'title' => 'Administer and configure viewer',
      'description' => 'Administer and configure viewer.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('dlts_viewer_admin'),
      'access arguments' => array('administer fileserver'),
      'file' => DLTS_VIEWER_MODULE . '/inc/admin.inc',
    ),
  );

  return $menu;

}
