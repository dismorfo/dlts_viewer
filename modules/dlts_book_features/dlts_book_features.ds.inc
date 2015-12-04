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
      'label' => 'hidden',
      'format' => 'default',
    ),
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '0',
        'wrapper' => 'h2',
        'class' => '',
        'ft' => array(
          'func' => 'theme_ds_field_minimal',
        ),
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

/**
 * Implements hook_ds_layout_settings_info().
 */
function dlts_book_features_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|dlts_book|metadata';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'dlts_book';
  $ds_layout->view_mode = 'metadata';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'field_subtitle',
        2 => 'field_author',
        3 => 'field_publication_location',
        4 => 'field_publisher',
        5 => 'field_publication_date_text',
        6 => 'field_subject',
        7 => 'series_for_this_book',
        8 => 'field_language',
        9 => 'field_collection',
        10 => 'field_partner',
        11 => 'field_call_number',
        12 => 'field_handle',
        13 => 'field_pdf_file',
        14 => 'field_rights',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'field_subtitle' => 'ds_content',
      'field_author' => 'ds_content',
      'field_publication_location' => 'ds_content',
      'field_publisher' => 'ds_content',
      'field_publication_date_text' => 'ds_content',
      'field_subject' => 'ds_content',
      'series_for_this_book' => 'ds_content',
      'field_language' => 'ds_content',
      'field_collection' => 'ds_content',
      'field_partner' => 'ds_content',
      'field_call_number' => 'ds_content',
      'field_handle' => 'ds_content',
      'field_pdf_file' => 'ds_content',
      'field_rights' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|dlts_book|metadata'] = $ds_layout;

  return $export;
}
