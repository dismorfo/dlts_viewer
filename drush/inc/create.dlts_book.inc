<?php

function save_dlts_book ( $node, $language = 'en' ) {

  global $user;

  $node_language = ( isset($node['node_language']) ? $node['node_language'] : $language );

  $book_exist = book_nid ( $node['identifier'], $node_language );

  if ($book_exist) {

    // Load the node by NID
    $entity = node_load($book_exist);

    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }

  else {

    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_book',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
    );

    if ( $node_language == LANGUAGE_NONE)
    {
      $node_language = 'en';
    }

    if ( $node_language != 'en' )
    {

      // find if the English version of this book exist
      $tnid = book_nid( $node['identifier'], 'en' );

      if ( $tnid )
      {

        // make sure the language exists
        $enabled_language = add_language($node_language);

        // if language does not exists, do not add the book
        if (!$enabled_language) return FALSE;

        // load the source
        $source_node = node_load( $tnid );

        // wrap it with Entity API
        $source_node_ewrapper = entity_metadata_wrapper( 'node', $source_node );

        // set nid as tnid
        $source_node->tnid = $tnid;

        // save source
        $source_node_ewrapper->save();

        // not sure why this is needed
        $values['translate'] = 0;

        // set source nid as the tnid for the translation
        $values['tnid'] = $tnid;

        // English is the default language for the books object
        $values['source'] = 'en';

      }

      // return as soon as possible
      else {

        drush_log( t( 'English version does not exist', 'error' ) );

        return FALSE;

      }

    }

    $values['language'] = $node_language;

    $entity = entity_create('node', $values);

    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }

  $collections_nids =
    $subjects =
    $pdf_files = array();

  // find the max for this field and trunk it
  $ewrapper->title->set( $node['title'] );

  if ( !empty( $node['identifier'] ) ) {
    $ewrapper->field_identifier->set( $node['identifier'] );
  }

  foreach ( $node['partners'] as $partner ) {

  	$partner_entity = NULL;

    $nid = partner_nid ( $partner['identifier'] ) ;

    // create collection
    if ( ! $nid ) {
    	$partner_entity = create_dlts_partner ( $partner ) ;
    }
    if ( $nid ) {
      $partners_nids[] = $nid;
    }
  }

  foreach ( $node['collections'] as $collection ) {

  	$collection_entity = NULL;

  	$nid = collection_nid ( $collection['identifier'] ) ;

  	// create collection
  	if ( ! $nid ) {
  		$collection_entity = create_dlts_collection ( $collection ) ;
  	}
  	if ( $nid ) {
  		$collections_nids[] = $nid;
  	}
  }

  if ( !empty($partners_nids) ) {
    $ewrapper->field_partner->set( $partners_nids );
  }

  if ( !empty($collections_nids) ) {
  	$ewrapper->field_collection->set( $collections_nids );
  }


  if (!empty($node['isbn'])) {
    $ewrapper->field_isbn->set( $node['isbn'] );
  }

  if (!empty($node['handle'])) {
    $ewrapper->field_handle->set( array( 'url' => $node['handle'] ) );
  }

  if (!empty($node['title_long'])) {
    $ewrapper->field_title->set( $node['title_long'] );
  }

  if (!empty($node['subtitle'])) {
    $ewrapper->field_subtitle->set( $node['subtitle'] );
  }

  if (!empty($node['description'])) {
    $ewrapper->field_description->set( $node['description'] );
  }

  if (!empty($node['editor'])) {
    $ewrapper->field_editor->set( $node['editor'] );
  }

  if (!empty($node['creator'])) {
    $ewrapper->field_creator->set( $node['creator'] );
  }

  if (!empty($node['author'])) {
    $ewrapper->field_author->set( $node['author'] );
  }

  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set( $node['publisher'] );
  }

  if (!empty($node['contributor'])) {
    $ewrapper->field_contributor->set( $node['contributor'] );
  }

  if (!empty($node['dimensions'])) {
    $ewrapper->field_dimensions->set( $node['dimensions'] );
  }

  if (!empty($node['volume'])) {
    $ewrapper->field_volume->set( $node['volume'] );
  }

  if (!empty($node['number'])) {
    $ewrapper->field_number->set( $node['number'] );
  }

  if (!empty($node['call_number'])) {
    $ewrapper->field_call_number->set( $node['call_number'] );
  }

  if (!empty($node['other_version'])) {
    $ewrapper->field_other_version->set( $node['other_version'] );
  }

  if (!empty($node['binding_orientation'])) {
    $ewrapper->field_binding_orientation->set( (int)$node['binding_orientation'] );
  }

  if (!empty($node['scan_order'])) {
    $ewrapper->field_scan_order->set( (int)$node['scan_order'] );
  }

  if (!empty($node['page_count'])) {
    $ewrapper->field_page_count->set( (int)$node['page_count'] );
  }

  if (!empty($node['sequence_count'])) {
    $ewrapper->field_sequence_count->set( (int)$node['sequence_count'] );
  }

  if (!empty($node['read_order'])) {
    $ewrapper->field_read_order->set( (int)$node['read_order'] );
  }

  if (!empty($node['publication_location'])) {
    $ewrapper->field_publication_location->set( $node['publication_location'] );
  }

  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set( $node['publisher'] );
  }

  if (!empty($node['publication_date_text'])) {
    $ewrapper->field_publication_date_text->set( $node['publication_date_text'] );
  }

  /**
  if (!empty($node['representative_image'])) {
    // send FALSE overwrite while developing
    $representative_image = import_add_file($node['representative_image'], FALSE);
    if ($representative_image) {
      $ewrapper->field_representative_image ->set(array('fid' => $representative_image->fid ));
    }
  }
  */

  if (!empty($node['language'])) {
    $ewrapper->field_language ->set( array( $node['language'] ) );
  }

  if (!empty($node['language_code'])) {
    $ewrapper->field_language_code ->set( array( $node['language_code'] ) );
  }

  /**
  if (!empty($node['pdf_files'])) {
    foreach ($node['pdf_files'] as $file) {
      // send FALSE overwrite while developing
      $content = import_add_file($file, FALSE);
	  if (isset($content) && isset($content->fid )) {
	    $pdf_files[] = array('fid' => $content->fid, 'display' => 0 );
	  }
    }
    $ewrapper->field_pdf_file->set( $pdf_files );
  }
  */

  /**
  if (!empty($node['subjects'])) {
    $vocabulary = taxonomy_vocabulary_machine_name_load('subjects');
    foreach($node['subjects'] as $subject) {
      $term = taxonomy_get_term_by_name($subject);
      if ($term) {
        $subjects = array_merge($subjects, array_keys($term));
      }
      else {
        $term = create_taxonomy_term($subject, $vocabulary->vid);
        $subjects = array_merge($subjects, array_keys($term));
      }
      $ewrapper->field_subject->set($subjects);
    }
  }
  */

  $ewrapper->save();

  /**
   * Multi volume book
  if ( $node_language == 'en' ) {

    foreach( $node['multivol'] as $multivol ) {

      $multivol_nid = multivol_nid($multivol['identifier']);

      if ($multivol_nid) {

        // $multivol['title'] = $node['title_long'] . ' ' . t('Volume') . ' ' . $multivol['volume'];

        $multivol['title'] = $node['title_long'];

        $multivol['book'] = $node['identifier'];

        $dlts_multivol_book = create_dlts_multivol_book($multivol);

      }

      else {
        drush_log( t('Book "@title" is part of multi volume set "@multivol", but multi volume does not exist. Skip.', array( '@title' => $node['title'], '@multivol' => $multivol['identifier'], ) ), 'warning' );
      }

    }
  }
  */

  return $entity;

}

