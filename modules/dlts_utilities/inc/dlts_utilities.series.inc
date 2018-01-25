<?php

/*
 * Series getters. If you need any of this functionalities load with module_load_include
 * e.g., module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.book');
 */


function dlts_utilities_series_books_nids ( $node ) {

  $query = new EntityFieldQuery;
  
  $keys = array () ; 
  
  $result = $query
                  ->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_series_book')
                  ->fieldCondition('field_series', 'nid', array( $node->nid ) )
                  ->execute();
  
  if ( !empty ( $result['node'] ) ) {
    $keys = array_keys( $result['node'] ) ;
  }
  
  return $keys ;

}


function dlts_utilities_series_get_identifier ( $entity ) {

  $ewrapper = entity_metadata_wrapper('node', $entity) ;
  
  return $ewrapper->field_identifier->value() ;

}

function dlts_utilities_series_book_get_series ( $entity ) {

  $ewrapper = entity_metadata_wrapper('node', $entity) ;
  
  return $ewrapper->field_series->value() ;

}