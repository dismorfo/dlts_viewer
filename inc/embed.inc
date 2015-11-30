<?php

/**
 * @file
 * Embed
 */

/**
 * Supports the REST style embed
 */
function dlts_viewer_embed_using_id($dlts_book) {
  if (is_object($dlts_book )) {
    $bnode = $dlts_book ;
  }
  else {
    return MENU_NOT_FOUND;
  }
  return dlts_viewer_embed_response($bnode);
}

/**
 * Generate a response to a client oEmbed request.
 *
 * @return
 *   A keyed array to be converted into JSON
 *
 * @todo Test oembed_node for whether it's actually a node
 */
function dlts_viewer_embed_response($dlts_book, $pre_response = array()) {

  /*
   * Invoke the URL that was passed as if it was its own request.
  */
  $node = $dlts_book;

  if (!$node) {
    watchdog('dlts_viewer', 'An Unexpected Error Has Occurred. The given path: :path does not exist.', array(':path' => $pre_response['node_path']), WATCHDOG_ERROR);
    return array(
        'code' => MENU_NOT_FOUND,
        'msg' => t('An Unexpected Error Has Occurred. The given path does not exist.' ),
   );
  }

  switch ($node->type) {
    case 'dlts_book':
      $book = $node;
      break;

    case 'dlts_book_page':
    case 'dlts_book_stitched_page':
      $page = $node;

      /*
       * TODO: Fix assumption that all pages have a relationship with a book.
      */
      $field_book = field_get_items('node', $page, 'field_book');
      $book = node_load($field_book[0]['nid']);
      break;
  }

  $argument_defaults = array(
      'format' => 'JSON',
      'width' => 480,
      'height' => 640,
      'callback' => '',
      'iframe' => TRUE,
      'mode' => 'single_page',
 );

  $arguments = array_merge($argument_defaults, $pre_response);

  /*
   * Get book title
  * field is required
  */

  $field_title = field_get_items('node', $book, 'field_title');
  $title = $field_title[0]['safe_value'];

  /*
   * Get book creators
  * @TODO: fix this, same as metadata
  */

  $authors = array();

  $author  = '';

  foreach ((array) field_get_items('node', $book, 'field_author' ) as $author) {
    if (isset($author['safe_value'])) {
      $authors[] = $author['safe_value'];
    }
  }
  if (!empty($authors )) {
    $author = implode(', ', $authors);
  }
  else {
    $author = t('undefined');
  }

  /*
   * Author URL is not part of our collections, for now 'undefined'.
  */
  $author_url = t('undefined');

  /*
   * Get book representitive image
  */

  $field_representitive_image = field_get_items('node', $book, 'field_representative_image');
  $thumbnail_url = file_create_url($field_representitive_image[0]['uri']);


  if (!empty($arguments['uri']['modified'])) {
    $url = url($arguments['uri']['modified'], array('query' => array('oembed' => 'true'), 'absolute' => TRUE));
  }
  else {
    $url = url($arguments['uri']['request'], array('query' => array('oembed' => 'true')));
  }

  $response = array(
      'type' => 'rich',
      'version' => '1.0',
      'title' => $title,
      'author_name' => $author,
      'author_url' => $author_url,
      'provider_name' => t('NYU Digital Library Technology Services'),
      'provider_url' => t('http://dlib.nyu.edu/dlts/projects'),
      'cache_age' => 86400,
      'thumbnail_url' => $thumbnail_url,
      'thumbnail_width' => 100,
      'thumbnail_height' => 154,
      'width' => (int) $arguments['width'],
      'height' => (int) $arguments['height'],
 );

  if ($arguments['iframe']) {
    $response['html'] = theme('dlts_viewer_oembed', array(
        'url' => $url,
        'width' => $arguments['width'],
        'height' => $arguments['height'],
    ));
  }
  return $response;
}
