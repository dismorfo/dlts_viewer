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
function dlts_photo_set_load($identifier) {
    $language = array('en', LANGUAGE_NONE);
    $query_parameters = drupal_get_query_parameters();
    if (isset($query_parameters['lang'])) {
        $language = array(filter_xss($query_parameters['lang']));
    }
    $query = new EntityFieldQuery;
    $identifier = filter_xss($identifier);
    $result = $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'dlts_photo_set')
        ->propertyCondition('status', 1) // only load books that are published
        ->propertyCondition('language', $language)
        ->fieldCondition('field_identifier', 'value', $identifier, '=')
        ->execute();
    if (!empty($result['node'])) {
        $keys = array_keys($result['node']);
        $nid = array_pop($keys);
        return node_load($nid);
    }
    return FALSE;
}

/**
 * A wildcard loader for dlts_photo pages data from MongoDB, using the book.
 * identifier and $sequence.
 *
 * @param unknown $sequence
 *
 * @return object MongoDB book page object
 */
function dlts_photo_load($sequence) {

    /**
     * Load MongoDB helper
     */
    module_load_include('inc', 'dlts_viewer', 'inc/mongodb');

    // I don't like this. Can we find a better solution for this?
    // investigate a way to pass this from hook_menu
    // @TODO: Make it so that it can be call as API
    $isPartOf = arg(1);


    // viewer defaults to single page view
    //$collection_name = 'dlts_books_page';
    $collection_name = 'dlts_photo';
    // user can requests a view modes, get query parameters and
    // check if page_view is set
    $query_parameters = drupal_get_query_parameters();

    //if (isset($query_parameters['page_view'])) {
        // check if the requested view mode is valid
      //  if (array_key_exists($query_parameters['page_view'], $collections)) {
            //$collection_name = $collections[$query_parameters['page_view']];
       // }
   // }

    $result = dlts_viewer_mongodb_load_sequence($sequence, $isPartOf, $collection_name);

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
                    'sequence' => $result['sequence'],
                    'uri' => file_create_url($result['cm']['uri']),
                    'width' => $djatoka['width'],
                    'height' => $djatoka['height'],
                    'levels' => $djatoka['levels'],
                    'dwtLevels' => $djatoka['dwtLevels'],
                    'compositingLayerCount' => $djatoka['compositingLayerCount'],
                    'timestamp' => $result['cm']['timestamp']
                );
            }
            /**
             * Request failed.
             */
            else {
            }
        }
        else {
            return array(
                'fid' => (string) $result['_id'],
                'sequence' => $result['sequence'],
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

    drupal_set_message(t('<em>DLTS Viewer</em>: Sequence page <strong>@sequence</strong> from set identify by identifier <strong>@identifier</strong> not found.', array('@sequence' => $sequence, '@identifier' => $isPartOf)), 'error');

    watchdog('MongoDB sequence not found', 'Unable to find set sequence page <strong>%sequence</strong> from book <strong>%identifier</strong>.', array('%identifier' => $isPartOf, '%sequence' => $sequence), WATCHDOG_NOTICE);

    // if $result is empty, retrun FALSE so that hook_menu can 404 redirect
    return FALSE;
}


