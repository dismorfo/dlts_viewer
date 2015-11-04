<?php

/**
 * @file
 * Admin page.
 */
function dlts_viewer_admin_fileserver ( $form, &$form_state ) {

  $form['fileServer'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Server'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $base_url = $GLOBALS['base_url'] . '/' . variable_get('file_public_path', conf_path() . '/files') ;

  $form['fileServer']['dlts_viewer_fileserver_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Service path'),
    '#default_value' => variable_get('dlts_viewer_fileserver_host', $base_url ) ,
    '#size' => 60,
    '#maxlength' => 256,
    '#description' => t("Set host name. <acronym title=\"for example\">e.g.</acronym>, http://dev-dl-pa.home.nyu.edu/files"),
    '#required' => TRUE,
  );

  // $form['#submit'][] = 'dlts_viewer_admin_fileserver_submit';

  return system_settings_form ( $form ) ;

}

/**
 * Form submit handler for Admin fileServer settings form.
 */
// function dlts_viewer_admin_fileserver_submit ( $form, &$form_state) { }
