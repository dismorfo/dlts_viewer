<?php

/**
 * @file
 * MongoDB.inc
 */

// $collection_name: dlts_book_page OR dlts_book_stitched_page
function dlts_viewer_mongodb_update_sequence($arguments, $collection_name = 'dlts_books_page') {
  $cursor = mongodb_collection($collection_name);
  $data = (array) $arguments;
	// make sure we let MongoDB assing the _id for the document.
  if (isset($data['_id'])) {
    unset($data['_id']);
  }
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

function dlts_viewer_mongodb_new_sequence($arguments, $collection_name = 'dlts_books_page') {
  // We use findAndModify with new flag to avoid record duplication (at cost of operation time)
  return dlts_viewer_mongodb_update_sequence($arguments, $collection_name);
}

function dlts_viewer_mongodb_load_by_identifier($identifier, $collection_name) {
  $query = (object) array('identifier' => $identifier);
  $cursor = mongodb_collection($collection_name);
  $result = $cursor->findOne($query);
  return $result;
}

function dlts_viewer_mongodb_load_double($sequence, $isPartOf, $collection_name) {
  // prepare query
  $query = (object) array(
    'isPartOf' => $isPartOf,
    'sequence' => array(
      '$gt' => min($sequence) - 1,
      '$lt' => max($sequence) + 1,
    ),
  );

  $cursor = mongodb_collection($collection_name)->find($query);

  $cursor->sort(array('sequence' => 1));

  $data = array('records' => array());

  foreach ($cursor as $item) {
    $data['records'][] = $item;
  }

  return $data;

}

function dlts_viewer_mongodb_load_sequence($sequence, $isPartOf, $collection_name = 'dlts_books_page') {
  // make sure we have a number
  $sequence = (int) $sequence;
  // prepare query
  $query = (object) array('isPartOf' => $isPartOf, 'sequence' => $sequence);
  $cursor = mongodb_collection($collection_name);
  $record = $cursor->findOne($query);
  $data = array('records' => array($record));
  return $data;
}

function dlts_viewer_mongodb_load_thumbnails($isPartOf, $page = 0, $per_page = 10, $collection_name, $content_type, $image_style = 'viewer_thumbnails') {
  $collection_name = 'dlts_books_x';
  $arguments = array();
  $items = array();
  $query = (object) array('isPartOf' => $isPartOf);
  $cursor = mongodb_collection($collection_name)->find($query);
	// no need to continue if request does not have items.
  $count = $cursor->count();
	if ($cursor->count() > 0) {
    $style = image_style_load($image_style);
    // see image style viewer_thumbnails
    // http://{site}/admin/config/media/image-styles/edit/viewer_thumbnails
    $width = 200;
    $height = '';
    foreach ($style['effects'] as $effect) {
      if ($effect['name'] == 'image_scale') {
        $width = ($effect['data']['width']) ? $effect['data']['width'] : '';
        $height = ($effect['data']['height']) ? $effect['data']['height'] : '';
        break;
      }
    }
		// DLTSVIEWER-27
		// https://docs.mongodb.org/manual/reference/method/cursor.sort/
		$cursor->sort(array('sequence' => 1));
    $cursor->limit($per_page);
    $thumbnails = $cursor->skip($per_page * $page);
		foreach ($thumbnails as $thumbnail) {
      if (is_array($thumbnail['sequence'])) {
				$thumbnail['sequence'] = min($thumbnail['sequence']);
      }
      $thumbnail['url'] = url($content_type.'/' . $isPartOf . '/' . $thumbnail['sequence'], array('absolute' => TRUE));
      if (dlts_viewer_is_embed() && $collection_name == 'dlts_photo') {
        $thumbnail['url'] = url($thumbnail['url'], array('query' => array('embed' => 'true')));
      }
      // {scheme}://{server}{/prefix}/{identifier}/{region}/{size}/{rotation}/{quality}.{format}
      // http://localhost:9080/loris/princeton_aco000319_n000001_d.jp2/full/full/0/default.jpg
      $url = $thumbnail['data']['@id'] . "/full/$width,$height/0/default.jpg";
      $thumbnail['imageServeURI'] = url($url, array('external' => true, 'query' => $arguments));
      $items[] = $thumbnail;
    }
    return array('count' => $count, 'items' => $items);
	}
	return FALSE;
}
