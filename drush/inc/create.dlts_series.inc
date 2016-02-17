<?php

/**
 * @file
 * DLTS Viewer drush
 */
function create_dlts_series($node) {
  global $user;
  $exists = series_nid($node->identifier);
  if (!$exists) {
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_series',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
      'language' => LANGUAGE_NONE, // make sure we only translate the book object
   );
    $entity = entity_create('node', $values);
  }
  else {
    $entity = node_load($exists);
  }
  $entity->exists = $exists;
  // The entity is now created, but we have not yet simplified use of it.
  // Now create an entity_metadata_wrapper around the new node entity
  // to make getting and setting values easier
  $ewrapper = entity_metadata_wrapper('node', $entity);
  // Using the wrapper, we do not have to worry about telling Drupal
  // what language we are using. The Entity API handles that for us.
  if (!empty($node->title)) {
  	// find the max for this field and trunk it
  	$ewrapper->title->set($node->title);
  }
  else {
  	$ewrapper->title->set('Untitled');
  }

  $ewrapper->field_identifier->set($node->identifier);

  if (isset($node->handle)) {
  	$ewrapper->field_handle->set(array('url' => $node->handle));
  }

  // $ewrapper->representative_image->set(array('url' => $node['representative_image']));
  $ewrapper->save();

  return $entity;
}
