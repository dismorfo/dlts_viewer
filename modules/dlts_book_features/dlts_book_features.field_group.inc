<?php
/**
 * @file
 * dlts_book_features.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dlts_book_features_field_group_info() {
  $field_groups = array();

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
      7 => 'field_geographic_coordinates',
      8 => 'field_geographic_subject',
      9 => 'field_language',
      10 => 'field_language_code',
      11 => 'field_number',
      12 => 'field_other_version',
      13 => 'field_page_count',
      14 => 'field_publication_date',
      15 => 'field_publication_date_text',
      16 => 'field_publication_location',
      17 => 'field_publisher',
      18 => 'field_sequence_count',
      19 => 'field_subject',
      20 => 'field_subtitle',
      21 => 'field_title',
      22 => 'field_topic',
      23 => 'field_volume',
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
  $field_groups['group_descriptive_metadata|node|dlts_book|form'] = $field_group;

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
    'weight' => '42',
    'children' => array(
      0 => 'field_creator',
      1 => 'field_description',
      2 => 'field_editor',
      3 => 'field_page_count',
      4 => 'field_publication_date',
      5 => 'field_read_order',
      6 => 'field_scan_date',
      7 => 'field_scan_order',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'element' => 'div',
        'show_label' => 0,
        'label_element' => 'div',
        'classes' => '',
        'attributes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_descriptive_metadata|node|dlts_book|metadata'] = $field_group;

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
    'children' => array(),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_essence|node|dlts_book|form'] = $field_group;

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
    'children' => array(),
    'format_type' => 'html-element',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
        'element' => 'div',
        'show_label' => 0,
        'label_element' => 'div',
        'attributes' => '',
      ),
    ),
  );
  $field_groups['group_essence|node|dlts_book|metadata'] = $field_group;

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
  $field_groups['group_structural_metadata|node|dlts_book|form'] = $field_group;

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
    'weight' => '40',
    'children' => array(
      0 => 'group_essence',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
        'element' => 'div',
        'show_label' => 0,
        'label_element' => 'div',
        'attributes' => '',
      ),
    ),
  );
  $field_groups['group_structural_metadata|node|dlts_book|metadata'] = $field_group;

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
  $field_groups['group_technical_metadata|node|dlts_book|form'] = $field_group;

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
    'weight' => '41',
    'children' => array(),
    'format_type' => 'html-element',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'element' => 'div',
        'show_label' => 0,
        'label_element' => 'div',
        'classes' => '',
        'attributes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_technical_metadata|node|dlts_book|metadata'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Descriptive Metadata');
  t('Essence');
  t('Structural Metadata');
  t('Technical Metadata');

  return $field_groups;
}
