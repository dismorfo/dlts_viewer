<?php

/**
 * @file
 * Themes
 */

/**
 * Themes functions
 */
function theme_dlts_viewer_pager_button($arguments) {

  $is_pjax = dlts_viewer_is_pjax() ;

  $pjax = $is_pjax ? 'pjax' : '';

  $status = $arguments['active'] ? 'active' : 'inactive';

  $classes = array($pjax, 'paging' , $arguments['operation'], $status);

  $render_array = array(
    'markup' => array(
      '#theme' => 'html_tag',
      '#tag' => 'a',
      '#value' => '<span>' . $arguments['text'] . '</span>',
      '#attributes' => array(
        'title' => $arguments['text'],
        'class' =>  $classes,
        'href' => $arguments['url'],
      ),
    ),
  );
  return drupal_render($render_array);
}

function theme_dlts_viewer_navbar_item($variables = array()) {
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
  return '<li class="navbar-item">' . l(t('<span>@title</span>' , array('@title' => $variables['title'])), $url, $parts) . '</li>';
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

  if (isset($file['timestamp'] ) ) $file_uri = $file_uri . '?v=' . $file['timestamp'];

  /** Add Openlayers to the page */
  drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY ) ) ;

  if (!isset($file['djakota_width'])) {
    $djatoka = dlts_image_djatoka_request($file);
    if (!isset($file['djakota_width'])) {
      $file['djakota_width'] = $djatoka['width'];
      $file['djakota_height'] = $djatoka['height'];
      $file['djakota_levels'] = $djatoka['levels'];
      $file['djakota_dwtLevels'] = $djatoka['dwtLevels'];
      $file['djakota_compositingLayerCount'] = $djatoka['compositingLayerCount'];
    }
  }

  $openlayers_options = array(
      'service' => variable_get('dlts_viewer_djatoka_service', 'http://dl-imgdev.home.nyu.edu/adore-djatoka'),
      'imgMetadata' => array(
          'width' => $file['djakota_width'],
          'height' => $file['djakota_height'],
          'levels' => $file['djakota_levels'],
          'dwtLevels' => $file['djakota_dwtLevels'],
          'compositingLayerCount' => $file['djakota_compositingLayerCount'],

      ),
      'zoom' => isset($file['zoom']) ? $file['zoom'] : 1,
 );

  $json_data = json_encode($openlayers_options);

  $js_inline = '(function(O){O.DLTS.Page("' . $fid . '","' .  $file_uri . '",' . $json_data . ')})(OpenLayers);';

  $js_options = array(
      'group' => JS_DEFAULT,
      'type' => 'inline',
      'every_page' => FALSE,
      'weight' => 5,
      'scope' => 'footer',
      'cache' => TRUE,
      'defer' => TRUE,
 );

  drupal_add_js($js_inline, $js_options);

  $options = array(
    '@id' => $fid,
    '@uri' => $file_uri,
    '@width' => $file['djakota_width'],
    '@height' => $file['djakota_height'],
    '@levels' => $file['djakota_levels'],
    '@dwtLevels' => $file['djakota_dwtLevels'],
    '@layer' => $file['djakota_compositingLayerCount'],
  );

  return t('<div id="@id" class="dlts_image_map" data-uri="@uri" data-width="@width" data-height="@height" data-levels="@levels" data-dwtLevels="@dwtLevels" data-compositing-layer="@layer"></div>', $options);

}

