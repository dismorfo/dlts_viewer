<?php

/**
 * @file
 * DLTS Viewer drush
 */
function create_dlts_series_book($node) {
  global $user;
  /** make sure the parent relationship exist */
  foreach ($node->isPartOf as $isPartOf) {
    $series_nid = series_nid($isPartOf->identifier);
  	if (!$series_nid) {
  		create_dlts_series($isPartOf);
  	}
  }
  
  $exist = series_book_nid($node->isPartOf[0]->identifier, $node->volume_number, $node->collection[0]->identifier);
  
  if ($exist) {
    // Load the node by NID
    $entity = node_load($exist);
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);
  }
  else {
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_series_book',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
      'language' => LANGUAGE_NONE, // make sure we only translate the book object
   );

    $entity = entity_create('node', $values);

    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);
  }
  
  $series_nid = multivol_nid($node->isPartOf[0]->identifier);

  $ewrapper->title->set($node->title);

  if ($node->collection) {
    $nid = collection_nid($node->collection[0]->identifier);
    if ($nid) {
      $ewrapper->field_collection->set(array($nid));
    }
  }
  
  if ($node->identifier) {
    $ewrapper->field_identifier->set($node->identifier);
  }
  else {
  	$ewrapper->field_identifier->set(uniqid());
  }

  if ($node->volume_number) {
    $ewrapper->field_volume_number->set((int)$node->volume_number);
  }
  
  if ($node->volume_number_str) {
    $ewrapper->field_volume_number_str->set($node->volume_number_str);
  }
  
  if ($node->book) {
    $book_exist = book_nid($node->book, 'en');
    if ($book_exist) {
      $ewrapper->field_book->set($book_exist);
    }
  }

  if ($series_nid) {
    $ewrapper->field_multivol->set($series_nid);
  }

  $ewrapper->save();

  return $entity;

}
