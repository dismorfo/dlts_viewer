<?php

/**
 * @file
 * Needs work. Not a priority at the moment
 */

function dlts_viewer_book($node, $list_pages = TRUE) {
  $book = dlts_viewer_book_metadata($node);
  if ($list_pages) {
    $book =  array_merge((array) $book, (array) dlts_viewer_book_pages($node));
  }
  return $book;
}

function dlts_viewer_books($start = 0, $length = 25) {
  global $language;
  $request_uri = drupal_parse_url(request_uri());
  if (isset($request_uri['query'])) {
    if (isset($request_uri['query']['rows']) && is_numeric($request_uri['query']['rows'])) {
      $length = $request_uri['query']['rows'];
    }
    if (isset($request_uri['query']['start']) && is_numeric($request_uri['query']['start'])) {
      $start = $request_uri['query']['start'];
    }
  }
  $count_query = new EntityFieldQuery();
  $entities_query = new EntityFieldQuery();
  $count = $count_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->propertyCondition('status', 1)
    ->count()
    ->execute();
  $entities = $entities_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->propertyCondition('status', 1)
    ->range($start, $length)
    ->execute();
  $keys = array_keys($entities['node']);
  $nodes = entity_load('node', $keys);
  // try to mimic Apache Solr response
  $response = array(
    'response' => array(
      'numFound' => $count,
      'start' => $start,
      'docs' => array(),
    )
  );
  foreach ((array) $nodes as $node) {
    $response['response']['docs'][] = dlts_viewer_book($node, FALSE);
  }
  return $response;
}

/**
 * Generate metadata about a book.
 *
 * @param $dlts_book
 * A dlts_book node object
 * @return
 * A keyed array to be converted into JSON
 *
 */
function dlts_viewer_book_metadata($book) {
	if (!is_object($book)) {
    return array('argument' => $book, 'code' => 1, 'message' => t('Invalid book'), 'status' => t('fail') );
	}
	module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.book');
	module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.collection');
  $type = dlts_utilities_content_types_type($book->type);
  $fields = array();
  $metadata = array();
  $subjects = array();
  $collections = array();
  $ewrapper = entity_metadata_wrapper('node', $book);
  foreach ($type['fields'] as $field) {
    $name = str_replace('field_', '', $field['field_name']);
    $raw_value = $ewrapper->$field['field_name']->value();
    if ($field['type'] == 'options_buttons') {
      $value = $field['settings']['allowed_values'][$raw_value];
    }
    elseif ($field['type'] == 'link_field' && isset($raw_value['url'] )) {
      $value = $raw_value['url'];
    }
    elseif ($field['type'] == 'file_generic') {
      $value = array();
      foreach ((array) $raw_value as $pdf_file) {
        $value[] = file_create_url($pdf_file['uri']);
      }
    }
    elseif ($field['type'] == 'date_text') {
      $raw_value['value'];
    }
    elseif ($field['type'] == 'image_image') {
      $value = file_create_url($raw_value['uri']);
    }
    elseif ($field['type'] == 'taxonomy_autocomplete') {
      $value = array();
      foreach ($raw_value as $taxonomy) {
        $value[] = $taxonomy->name;
      }
    }
    else {
      $value = $raw_value;
    }
    $fields[$name] = array('label' => $field['label'], 'value' => (array) $value);
  }
  return array(
    //'entity_title' => !empty($title) ? $title : html_entity_decode($book->title,ENT_QUOTES),
    'entity_title' => 'Hyi',
    'identifier' => $fields['identifier']['value'][0],
    'entity_path' => url('books/' . $identifier . '/1', array('absolute' => TRUE)),
    'base_url' => url('', array('absolute' => TRUE)),
    'entity_language' => $book->language,
    'entity_status' => $book->status,
    'entity_type' => $book->type,
    'metadata' => $fields,
  );
}
