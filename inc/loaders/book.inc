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
  /**
   * Load MongoDB helper
   */
  module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
  // find by isPartOf
  $isPartOf = filter_xss(arg(1));
  // viewer defaults page view
  $collection_name = 'dlts_books_x';
  // user can requests a view modes, get query parameters and
  // check if page_view is set
  $query_parameters = drupal_get_query_parameters();
  try {
    // view request?
    if (isset($query_parameters['page_view']) && $query_parameters['page_view'] == 'double') {
      $sequence = array($sequence, (($sequence % 2) === 1) ? ($sequence + 1) : ($sequence - 1));
      $result = dlts_viewer_mongodb_load_double($sequence, $isPartOf, $collection_name);
    }
    else {
      $result = dlts_viewer_mongodb_load_sequence($sequence, $isPartOf, $collection_name);
    }
    if ($result) {
      return $result;
    }
    else {
      /**
       * Log not found
       * @TODO: Admin option to show errors if debug mode TRUE?
       */
      drupal_set_message(t('<em>DLTS Viewer</em>: Sequence page <strong>@sequence</strong> from book identify by identifier <strong>@identifier</strong> not found.', array('@sequence' => $sequence, '@identifier' => $isPartOf)), 'error');
      watchdog('MongoDB sequence not found', 'Unable to find book sequence page <strong>%sequence</strong> from book <strong>%identifier</strong>.', array('%identifier' => $isPartOf, '%sequence' => $sequence), WATCHDOG_NOTICE);
    }
  } catch (Exception $e) {
    watchdog('MongoDB exception', $e->getMessage(), array(), WATCHDOG_ERROR);
  }

  // if $result is empty, retrun FALSE so that hook_menu can 404 redirect
  return FALSE;  

}
