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
    'mv_2016' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'show_title' => 0,
        'title_wrapper' => '',
        'ctools' => 'a:3:{s:4:"conf";a:16:{s:23:"override_pager_settings";i:0;s:9:"use_pager";i:0;s:14:"nodes_per_page";s:3:"100";s:8:"pager_id";s:1:"0";s:6:"offset";s:1:"0";s:9:"more_link";i:0;s:10:"feed_icons";i:0;s:10:"panel_args";i:0;s:12:"link_to_view";i:0;s:4:"args";s:0:"";s:3:"url";s:0:"";s:7:"display";s:7:"default";s:7:"context";a:1:{i:0;s:42:"argument_entity_id:node_1.field_identifier";}s:14:"override_title";i:0;s:19:"override_title_text";s:0:"";s:22:"override_title_heading";s:2:"h2";}s:4:"type";s:5:"views";s:7:"subtype";s:2:"mv";}',
        'load_terms' => 0,
        'ft' => array(),
      ),
    ),
    'series_for_this_book' => array(
      'weight' => '6',
      'label' => 'above',
      'format' => 'default',
    ),
    'title' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
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
  $ds_field->field = 'mv_2016';
  $ds_field->label = 'Multivol Select 2016';
  $ds_field->field_type = 7;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array(
    'default' => array(),
    'settings' => array(
      'show_title' => array(
        'type' => 'checkbox',
      ),
      'title_wrapper' => array(
        'type' => 'textfield',
        'description' => 'Eg: h1, h2, p',
      ),
      'ctools' => array(
        'type' => 'ctools',
      ),
    ),
  );
  $export['mv_2016'] = $ds_field;

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
        0 => 'mv_2016',
        1 => 'title',
        2 => 'field_subtitle',
        3 => 'field_author',
        4 => 'field_publication_location',
        5 => 'field_publisher',
        6 => 'series_for_this_book',
        7 => 'field_publication_date_text',
        8 => 'field_subject',
        9 => 'field_call_number',
        10 => 'field_collection',
        11 => 'field_language',
        12 => 'field_partner',
        13 => 'field_handle',
        14 => 'field_pdf_file',
        15 => 'field_rights',
      ),
    ),
    'fields' => array(
      'mv_2016' => 'ds_content',
      'title' => 'ds_content',
      'field_subtitle' => 'ds_content',
      'field_author' => 'ds_content',
      'field_publication_location' => 'ds_content',
      'field_publisher' => 'ds_content',
      'series_for_this_book' => 'ds_content',
      'field_publication_date_text' => 'ds_content',
      'field_subject' => 'ds_content',
      'field_call_number' => 'ds_content',
      'field_collection' => 'ds_content',
      'field_language' => 'ds_content',
      'field_partner' => 'ds_content',
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
    'layout_disable_css' => FALSE,
  );
  $export['node|dlts_book|metadata'] = $ds_layout;

  return $export;
}
