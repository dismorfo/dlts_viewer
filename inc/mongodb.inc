<?php

/**
 * @file
 * MongoDB.inc
 */

// $collection_name: dlts_book_page OR dlts_book_stitched_page
function dlts_viewer_mongodb_update_sequence($arguments, $collection_name = 'dlts_book_page') {
	$cursor = mongodb_collection($collection_name);
	$data = (array) $arguments;
  $query = array('sequence' => $data['sequence'], 'isPartOf' => $data['isPartOf']);
  // Find document and modify
  // See: https://docs.mongodb.org/manual/reference/method/db.collection.findAndModify/
  // See: http://php.net/manual/en/mongocollection.findandmodify.php
  $op = $cursor->findAndModify($query, array('$set' => $data), null,  array('sort' => array('rating' => 1), 'upsert' => true, 'new' => true));
  // We should have the current result document as an associative array.
  // if NULL, we have a problem, log error.
  if (!$op) {
  	watchdog('MongoDB update document', 'Unable to update book sequence page <strong>%sequence</strong> from book <strong>%identifier</strong>.', array('%identifier' => $arguments['isPartOf'], '%sequence' => $arguments['sequence']), WATCHDOG_ERROR);
  	return FALSE;
  }
  return $op;
}

function dlts_viewer_mongodb_new_sequence($arguments, $collection_name = 'dlts_book_page') {
	// make sure we let MongoDB assing the _id for the document.
	if (isset($arguments->_id)) {
		unset($arguments->_id);
	}
	// We use findAndModify with new flag to avoid record duplication (at cost of operation time)
	return dlts_viewer_mongodb_update_sequence($arguments, $collection_name);
}
