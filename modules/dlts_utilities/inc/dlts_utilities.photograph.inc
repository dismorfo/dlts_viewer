<?php

/*
 * Photograph getters. If you need any of this functionalities load with module_load_include
 * e.g., module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.photograph');
 */
function dlts_utilities_photograph_get_services_image_path($node) {

  $field = dlts_utilities_field_get_first_item('node', $node, 'field_service_copy');
  
  if (isset($field['uri'])) { 
    return array(
      'path' => drupal_realpath($field['uri']),
      'url' => file_create_url($field['uri'])
    );
  }
  else {
    return FALSE;
  }
}

function dlts_utilities_photograph_page_get_identifier($node) {

  $field = dlts_utilities_field_get_first_item('node', $node, 'field_identifier');

  if ( isset($field) && isset($field['safe_value'])) {

    return $field['safe_value'];

  }

}

function dlts_utilities_photograph_page_get_sequence_number($node) {
  $field = dlts_utilities_field_get_first_item('node', $node, 'field_sequence_number');

  if (isset($field) && isset($field['value'])) {

    return (int)$field['value'];

  }

}
function dlts_utilities_photograph_page_get_photo_set_node( $node ) {

  $field = dlts_utilities_field_get_first_item('node', $node, 'field_photo_set');

  if (isset($field) && isset($field['nid'])) {

    return node_load(array($field['nid']));

  }

}

function dlts_utilities_photograph_page_get_item_count($node) {
  return dlts_utilities_photo_set_get_item_count($node);
}

function dlts_utilities_photo_set_get_item_count ( $node ) {

  if ( $node->type = 'dlts_photograph' ) {
  
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $node ) ;
    
    $photo_set = $ewrapper->field_photo_set->value() ;
    
    $photo_set_wrapper = entity_metadata_wrapper('node', $photo_set ) ;
    
    return $photo_set_wrapper->field_item_count->value() ;
    
  }
}
