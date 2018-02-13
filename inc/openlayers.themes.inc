<?php

function theme_dlts_viewer_openlayers_image($variables) {

  $id = 'id-' . $variables['_id'];

  $module = drupal_get_path('module', 'dlts_viewer');

  drupal_add_css($module . '/css/dlts_viewer.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));

  drupal_add_js('https://code.jquery.com/jquery-3.3.1.min.js', 'external');

  $files = array(
    'js/jquery/jquery.once.js',
    'js/openseadragon-bin-2.3.1/openseadragon.js',
    'js/doT.min.js',
    'js/EventEmitter.min.js',
    'js/lodash.js',
    'js/app.js',
  );

  foreach ($files as $file) {
    drupal_add_js($module . '/' . $file, 'file');
  }

  if (!isset($file['thumbnailsUrl'])) {
    $variables['thumbnailsUrl'] = 'undefined';
  }

  if (!isset($variables['thumbnailsRows'])) {
    $variables['thumbnailsRows'] = 0;
  }

  if (!isset($variables['thumbnailsPage'])) {
    $variables['thumbnailsPage'] = 0;
  }

  if (!isset($variables['rotation'])) {
    $variables['rotation'] = 0;
  }

  if (!isset($variables['preserveViewport'])) {
    $variables['preserveViewport'] = 'true';
  }

  if (!isset($variables['degrees'])) {
    $variables['degrees'] = 0;
  }

  if (!isset($variables['sequenceMode'])) {
    $variables['sequenceMode'] = 'false';
  }

  return theme('dlts_viewer_dragon', 
    array(
      'dragon' => 
        array(
          'id' => $id,
          'class' => 'dlts_viewer_map',
          'data-title' => $variables['title'],
          'data-identifier' => $variables['isPartOf'],
          'data-tiles' => drupal_json_encode($variables['tiles']),
          'data-pagev-view' => $variables['pageView'],
          'data-sequence-count' => $variables['sequenceCount'],
          'data-sequence' => $variables['sequence'],
          'data-sequence-mode' => $variables['sequenceMode'],
          'data-preserve-viewport' => $variables['preserveViewport'],
          'data-rotation' => $variables['rotation'],
          'data-degrees' => $variables['degrees'],
          'data-thumbnails-url' => $variables['thumbnailsUrl'],
          'data-thumbnails-rows' => $variables['thumbnailsRows'],
          'data-thumbnails-page' => $variables['thumbnailsPage'],
        )
    )
  );

}
