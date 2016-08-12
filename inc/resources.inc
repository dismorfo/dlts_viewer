<?php

function dlts_viewer_resource_dispatcher($entity, $arg = array()) {
  $object = array();
  $object_type = $entity['object_type'];
  // types of returns
  switch ($object_type) {
    case 'dlts_photo' :
      include_once 'resources/photo.inc';
      $object = dlts_viewer_photo_dispatcher($entity);
      break;
  }
  return $object;
}
