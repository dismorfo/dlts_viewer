<?php

/**
 * collection getters. If you need any of this functionalities load with module_load_include
 * e.g., module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.collection');
 */

/** Returns book identifier */
function dlts_utilities_collection_get_identifier ($node) {

  $ewrapper = entity_metadata_wrapper('node', $node);
  
  return $ewrapper->field_identifier->value();
  
}

/** Returns book identifier */
function dlts_utilities_collection_get_books_by_identifier ($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_book')
                  ->fieldCondition('field_identifier', 'value', array($identifier))
                  ->execute();
  
  if ( !empty( $result['node'] ) ) {
    $keys = array_keys( $result['node'] );
    return node_load($keys);
  }

}

function dlts_utilities_collection_get_collection_by_identifier ( $identifier ) {

  $query = new EntityFieldQuery;
  
  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_collection')
                  ->fieldCondition('field_identifier', 'value', $identifier, '=')
                  ->execute();
  
  if ( ! empty ( $result['node'] ) ) {
    $keys = array_keys( $result['node'] );
    
    return node_load ( array_shift( $keys ) );
  }

}

function dlts_utilities_collection_get_collection_code ( $identifier ) {

  $entity = dlts_utilities_collection_get_collection_by_identifier ( $identifier ) ;

  $ewrapper = entity_metadata_wrapper('node', $entity) ;
  
  return $ewrapper->field_code->value() ;

}
