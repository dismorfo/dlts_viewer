<?php

/**
 * @file
 * Wildcard loaders
 */

/**
 * A wildcard loader for dlts_photo\, using the book identifier.
 *
 * @parameter $identifier
 *  The value of field_identifier for a dtls_book node.
 *
 * @return
 * A node object with the given value for field_identifier.
 *
 * @todo
 * field_identifier is supposed to be unique across all books, but we do
 * no checking of this assumption.
 */
function dlts_photo_load($identifier) {

  $query_parameters = drupal_get_query_parameters();

  $identifier = filter_xss($identifier);

  // viewer defaults to single page view
  $collection_name = 'dlts_photo';

  $result = dlts_viewer_mongodb_load_by_identifier($identifier, $collection_name);

  if ($result && isset($result['cm']) && !empty($result['cm']['uri'])) {
   /**
    * Sequence exist in MongoDB but we need to request metadata from image server
    */
   if (empty($result['cm']['levels'])) {
     /**
      * Load connector
      */
     module_load_include('inc', 'dlts_viewer', 'inc/djatoka');
      /**
       * Pass over $result['cm'] as $file argument
       */
     $djatoka = dlts_viewer_djatoka_request($result['cm']);
     /**
      * No error, update MongoDB record
      * @TODO: Consider using Drupal queue to update the record.
      * See: https://api.drupal.org/api/drupal/modules!system!system.queue.inc/class/DrupalQueue/7
      */
     if (!isset($djatoka['error'])) {
       /**
        * Update $result reference with Image Server requested metadata
        */
       $result['cm'] = array_merge($result['cm'], $djatoka);
       /**
        * Update sequence record
        */
       dlts_viewer_mongodb_update_sequence($result, $collection_name);
       /**
        * Return sequence object
        */
       return array(
         'fid' => (string) $result['_id'],
         'identifier' => $result['identifier'],
         'uri' => file_create_url($result['cm']['uri']),
         'width' => $djatoka['width'],
         'height' => $djatoka['height'],
         'levels' => $djatoka['levels'],
         'dwtLevels' => $djatoka['dwtLevels'],
         'compositingLayerCount' => $djatoka['compositingLayerCount'],
         'timestamp' => $result['cm']['timestamp']
       );
     }
   }
   else {
      return array(
        'fid' => (string) $result['_id'],
        'identifier' => $result['identifier'],
        'uri' => file_create_url($result['cm']['uri']),
        'width' => $result['cm']['width'],
        'height' => $result['cm']['height'],
        'levels' => $result['cm']['levels'],
        'dwtLevels' => $result['cm']['dwtLevels'],
        'compositingLayerCount' => $result['cm']['compositingLayerCount'],
        'timestamp' => $result['cm']['timestamp'],
      );
   }
  }

  drupal_set_message(t('<em>DLTS Viewer</em>: MongoDB photo not found. Unable to find photo with identifier <strong>@identifier</strong>.', array('@identifier' => $identifier)), 'error');

  watchdog('MongoDB photo not found', 'Unable to find photo with identifier <strong>%identifier</strong>.', array('%identifier' => $identifier), WATCHDOG_NOTICE);

  // if $result is empty, retrun FALSE so that hook_menu can 404 redirect
  return FALSE;

 }