function create_dlts_book_from_json ( $books ) {

  foreach ( $books as $value ) {

  	$pages_array =
  	$partners =
  	$stitch_page_array =
  	$multivols =
  	$other_versions =
  	$isbns =
  	$authors =
  	$languages =
  	$collections =
  	$contributors =
  	$creators =
  	$editors =
  	$publishers =
  	$pdf_files =
  	$language_codes =
  	$ocr_text_files =
  	$subjects = array();

  	$page_count = $subtitle = $title = NULL;

  	$json_string = file_get_contents ( $value->uri ) ;

  	$data = json_decode ( $json_string ) ;

  	$entity_title = $data->entity_title;

  	drush_log ( dt ( 'Import book @title from JSON document @document' , array ('@documet' => $value->uri , '@title' => $entity_title ) ) , 'ok' ) ;

  	$entity_language = $data->entity_language;

    $entity_identifier = $data->identifier;

    $metadata = $data->metadata;

    /** partners the book belong */
    if ( $metadata->partner ) {
    	foreach ($metadata->partner->value as $value) {
    		$partners[] =  array(
    				'title' => $value->title,
    				'language' => $value->language,
    				'identifier' => $value->identifier,
    				'code' => $value->code,
    				'name' => $value->name,
    		);
    	}
    }

    /** collections the book belong */
    if ( $metadata->collection ) {
    	foreach ($metadata->collection->value as $value) {
    		$collections[] =  array(
    				'title' => $value->title,
    				'language' => $value->language,
    				'identifier' => $value->identifier,
    				'code' => $value->code,
    				'name' => $value->name,
    				'partner' => $value->partner,
    		);
    	}
    }

    /**
    [29] => subject
    [19] => publication_date
    [36] => geographic_coordinates
    [37] => geographic_subject
    */

    if ( $metadata->title ) $title = $metadata->title->value[0];

    if ( $metadata->subtitle && ! empty ( $metadata->subtitle->value ) ) $subtitle = $metadata->subtitle->value[0];

    if ( $metadata->page_count && ! empty ( $metadata->page_count->value ) ) $page_count = $metadata->page_count->value[0];

    if ( $metadata->sequence_count && ! empty ( $metadata->sequence_count->value ) ) $sequence_count = $metadata->sequence_count->value[0];

    if ( $metadata->binding_orientation && ! empty ( $metadata->binding_orientation->value ) ) $binding_orientation = $metadata->binding_orientation->value[0];

    if ( $metadata->call_number && ! empty ( $metadata->call_number->value ) ) $call_number = $metadata->call_number->value[0];

    if ( $metadata->description && ! empty ( $metadata->description->value ) ) $description = $metadata->description->value[0];

    if ( $metadata->dimensions && ! empty ( $metadata->dimensions->value ) ) $dimensions = $metadata->dimensions->value[0];

    if ( $metadata->handle && ! empty ( $metadata->handle->value ) ) $handle = $metadata->handle->value[0];

    if ( $metadata->number && ! empty ( $metadata->number->value ) ) $number = $metadata->number->value[0];

    if ( $metadata->read_order && ! empty ( $metadata->read_order->value ) ) $read_order = $metadata->read_order->value[0];

    if ( $metadata->scan_order && ! empty ( $metadata->scan_order->value ) ) $scan_order = $metadata->scan_order->value[0];

    if ( $metadata->scanning_notes && ! empty ( $metadata->scanning_notes->value ) ) $scanning_notes = $metadata->scanning_notes->value[0];

    if ( $metadata->scan_date && ! empty ( $metadata->scan_date->value ) ) $scan_date = $metadata->scan_date->value[0];

    if ( $metadata->rights && ! empty ( $metadata->rights->value ) ) $rights = $metadata->rights->value[0];

    if ( $metadata->volume && ! empty ( $metadata->volume->value ) ) $volume = $metadata->volume->value[0];

    if ( $metadata->publication_date && ! empty ( $metadata->publication_date->value ) ) $publication_date = $metadata->publication_date->value[0];

    if ( $metadata->publication_location && ! empty ( $metadata->publication_location->value ) ) $publication_location = $metadata->publication_location->value[0];

    if ( $metadata->publication_date_text && ! empty ( $metadata->publication_date_text->value ) ) $publication_date_text = $metadata->publication_date_text->value[0];

    if ( $metadata->scan_date && ! empty ( $metadata->scan_date->value ) ) $scan_date = $metadata->scan_date->value[0];

    if ( $metadata->representative_image  && ! empty ( $metadata->representative_image ) ) $representative_image = $metadata->representative_image->value->fileserver;

    if ( $metadata->other_version ) foreach ( $metadata->other_version->value as $other_version ) $other_versions[] = $other_version ;

    if ( $metadata->isbn ) foreach ( $metadata->isbn->value as $isbn ) $isbns[] = $isbn ;

    if ( $metadata->language ) foreach ( $metadata->language->value as $language ) $languages[] = $language ;

    if ( $metadata->language_code ) foreach ( $metadata->language_code->value as $language_code ) $language_codes[] = $language_code ;

    if ( $metadata->topic ) foreach ( $metadata->topic->value as $topic ) $topics[] = $topic ;

    if ( $metadata->author ) foreach ( $metadata->author->value as $author ) $authors[] = $author ;

    if ( $metadata->creator ) foreach ( $metadata->creator->value as $creator ) $creators[] = $creator ;

    if ( $metadata->contributor ) foreach ( $metadata->contributor->value as $contributor ) $contributors[] = $contributor ;

    if ( $metadata->editor ) foreach ( $metadata->editor->value as $editor ) $editors[] = $editor ;

    if ( $metadata->publisher ) foreach ( $metadata->publisher->value as $publisher ) $publishers[] = $publisher ;

    if ( $metadata->subjects && ! empty ( $subjects ) ) $subjects = $metadata->subjects->value[0] ;

    if ( $metadata->pdf_file ) foreach ( $metadata->pdf_file->value as $pdf_file ) $pdf_files[] = $pdf_file->fileserver ;

    if ( $metadata->ocr_text ) foreach ( $metadata->ocr_text->value as $ocr_text ) $ocr_text_files[] = $ocr_text->fileserver ;

    $node = array (
    	'entity_title' => $entity_title,
    	'entity_language' => $entity_language,
    	'identifier' => $entity_identifier,
   		'partners' => $partners,
   		'collections' => $collections,
   		'title' => $title,
   		'subtitle' => $subtitle,
   		'page_count' => $page_count,
   		'sequence_count' => $sequence_count,
   		'binding_orientation' => $binding_orientation,
   		'call_number' => $call_number,
   		'description' => $description,
   		'dimensions' => $dimensions,
   		'handle' => $handle,
    	'language_code' => $language_codes,
    	'language' => $languages,
    	'number' => $number,
    	'read_order' => $read_order,
    	'scan_order' => $scan_order,
    	'scanning_notes' => $scanning_notes,
    	'volume' => $volume,
    	'publication_date' => $publication_date,
    	'publication_location' => $publication_location,
    	'publication_date_text' => $publication_date_text,
    	'scan_date' => $scan_date,
    	'representative_image' => $representative_image,
    	'ocr_text' => $ocr_text_files,
    	'other_version' => $other_versions,
    	'rights' => $rights,
    	'isbn' => $isbns,
    	'author' => $authors,
    	'creator' => $creators,
   		'contributor' => $contributors,
   		'editor' => $editors,
   		'publisher' => $publishers,
   		'subjects' => $subjects,
   		'pdf_files' => $pdf_files,
   		'pages' => $pages_array,
    	// 'multivol' => $multivols,
    );

    $created = save_dlts_book ( $node ) ;

    if ( $created && isset ( $created->nid ) ) {

    	foreach ( $data->pages->page as $key => $page ) {

      	$collection_name = 'dlts_book_page';

      	$collection = mongodb_collection ( $collection_name ) ;

      	$collection->save (
      			array (
      					'isPartOf' => $entity_identifier,
      					'sequence' => array ( $page->sequence ),
      					'realPageNumber' => $page->page,
      					'cm' => array (
      					  'uri' => $page->fileserver,
      					  'width' => $page->metadata->width ,
      					  'height' => $page->metadata->height ,
      					  'levels' => $page->metadata->levels ,
      					  'dwtLevels' => $page->metadata->dwtLevels ,
      					  'compositingLayerCount' => $page->metadata->compositingLayerCount ,
      					  'timestamp' => $page->metadata->timestamp ,
      				),
      			)
        ) ;

      }
    }
  }
}