<?php

/**
 * @file
 * photo.inc
 */

function dlts_viewer_full_photo_view(&$entity) {

	$module_path = drupal_get_path('module', 'dlts_viewer');

	$js_common_config = array('type' => 'file', 'scope' => 'footer');

	$css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);

	drupal_add_css($module_path . '/css/dlts_viewer.css', $css_common_config);

	drupal_add_js('http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js', 'external', array('group' => JS_LIBRARY, 'weight' => -100));

	drupal_add_js($module_path . '/js/crossframe.js', $js_common_config);

	drupal_add_js($module_path . '/js/modules/slider-base.js', $js_common_config);

	drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);

	drupal_add_js($module_path . '/js/ui.components.yui.js', $js_common_config);

	drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY));

	$data_attributes = array(
		'uri' => $entity->data['uri'],
		'pageView' => 'single',
		'sequenceCount' => 1,
		'sequence' => 1,
		'title' => $entity->title,
		'width' => $entity->data['width'],
		'height' => $entity->data['height'],
		'levels' => $entity->data['levels'],
		'dwtLevels' => $entity->data['dwtLevels'],
		'compositingLayerCount' => $entity->data['compositingLayerCount'],
		'thumbnails-url' => $entity->thumbnailsURL,
		'thumbnails-rows' => $entity->thumbnailsRows,
		'thumbnails-page' => $entity->thumbnailsPage,
	);

	$vars = array(
    'compositingLayerCount' => $entity->data['compositingLayerCount'],
    'dwtLevels' => $entity->data['dwtLevels'],
    'fid' => $entity->data['identifier'],
    'height' => $entity->data['height'],
    'identifier' => $entity->identifier,
    'levels' => $entity->data['levels'],
    'thumbnailsPage' => $entity->thumbnailsPage,
    'thumbnailsRows' => $entity->thumbnailsRows,
    'thumbnailsURL' => $entity->thumbnailsURL,
    'title' => $entity->title,
    'uri' => $entity->data['uri'],
    'width' => $entity->data['width'],
  	'pageView' => 'single',
		'data_attributes' => $data_attributes,
  );

  $viewer = array(
	  'slider' => $entity->slider,
    'book_page' => $entity->map,
		'map' => theme('viewer_map', $vars),
		'language_dir' => $entity->lang_dir,
		'pagemeta' => NULL,
		'navbar' => $entity->navbar,
		'url' => $entity->url,
		'identifier' => $entity->identifier,
		'title' => $entity->title,
		'loading' => $entity->loading,
	);

  return theme('viewer', $viewer);

}

function dlts_viewer_pjax_photo_view(&$entity) {
  return theme('viewer_page', array());
}

function dlts_viewer_photo_dispatcher($entity) {

	$sequence = array(1); // for now

  $sequence_count = count($sequence);

	$sequence_number = array_shift($sequence);

	// check request type
	$is_pjax = dlts_viewer_is_pjax();

	// get query parameters
	$query_parameters = drupal_get_query_parameters();

	// type of request
  $display_type = 'photo_page';

	if ($is_pjax) {
  	$display_type = 'photo_pjax';
  }

	$data = $entity;

	$entity = (object) $entity;

	$entity->data = $data;

	$entity->lang_dir = dlts_viewer_viewer_language();

  // set entity title
	drupal_set_title($entity->identifier);

	$entity->title = $entity->identifier;

	// Book URL. Set $url so that .tpl.php can use it
	$entity->url = url('photos/' . $entity->identifier, array('absolute' => TRUE));

	$thumbnails_rows_per_page = 10;

	if (isset($query_parameters['rows'])) {
  	$thumbnails_rows_per_page = filter_xss($query_parameters['rows']);
  }

	$button_fullscreen_options = array(
  	'title' => t('Toggle fullscreen'),
  	'path' => $entity->url,
  	'attributes' => array(
  		'data-title' => t('Toggle fullscreen'),
  		'title' => t('Toggle fullscreen'),
  		'class' => array('button', 'fullscreen', 'off'),
  		'id' => array('button-fullscreen')
  	),
  	'fragment' => 'fullscreen',
  );

	$button_thumbnails_options = array(
  	'title' => t('Pages'),
  	'path' => $entity->url,
  	'attributes' => array(
  		'data-title' => t('Select page'),
  		'title' => t('Select page'),
  		'class' => array('button', 'thumbnails', 'off'),
  		'id' => array('button-thumbnails')
  	),
  	'fragment' => 'thumbnails',
  );

  /** fullscreen button */
  $button_fullscreen = theme('dlts_viewer_navbar_item', $button_fullscreen_options);

  /** thumbnails button */
  $button_thumbnails = theme('dlts_viewer_navbar_item', $button_thumbnails_options);

	$controlpanel_button = theme('dlts_viewer_controlpanel');

	$navbar_options = array(
  	'nav_buttons_left' => array($button_thumbnails),
  	'nav_buttons_right' => array($button_fullscreen),
  	'control_panel' => $controlpanel_button,
  );

	$loading_options = array('sequence_number' => 1);

  $entity->navbar = theme('dlts_viewer_navbar', $navbar_options);

	$entity->loading = theme('dlts_viewer_loading', $loading_options);

	$slider_options = array(
		'slider_id' => 'slider',
		'uri' => $entity->url,
		'lang_dir' => $entity->lang_dir,
		'sequence_number' => $sequence_number,
		'sequence_count' => $sequence_count
	);

	$entity->thumbnailsPage = ceil($sequence_number / $thumbnails_rows_per_page) - 1;

	$entity->thumbnailsRows = $thumbnails_rows_per_page;

	$entity->thumbnailsURL = $entity->url . '/thumbnails';

	$data['thumbnailsUrl'] = $entity->thumbnailsURL;

	$data['thumbnailsRows'] = $entity->thumbnailsRows;

	$data['thumbnailsPage'] = $entity->thumbnailsPage;

	$data['sequence'] = $sequence;

	$book_page_options = array(
		'file' => $data
	);

	$entity->map = theme('dlts_viewer_openlayers_image', $book_page_options);

	$entity->slider = theme('dlts_viewer_slider', $slider_options);

  // types of returns
  switch ($display_type) {
  	case 'photo_page':
  		return dlts_viewer_full_photo_view($entity);
  	  break;
  	case 'photo_pjax':
  		return dlts_viewer_pjax_photo_view($entity);
  		break;
  }

}

function dlts_viewer_preprocess_html_photos(&$vars) {
  dlts_viewer_preprocess_html_photo($vars);
}

function dlts_viewer_preprocess_html_photo(&$vars) {
  $vars['classes_array'][] = 'photo';
}
