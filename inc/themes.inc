<?php

/**
 * @file
 * Themes
 */

/**
 * Themes functions
 */
function theme_dlts_viewer_pager_button($variables) {
  $classes = array('paging');
  if (isset($variables['classes'])) {
    $classes = array_merge($classes, $variables['classes']);
  }
  if (dlts_viewer_is_pjax()) {
    $classes[] = 'pjax';
  }
  if (isset($variables['active'])) {
    $classes[] = 'active';
  }
  else {
    $classes[] = 'inactive';
  }

  $render_array = array(
    'markup' => array(
      '#theme' => 'html_tag',
      '#tag' => 'a',
      '#value' => '<span>' . $variables['text'] . '</span>',
      '#attributes' => array(
        'title' => $variables['text'],
        'href' => $variables['url'],
        'class' =>  $classes,
      ),
    ),
  );
  return drupal_render($render_array);
}

function theme_dlts_viewer_navbar_item($variables) {
  $parts = array('html' => TRUE);
  if (isset($variables['fragment'])) {
    $parts = array_merge($parts, array('fragment' => $variables['fragment']));
  }
  if (isset($variables['attributes'])) {
    $parts = array_merge($parts, array('attributes' => $variables['attributes']));
  }
  if (isset($variables['query'])) {
    $parts = array_merge($parts, array('query' => $variables['query']));
  }
  if (empty($variables['url'])) {
    $url = $variables['path'];
  }
  else {
    $url = $variables['url'];
  }
  return '<li class="navbar-item">' . l(t('<span>@title</span>', array('@title' => $variables['title'])), $url, $parts) . '</li>';
}

/**
 * Returns HTML for a high-resolution JPEG 2000 image.
 *
 * This function returns the necessary HTML elements, and sets certain
 * JavaScript settings for the file being displayed. The given elements are
 * then replaced by OpenLayers with a zoomable version of the image.
 *
 * @ingroup themable
 */
function theme_dlts_viewer_openlayers_image($variables) {
  $file = $variables['file'];
  $fid = 'id-' . $file['fid'];
  $file_uri = file_create_url($file['uri']);
  if (isset($file['timestamp'])) {
  	$file_uri = $file_uri . '?v=' . $file['timestamp'];
  }
  $openlayers_options = array(
    'service' => variable_get('dlts_viewer_djatoka_service', 'http://dl-imgdev.home.nyu.edu/adore-djatoka'),
    'imgMetadata' => array(
      'width' => $file['width'],
      'height' => $file['height'],
      'levels' => $file['levels'],
      'dwtLevels' => $file['dwtLevels'],
      'compositingLayerCount' => $file['compositingLayerCount'],
    ),
    'zoom' => isset($file['zoom']) ? $file['zoom'] : 1,
  );
  $json_data = json_encode($openlayers_options);
  $js_inline = '(function(O){O.DLTS.Page("' . $fid . '","' .  $file_uri . '",' . $json_data . ')})(OpenLayers);';
  $js_options = array('group' => JS_DEFAULT, 'type' => 'inline', 'every_page' => FALSE, 'weight' => 5, 'scope' => 'footer', 'cache' => TRUE, 'defer' => TRUE);
  drupal_add_js($js_inline, $js_options);
  $options = array(
    '@id' => $fid,
    '@uri' => $file_uri,
    '@width' => $file['width'],
    '@height' => $file['height'],
    '@levels' => $file['levels'],
    '@dwtLevels' => $file['dwtLevels'],
    '@layer' => $file['compositingLayerCount'],
  	'@pageView' => 'single',
  );
  return t('<div id="@id" class="dlts_image_map dlts_viewer_map" data-pageView="@pageView" data-uri="@uri" data-width="@width" data-height="@height" data-levels="@levels" data-dwtLevels="@dwtLevels" data-compositing-layer="@layer"></div>', $options);
}

