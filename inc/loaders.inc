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
  $language = array('en', LANGUAGE_NONE);
  $query_parameters = drupal_get_query_parameters();
  if (isset($query_parameters['lang'])) {
    $language = array(filter_xss($query_parameters['lang']));
  }
  $query = new EntityFieldQuery;
  $identifier = filter_xss($identifier);
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->propertyCondition('status', 1) // only load books that are published
    ->propertyCondition('language', $language)
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
  if (!empty($result['node'])) {
    $keys = array_keys($result['node']);
    $nid = array_pop($keys);
    return node_load($nid);
  }
  return FALSE;
 }

/**
 * A wildcard loader for dlts_book pages data from MongoDB, using the book.
 * identifier and $sequence.
 *
 * @param unknown $sequence
 *
 * @return object MongoDB book page object
 */
function dlts_book_page_load($sequence) {
  // I don't like this. Can we find a better solution for this?
  // investigate a way to pass this from hook_menu
  // @TODO: Make it so that it can be call as API
	$identifier = arg(1);
	// view modes, we map the views with their machine names in MongoDB
  $collections = array(
    'single' => 'dlts_book_page',
  	'double' => 'dlts_book_stitched_page',
  );

  // viewer defaults to single page view
  $collection_name = 'dlts_book_page';
  // user can requests a view modes, get query parameters and
  // check if page_view is set
  $query_parameters = drupal_get_query_parameters();
  if (isset($query_parameters['page_view'])) {
  	// check if the requested view mode is valid
    if (array_key_exists($query_parameters['page_view'], $collections)) {
      $collection_name = $collections[$query_parameters['page_view']];
    }
  }
  // prepare query
  $query = (object) array('isPartOf' => $identifier, 'sequence' => $sequence);
  // query MongoDB
  $result = mongodb_collection($collection_name)->findOne($query);

  if ($result) {
    return array(
      'fid' => (string) $result['_id'], // do we need this?
      'sequence' => $result['sequence'],
      'uri' => file_create_url($result['cm']['uri']),
      'width' => $result['cm']['width'],
      'height' => $result['cm']['height'],
      'levels' => $result['cm']['levels'],
      'dwtLevels' => $result['cm']['dwtLevels'],
      'compositingLayerCount' => $result['cm']['compositingLayerCount'],
      'timestamp' => $result['cm']['timestamp'],
    );
  }

  // if $result is empty, retrun FALSE so that hook_menu can 404 redirect
  return FALSE;
}
