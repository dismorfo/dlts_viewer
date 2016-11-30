<?php

function _dlts_viewer_menu_photo() {
  $menu = array(
    /** Render photo object */
    'photos/%dlts_photo' => array(
      'title' => 'Photo page',
      'access arguments' => array('access content'),
      'page callback' => 'dlts_viewer_photo_dispatcher',
      'page arguments' => array(1),
      'delivery callback' => 'drupal_deliver_html_page',
      'type' => MENU_CALLBACK,
    ),
  );
  return $menu;
}
