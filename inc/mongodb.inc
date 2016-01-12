<?php

/**
 * @file
 * MongoDB.inc
 */

// $collection_name: dlts_book_page OR dlts_book_stitched_page
function dlts_viewer_mongodb_update_sequence($arguments, $collection_name = 'dlts_book_page') {
	$cursor = mongodb_collection($collection_name);
  $timestamp = ($arguments['cm']['timestamp']) ? $arguments['cm']['timestamp'] : time() ;
  $data = array(
    'isPartOf' => $arguments['isPartOf'],
  	'sequence' => $arguments['sequence'],
  	'realPageNumber' => $arguments['realPageNumber'],
  	'cm' => array(
      'uri' => $arguments['cm']['uri'],
  		'width' => $arguments['cm']['width'],
  		'height' => $arguments['cm']['height'],
  		'levels' => $arguments['cm']['levels'],
  		'dwtLevels' => $arguments['cm']['dwtLevels'],
  		'compositingLayerCount' => $arguments['cm']['compositingLayerCount'],
  		'timestamp' => $timestamp,
  	),
  );
  // Find document and modify
  // See: https://docs.mongodb.org/manual/reference/method/db.collection.findAndModify/
  // See: http://php.net/manual/en/mongocollection.findandmodify.php
  $op = $cursor->findAndModify($query, array('$set' => $data), null,  array( 'sort' => array('rating' => 1), 'new' => true, 'upsert' => true));
  // We should have the current result document as an associative array.
  // if NULL, we have a problem, log error.
  if (!$op) {
  	watchdog('MongoDB update document', 'Unable to update book sequence page <strong>%sequence</strong> from book <strong>%identifier</strong>.', array('%identifier' => $arguments['isPartOf'], '%sequence' => $arguments['sequence']), WATCHDOG_ERROR);
  }
}

