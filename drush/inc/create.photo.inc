<?php

/**
 * @file
 * DLTS Viewer drush
 */
function create_dlts_photo_from_json($documents) {
  foreach ($documents as $document) {
  	if (!file_exists($document->uri)) continue;
    $json_string = file_get_contents($document->uri);
    $json_string = str_replace(array('\r', '\n'), '', $json_string);
    $data = json_decode($json_string);
    drush_log(dt('Importing photo from JSON document @document',array('@document'=>basename ($document->uri))), 'ok');
		/** Load MongoDB util */
   	module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
		dlts_viewer_mongodb_new_sequence($data, 'dlts_photo');
  }
}
