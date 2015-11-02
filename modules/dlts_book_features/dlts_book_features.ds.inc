<?php
/**
 * @file
 * dlts_book_features.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function dlts_book_features_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|dlts_book|metadata';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'dlts_book';
  $ds_fieldsetting->view_mode = 'metadata';
  $ds_fieldsetting->settings = array(
    'series_for_this_book' => array(
      'weight' => '7',
      'label' => 'above',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
  );
  $export['node|dlts_book|metadata'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|dlts_map|metadata';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'dlts_map';
  $ds_fieldsetting->view_mode = 'metadata';
  $ds_fieldsetting->settings = array(
    'field_notes_field' => array(
      'formatter_settings' => array(
        'ft' => array(
          'lb' => 'Notes',
        ),
      ),
    ),
  );
  $export['node|dlts_map|metadata'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function dlts_book_features_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'multivolume_by_identifier_select';
  $ds_field->label = 'multivolume by identifier select';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'block' => 'views|ff662681e3c55e01c76472b14059f973',
    'block_render' => '3',
  );
  $export['multivolume_by_identifier_select'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'multivolume_select';
  $ds_field->label = 'Multivolume select';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'block' => 'views|65adcdd84849d7419640ca9bef8a6b82',
    'block_render' => '3',
  );
  $export['multivolume_select'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'series_for_this_book';
  $ds_field->label = 'Series';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '*|*';
  $ds_field->properties = array(
    'block' => 'views|series_for_a_book-block',
    'block_render' => '3',
  );
  $export['series_for_this_book'] = $ds_field;

  return $export;
}
