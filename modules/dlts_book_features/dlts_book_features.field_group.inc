<?php
/**
 * @file
 * dlts_book_features.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dlts_book_features_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_descriptive_metadata|node|dlts_book|form';
  $field_group->group_name = 'group_descriptive_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Descriptive Metadata',
    'weight' => '10',
    'children' => array(
      0 => 'field_author',
      1 => 'field_call_number',
      2 => 'field_contributor',
      3 => 'field_creator',
      4 => 'field_description',
      5 => 'field_dimensions',
      6 => 'field_editor',
      7 => 'field_language',
      8 => 'field_language_code',
      9 => 'field_number',
      10 => 'field_other_version',
      11 => 'field_page_count',
      12 => 'field_publication_date',
      13 => 'field_publication_location',
      14 => 'field_publisher',
      15 => 'field_sequence_count',
      16 => 'field_subject',
      17 => 'field_subtitle',
      18 => 'field_title',
      19 => 'field_topic',
      20 => 'field_volume',
      21 => 'field_publication_date_text',
      22 => 'field_geographic_coordinates',
      23 => 'field_geographic_subject',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_descriptive_metadata|node|dlts_book|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_descriptive_metadata|node|dlts_book|metadata';
  $field_group->group_name = 'group_descriptive_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'metadata';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Descriptive Metadata',
    'weight' => '0',
    'children' => array(
      0 => 'field_author',
      1 => 'field_contributor',
      2 => 'field_creator',
      3 => 'field_description',
      4 => 'field_dimensions',
      5 => 'field_editor',
      6 => 'field_handle',
      7 => 'field_is_part_of_collection',
      8 => 'field_language',
      9 => 'field_pdf_file',
      10 => 'field_publication_date',
      11 => 'field_publisher',
      12 => 'field_representative_image',
      13 => 'field_subject',
      14 => 'field_subtitle',
      15 => 'field_title',
      16 => 'field_publication_location',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Descriptive Metadata',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_descriptive_metadata|node|dlts_book|metadata'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_essence|node|dlts_book|form';
  $field_group->group_name = 'group_essence';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Essence',
    'weight' => '9',
    'children' => array(
      0 => 'field_ocr_text',
      1 => 'field_pdf_file',
      2 => 'field_representative_image',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_essence|node|dlts_book|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_essence|node|dlts_book|metadata';
  $field_group->group_name = 'group_essence';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'metadata';
  $field_group->parent_name = 'group_structural_metadata';
  $field_group->data = array(
    'label' => 'Essence',
    'weight' => '1',
    'children' => array(
      0 => 'field_ocr_text',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_essence|node|dlts_book|metadata'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_structural_metadata|node|dlts_book|form';
  $field_group->group_name = 'group_structural_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Structural Metadata',
    'weight' => '11',
    'children' => array(
      0 => 'field_binding_orientation',
      1 => 'field_read_order',
      2 => 'field_scan_order',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_structural_metadata|node|dlts_book|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_structural_metadata|node|dlts_book|metadata';
  $field_group->group_name = 'group_structural_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'metadata';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Structural Metadata',
    'weight' => '2',
    'children' => array(
      0 => 'field_read_order',
      1 => 'field_scan_order',
      2 => 'group_essence',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_structural_metadata|node|dlts_book|metadata'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_technical_metadata|node|dlts_book|form';
  $field_group->group_name = 'group_technical_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Technical Metadata',
    'weight' => '12',
    'children' => array(
      0 => 'field_scan_date',
      1 => 'field_scanning_notes',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_technical_metadata|node|dlts_book|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_technical_metadata|node|dlts_book|metadata';
  $field_group->group_name = 'group_technical_metadata';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'dlts_book';
  $field_group->mode = 'metadata';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Technical Metadata',
    'weight' => '17',
    'children' => array(
      0 => 'field_scan_date',
      1 => 'field_scanning_notes',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(),
    ),
  );
  $export['group_technical_metadata|node|dlts_book|metadata'] = $field_group;

  return $export;
}
