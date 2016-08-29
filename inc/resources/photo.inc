<?php

/**
 * @file
 * photo.inc
 */

function dlts_viewer_photo_dispatcher($entity, $arg = array()) {

  $arg = $entity;

	$entity = (object) $entity;

	$entity->title = 'Photo';

	// check request type
 	$is_pjax = dlts_viewer_is_pjax();

	// get query parameters
 	$query_parameters = drupal_get_query_parameters();

	// set entity title
 	drupal_set_title($entity->title);

	// wrapp book entity with Entity API wrapper
 	// check for type of request: 1) book page or 2) display pane
  $display_type = 'photo_page';

  if ($is_pjax) {
    $display_type = 'photo_page_pjax';
  }

  /** fullscreen button */
  $button_fullscreen = theme('dlts_viewer_navbar_item', array(
    'title' => t('Toggle fullscreen'),
   	'path' => '_',
   	'attributes' => array(
   	  'data-title' => t('Toggle fullscreen'),
   		'title' => t('Toggle fullscreen'),
   		'class' => array('button', 'fullscreen', 'off'),
   		'id' => array('button-fullscreen')
   	),
   	'fragment' => 'fullscreen',
    )
  );

  /** thumbnails button */
  $button_thumbnails = theme('dlts_viewer_navbar_item', array(
    'title' => t('Pages'),
   	'path' => '_',
   	'attributes' => array(
   		'data-title' => t('Select page'),
   		'title' => t('Select page'),
   		'class' => array('button', 'thumbnails', 'off'),
   		'id' => array('button-thumbnails')
   	),
   	'fragment' => 'thumbnails',
   	)
  );

	$nav_buttons_left = array();

	$nav_buttons_arrows = array();

	$nav_buttons_pager_left_arrow = array();

	$nav_buttons_pager_right_arrow = array();

  $nav_buttons_right = array($button_fullscreen);

	$control_panel = theme('dlts_book_controlpanel');

  $entity->navbar = theme('dlts_book_navbar', array(
    'nav_buttons_left' => $nav_buttons_left,
   	'nav_buttons_arrows' => $nav_buttons_arrows,
    'nav_buttons_right' => $nav_buttons_right,
   	'nav_buttons_pager_left_arrow' => $nav_buttons_pager_left_arrow,
   	'nav_buttons_pager_right_arrow' => $nav_buttons_pager_right_arrow,
    'control_panel' => $control_panel,
  ));

  $entity->loading = theme('dlts_loading');

  $entity->map = theme('dlts_viewer_openlayers_image' , array('file' => $arg));

  // types of returns
  switch ($display_type) {
    case 'photo_page' :
      return dlts_viewer_full_photo_view($entity);
   	  break;
   	//case 'photo_page_pjax' :
   	//	return dlts_viewer_pjax_photo_view($entity);
   	//	break;
   }
}

function dlts_viewer_full_photo_view($entity) {

	$module_path = drupal_get_path('module', 'dlts_viewer');

	$js_common_config = array('type' => 'file', 'scope' => 'footer');

	$css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);

	drupal_add_js(DLTS_JS_FRAMEWORK, 'external', array('group' => JS_LIBRARY, 'weight' => -100));

	drupal_add_js($module_path . '/js/crossframe.js', $js_common_config);

	drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);

	drupal_add_js($module_path . '/js/ui.components.yui.js', $js_common_config);

	drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY));

  drupal_add_css($module_path . '/css/dlts_viewer.css', $css_common_config);

	//return node_view($entity , 'metadata', $entity->language);

	//return theme('photo_page', $entity);

  return array();

}

//function dlts_viewer_pjax_photo_view(&$entity) {}
