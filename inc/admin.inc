<?php

/**
 * @file
 * Admin page.
 */
function dlts_viewer_admin($form, &$form_state) {

  include_once 'djatoka.inc' ;

  $djatoka = dlts_viewer_djatoka_request(array('uri' => 'http://memory.loc.gov/gmd/gmd433/g4330/g4330/np000066.jp2'));

  if (isset($djatoka['error'] )) {
     drupal_set_message(t('Djatoka: Unable to request metadata from Djakota Jpeg 2000 Image Server. See: @message(code: @code)', array('@message' => $djatoka['message'], '@code' => $djatoka['code'] ) ), 'error' ) ;
  }

  $form['fileServer'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Server'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['fileServer']['dlts_viewer_fileserver_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Service path'),
    '#default_value' => variable_get('dlts_viewer_fileserver_host', 'http://dl-imgdev.home.nyu.edu/adore-djatoka' ) ,
    '#size' => 60,
    '#maxlength' => 256,
    '#description' => t("Set host name. <acronym title=\"for example\">e.g.</acronym>, http://dev-dl-pa.home.nyu.edu/files"),
    '#required' => TRUE,
 );

  $form['djatoka'] = array(
    '#type' => 'fieldset',
    '#title' => t('Djatoka Jpeg 2000 Image Server'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
 );

  $form['djatoka']['dlts_viewer_djatoka_service'] = array(
    '#type' => 'textfield',
    '#title' => t('Service path'),
    '#default_value' => variable_get('dlts_viewer_djatoka_service', 'http://dl-imgdev.home.nyu.edu/adore-djatoka'),
    '#size' => 60,
    '#maxlength' => 256,
    '#description' => t("Set host name. <acronym title=\"for example\">e.g.</acronym>, http://myhost.bobst.nyu.edu:8080/adore-djatoka"),
    '#required' => TRUE,
 );

  $form['openlayers'] = array(
    '#type' => 'fieldset',
    '#title' => t('OpenLayers'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
 );

  $available_files_lib = file_scan_directory(libraries_get_path('openlayers') . '/lib', '/.*\.js$/', array('recurse' => FALSE));

  $available_files_build = file_scan_directory(libraries_get_path('openlayers') . '/build', '/.*\.js$/', array('recurse' => FALSE));

  $available_files = array_merge( (array)$available_files_lib, (array) $available_files_build);

  $js = array();

  foreach ($available_files as $key => $value) {
    $js[$key] = $value->uri;
  }

  $form['openlayers']['dlts_viewer_openlayers_source'] = array(
      '#type' => 'radios',
      '#title' => t('Source filepath'),
      '#default_value' => variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/lib/OpenLayers.js'),
      '#options' => $js,
 );

  $form['#submit'][] = 'dlts_viewer_image_service_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Form submit handler for dlts_viewer admin settings form.
 */
function dlts_viewer_image_service_admin_settings_submit($form, &$form_state) {

  $form_state['values']['dlts_viewer_djatoka_service'] = rtrim($form_state['values']['dlts_viewer_djatoka_service'], '/');

  $form_state['values']['dlts_viewer_fileserver_host'] = rtrim($form_state['values']['dlts_viewer_fileserver_host'], '/');

}
