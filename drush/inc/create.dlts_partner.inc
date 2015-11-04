<?php

function create_dlts_partner( $node ) {

  global $user;

  $node = (array) $node;

  $exists = partner_nid($node['identifier']);

  if ($exists) {

    // Load the node by NID
    $entity = node_load($exists);

    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }

  else {

    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_partner',
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

  $entity->exists = $exists;

  $uniqid = (string) uniqid();

  if ( isset( $node['title'] ) ) {
    $ewrapper->title->set( $node['title'] );
  }
  else {
    $ewrapper->title->set( 'partner ' . $uniqid );
  }

  if ( isset( $node['identifier'] ) ) {
    $ewrapper->field_identifier->set( $node['identifier'] );
  }
  else {
    $ewrapper->field_identifier->set( $uniqid );
  }

  if ( isset( $node['code'] ) ) {
    $ewrapper->field_code->set( $node['code'] );
  }

  if ( isset( $node['name'] ) ) {
    $ewrapper->field_name->set( $node['name'] );
  }

  $ewrapper->save();

  return $entity;

}
