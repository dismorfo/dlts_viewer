<?php

/**
 * @file
 * Wildcard loaders
 */

/**
 * A wildcard loader for dlts_book nodes, using the book identifier.
 *
 * @parameter $identifier
 *  The value of field_identifier for a dtls_book node.
 *
 * @return
 * A node object with the given value for field_identifier.
 *
 * @todo
 * field_identifier is supposed to be unique across all books, but we do
 * no checking of this assumption.
 * @todo
 * Does the query need to be tagged as node_access? We're checking node_access
 * via the menu system, but since the loader is called before access is
 * checked, adding a check here might save an entity_load if the user's not
 * going to have access anyway, at the cost of an extra JOIN here.
 */
function dlts_book_load($identifier) {

  $language = LANGUAGE_NONE ;

  $query_parameters = drupal_get_query_parameters() ;

  if (isset($query_parameters['lang'] )) {
    $language = filter_xss($query_parameters['lang'] ) ;
  }

  $query = new EntityFieldQuery;

  $identifier = filter_xss($identifier) ;

  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_book')
                  ->propertyCondition('status', 1) // only load books that are published
                  // ->propertyCondition('language', $language) // fix this
                  ->fieldCondition('field_identifier', 'value', $identifier, '=')
                  ->execute();

  if (! empty($result['node'] )) {

    $keys = array_keys($result['node'] ) ;

    $nid = array_pop($keys ) ;

    return node_load($nid ) ;

  }

  return FALSE ;

 }

/**
 * A wildcard loader for dlts_book pages data from MongoDB, using the book identifier and $page.
 *
 * @param unknown $identifier
 *  xxx
 *
 * @param unknown $page
 *  xxx
 *
 * @return multitype:unknown number
 *  xxx
 */
function dlts_book_page_load($sequence) {

  $identifier = arg(1 ) ;

  $collections = array('single' => 'dlts_book_page' , 'double' => 'dlts_book_stitched_page' ) ;

  $query_parameters = drupal_get_query_parameters() ;

  $collection_name = 'dlts_book_page';

  if (isset($query_parameters['page_view'] )) {
    if (array_key_exists($query_parameters['page_view'] , $collections )) {
      $collection_name = $collections[$query_parameters['page_view']];
    }
  }

  $query =(object) array('isPartOf' => $identifier , 'sequence' => $sequence ) ;

  $result = mongodb_collection($collection_name )->findOne($query ) ;

  if (! $result ) return FALSE ;

  $cm_uri = file_create_url($result['cm']['uri'] ) ;

  return array(
    'fid' => (string) $result['_id'] ,
    'sequence' => $result['sequence'] ,
    'uri' => $cm_uri ,
    'djakota_width' => $result['cm']['width'] ,
    'djakota_height' => $result['cm']['height'] ,
    'djakota_levels' => $result['cm']['levels'] ,
    'djakota_dwtLevels' => $result['cm']['dwtLevels'] ,
    'djakota_compositingLayerCount' => $result['cm']['compositingLayerCount'] ,
    'timestamp' => $result['cm']['timestamp'] ,
  ) ;

}
