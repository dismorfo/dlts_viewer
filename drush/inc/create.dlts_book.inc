<?php

/**
 * @file
 * DLTS Viewer drush
 */

/**
 * @TODO: Support for fields: publication_date, geographic_coordinates, geographic_subject
 */
function save_dlts_book($node, $language = 'en') {
  // Get the current user
  // --user=uid OR --user=1 for super user
	global $user;
  $node_language = (isset($node['entity_language']) ? $node['entity_language'] : $language);
  /** force EN language */
  if ($node_language == LANGUAGE_NONE) {
  	$node_language = 'en';
  }
  $book_exist = book_nid($node['identifier'], $node_language);
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
    if ($node_language == LANGUAGE_NONE) {
      $node_language = 'en';
    }
    if ($node_language != 'en') {
      // find if the English version of this book exist
      $tnid = book_nid($node['identifier'], 'en');
      if ($tnid) {
        // make sure the language exists
        $enabled_language = add_language($node_language);
        // if language does not exists, do not add the book
        if (!$enabled_language) return FALSE;
        // load the source
        $source_node = node_load($tnid);
        // wrap it with Entity API
        $source_node_ewrapper = entity_metadata_wrapper('node', $source_node);
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
        drush_log(t('English version does not exist', 'error'));
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
  $collections_nids = $subjects = array();
  if (!empty($node['entity_title'])) {
  	// find the max for this field and trunk it
  	$ewrapper->title->set($node['entity_title']);
  }
  else {
  	$ewrapper->title->set('Untitled');
  }
  if (!empty($node['identifier'])) {
    $ewrapper->field_identifier->set($node['identifier']);
  }
  foreach ($node['partners'] as $partner) {
    $partner_entity = NULL;
    $nid = partner_nid($partner['identifier']);
    // create collection
    if (!$nid) {
      $partner_entity = create_dlts_partner($partner);
    }
    if ($nid) {
      $partners_nids[] = $nid;
    }
  }
  foreach ($node['collections'] as $collection) {
    $collection_entity = NULL;
    $nid = collection_nid($collection['identifier']);
    // create collection
    if (!$nid) {
      $collection_entity = create_dlts_collection($collection);
    }
    if ($nid) {
      $collections_nids[] = $nid;
    }
  }
  if (!empty($partners_nids)) {
    $ewrapper->field_partner->set($partners_nids);
  }
  if (!empty($collections_nids)) {
    $ewrapper->field_collection->set($collections_nids);
  }
  if (!empty($node['isbn'])) {
    $ewrapper->field_isbn->set($node['isbn']);
  }
  if (!empty($node['handle'])) {
    $ewrapper->field_handle->set(array('url' => $node['handle']));
  }
  if (!empty($node['title_long'])) {
    $ewrapper->field_title->set($node['title_long']);
  }
  if (!empty($node['subtitle'])) {
    $ewrapper->field_subtitle->set($node['subtitle']);
  }
  if (!empty($node['description'])) {
    $ewrapper->field_description->set($node['description']);
  }
  if (!empty($node['editor'])) {
    $ewrapper->field_editor->set($node['editor']);
  }
  if (!empty($node['creator'])) {
    $ewrapper->field_creator->set($node['creator']);
  }
  if (!empty($node['author'])) {
    $ewrapper->field_author->set($node['author']);
  }
  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set($node['publisher']);
  }
  if (!empty($node['contributor'])) {
    $ewrapper->field_contributor->set($node['contributor']);
  }
  if (!empty($node['dimensions'])) {
    $ewrapper->field_dimensions->set($node['dimensions']);
  }
  if (!empty($node['volume'])) {
    $ewrapper->field_volume->set($node['volume']);
  }
  if (!empty($node['number'])) {
    $ewrapper->field_number->set($node['number']);
  }
  if (!empty($node['call_number'])) {
    $ewrapper->field_call_number->set($node['call_number']);
  }
  if (!empty($node['other_version'])) {
    $ewrapper->field_other_version->set($node['other_version']);
  }
  if (!empty($node['binding_orientation'])) {
    $ewrapper->field_binding_orientation->set((int)$node['binding_orientation']);
  }
  if (!empty($node['scan_order'])) {
    $ewrapper->field_scan_order->set((int)$node['scan_order']);
  }
  if (!empty($node['page_count'])) {
    $ewrapper->field_page_count->set((int)$node['page_count']);
  }
  if (!empty($node['sequence_count'])) {
    $ewrapper->field_sequence_count->set((int)$node['sequence_count']);
  }
  if (!empty($node['read_order'])) {
    $ewrapper->field_read_order->set((int)$node['read_order']);
  }
  if (!empty($node['publication_location'])) {
    $ewrapper->field_publication_location->set($node['publication_location']);
  }
  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set($node['publisher']);
  }
  if (!empty($node['publication_date_text'])) {
    $ewrapper->field_publication_date_text->set($node['publication_date_text']);
  }
  if (!empty($node['representative_image'])) {
    $label = (isset($node['representative_image']['label'])) ? $node['representative_image']['label'] : null;
    $serialize_data = serialize(
      array(
    	  'label' => $label,
    	  'uri' => $node['representative_image']['uri'],
    	  'url' => file_create_url($node['representative_image']['uri']),
      	'image_style' => '0x230'
      )
    );
    $representative_image = array(
      'uri' => $node['representative_image']['uri'],
    	'label' => $label,
    	'data' => $serialize_data,
    );
    $ewrapper->field_representative_image->set($representative_image);
  }
  if (!empty($node['pdf_files'])) {
    foreach ($node['pdf_files'] as $pdf) {
    	$label = (isset($pdf['label'])) ? $pdf['label'] : '';
    	$serialize_data = serialize(array('label' => $label, 'uri' => $pdf['file'], 'url' => file_create_url($pdf['file'])));
      $field_pdf_file[] = array(
        'uri' => $pdf['file'],
      	'label' => $label,
      	'data' => $serialize_data,
      );
    }
    $ewrapper->field_pdf_file->set($field_pdf_file);
  }
  if (!empty($node['language'])) {
    $ewrapper->field_language->set(array($node['language']));
  }
  if (!empty($node['language_code'])) {
    $ewrapper->field_language_code->set(array($node['language_code']));
  }
  if (!empty($node['rights'])) {
    $ewrapper->field_rights->set($node['rights']);
  }
  if (!empty($node['subject'])) {
  	$vid = dlts_viewer_subject_vocabulary_vid();
  	$node_subjects = array();
    foreach ($node['subject'] as $subject) {
    	if (!empty($subject)) {
    		$subject_list = dlts_viewer_subject_tid($subject,$vid);
    		$node_subjects[] = $subject_list[$subject];
      }
    }
    if (!empty($node_subjects)) {
    	$ewrapper->field_subject->set($node_subjects);
    }
  }
  $ewrapper->save();
  /** If book language is EN check for  Multi volume and Series*/
  if ($node_language == 'en') {
  	/** Multi volume book */
  	if (!empty($node['multivolume']) && !empty($node['multivolume']->volume)) {
      foreach ($node['multivolume']->volume as $multivolume) {
        $multivolume->title = (isset($node['title_long']) && !empty($node['title_long'])) ? $node['title_long'] : $node['title'];
        $multivolume->book = $node['identifier'];
        create_dlts_multivol_book($multivolume);
      }
  	}
    /** Series book */
  	if (!empty($node['series'])) {
      foreach ($node['series'] as $series) {
      	$series->book = $node['identifier'];
    	  create_dlts_series_book($series);
      }
  	}
  }
  return $entity;
}

function dlts_viewer_subject_tid($subject,$vid) {
	$global_subjects = &drupal_static('global_subjects');
	if (empty($global_subjects)) {
		$global_subjects = array();
	}
	if (!empty($subject)) {
	  $this_subject = trim(preg_replace('/\s\s+/', ' ', str_replace("\n", " ", $subject)));
	  if (!array_key_exists($this_subject, $global_subjects)) {
	  	$term = taxonomy_get_term_by_name($this_subject);
		  if ($term) {
		  	$this_term = array_pop($term);
		    $tid = $this_term->tid;
		  }
		  else {
		    $tid = create_taxonomy_term($this_subject, $vid);
		  }
      $global_subjects[$this_subject] = $tid;
	  }
	}
	return $global_subjects;
}

function dlts_viewer_subject_vocabulary_vid() {
	$global_vocabulary_vid = &drupal_static('global_vocabulary_vid');
	if (empty($global_vocabulary_vid)) {
		$global_vocabulary_vid = taxonomy_vocabulary_machine_name_load('subjects')->vid;
	}
	return $global_vocabulary_vid;
}

function create_dlts_book_from_json($books, $cretePages = false) {
  foreach ($books as $value) {
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
      $subjects =
      $series = array();
    $page_count = $subtitle = $title = NULL;
    $json_string = file_get_contents($value->uri);
    $json_string = str_replace(array('\r', '\n'), '', $json_string);
    $data = json_decode($json_string);
    $entity_title = $data->entity_title;
    drush_log('Importing ' . $entity_title . ' from JSON document ' . $value->uri, 'ok');
    $entity_language = $data->entity_language;
    $entity_identifier = $data->identifier;
    if (!isset($data->metadata)) {
    	$metadata = (object) array();
    }
    else {
    	$metadata = $data->metadata;
    }
    if (!isset($data->multivolume)) {
    	$multivolume  = (object) array();
    }
    else {
      $multivolume = $data->multivolume;
    }
    if (!isset($data->series)) {
    	$series = array();
    }
    else {
    	$series = $data->series;
    }
    /** partners the book belong */
    if ($metadata->partner) {
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
    if ($metadata->collection) {
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
    if ($metadata->title) {
    	$title = $metadata->title->value[0];
    }
    if ($metadata->subtitle && !empty($metadata->subtitle->value)) {
    	$subtitle = $metadata->subtitle->value[0];
    }
    if ($metadata->page_count && !empty($metadata->page_count->value)) {
    	$page_count = $metadata->page_count->value[0];
    }
    if ($metadata->sequence_count && !empty($metadata->sequence_count->value)) {
    	$sequence_count = $metadata->sequence_count->value[0];
    }
    if ($metadata->binding_orientation && !empty($metadata->binding_orientation->value)) {
    	switch ($metadata->scan_order->value[0]) {
    		case 'Vertical' :
    		case '0' :
    		case 0 :
    			$binding_orientation = 0;
    			break;
    		case 'Horizontal' :
    		case '1' :
    		case 1 :
    			$binding_orientation = 1;
    			break;
    	}
    }
    if ($metadata->call_number && !empty($metadata->call_number->value)) {
    	$call_number = $metadata->call_number->value[0];
    }
    if ($metadata->description && !empty($metadata->description->value)) {
    	$description = $metadata->description->value[0];
    }
    if ($metadata->dimensions && !empty($metadata->dimensions->value)) {
    	$dimensions = $metadata->dimensions->value[0];
    }
    if ($metadata->handle && !empty($metadata->handle->value)) {
    	$handle = $metadata->handle->value[0];
    }
    if ($metadata->number && !empty($metadata->number->value)) {
    	$number = $metadata->number->value[0];
    }
    if ($metadata->read_order && !empty($metadata->read_order->value)) {
      switch ($metadata->read_order->value[0]) {
      	case 'Left to right' :
      	case '0' :
      	case 0 :
      		$read_order = 0;
      		break;
      	case 'Right to left' :
      	case '1' :
      	case 1 :
    	  	$read_order = 1;
    		  break;
      }
    }
    if ($metadata->scan_order && !empty($metadata->scan_order->value)) {
    	switch ($metadata->scan_order->value[0]) {
    		case 'Left to right' :
    		case '0' :
    		case 0 :
    			$scan_order = 0;
    			break;
    		case 'Right to left' :
    		case '1' :
    		case 1 :
    			$scan_order = 1;
    			break;
    	}
    }
    if ($metadata->scanning_notes && !empty($metadata->scanning_notes->value)) {
    	$scanning_notes = $metadata->scanning_notes->value[0];
    }
    if ($metadata->scan_date && !empty($metadata->scan_date->value)) {
    	$scan_date = $metadata->scan_date->value[0];
    }
    if ($metadata->rights && !empty($metadata->rights->value)) {
    	$rights = trim($metadata->rights->value[0]);
    }
    if ($metadata->volume && !empty($metadata->volume->value)) {
    	$volume = $metadata->volume->value[0];
    }
    if ($metadata->publication_date && !empty($metadata->publication_date->value)) {
    	$publication_date = $metadata->publication_date->value[0];
    }
    if ($metadata->publication_location && !empty($metadata->publication_location->value)) {
    	$publication_location = $metadata->publication_location->value[0];
    }
    if ($metadata->publication_date_text && !empty($metadata->publication_date_text->value)) {
    	$publication_date_text = $metadata->publication_date_text->value[0];
    }
    if ($metadata->scan_date && !empty($metadata->scan_date->value)) {
    	$scan_date = $metadata->scan_date->value[0];
    }
    if ($metadata->representative_image && !empty($metadata->representative_image)) {
    	$representative_image = array(
    	  'uri' => $metadata->representative_image->cm->uri,
    	  'timestamp' => $metadata->representative_image->cm->timestamp,
      );
    }
    if ($metadata->other_version) {
    	foreach ($metadata->other_version->value as $other_version) {
    	  $other_versions[] = $other_version;
    	}
    }
    if ($metadata->isbn) {
    	foreach ($metadata->isbn->value as $isbn) {
    	  $isbns[] = $isbn;
    	}
    }
    if ($metadata->language) {
    	foreach ($metadata->language->value as $language) {
    	  $languages[] = $language;
    	}
    }
    if ($metadata->language_code) {
    	foreach ($metadata->language_code->value as $language_code) {
    	  $language_codes[] = $language_code;
    	}
    }
    if ($metadata->topic) {
    	foreach ($metadata->topic->value as $topic) {
    	  $topics[] = $topic;
      }
    }
    if ($metadata->author) {
    	foreach ($metadata->author->value as $author) {
    	  $authors[] = $author;
      }
    }
    if ($metadata->creator) {
    	foreach ($metadata->creator->value as $creator) {
    	  $creators[] = $creator;
    	}
    }
    if ($metadata->contributor) {
      foreach ($metadata->contributor->value as $contributor) {
    	  $contributors[] = $contributor;
    	}
    }
    if ($metadata->editor) {
    	foreach ($metadata->editor->value as $editor) {
    	  $editors[] = $editor;
    	}
    }
    if ($metadata->publisher) {
    	foreach ($metadata->publisher->value as $publisher) {
    	  $publishers[] = $publisher;
    	}
    }
    if ($metadata->subject && !empty($metadata->subject->value)) {
    	foreach ($metadata->subject->value as $subject) {
    	  $subjects[] = $subject;
    	}
    }
    if ($metadata->pdf_file) {
    	$valid_extensions = array('pdf');
      foreach ($metadata->pdf_file->value as $pdf_file) {
      	$pathinfo = pathinfo($pdf_file);
      	$basename = str_replace('fileserver://', '', $pathinfo['basename']);
      	$ext = pathinfo($pdf_file, PATHINFO_EXTENSION);
      	// check if we have a fileserver scheme
      	if (strpos($pdf_file, 'fileserver://') === false) continue;
      	// check that the URI is not bigger than the allowed MySQL size for varchar with index
      	if (strlen($pdf_file) > 255) continue;
      	// check that the URI contain a file extension
      	if (!$ext) continue;
      	// do we have a valid extension
      	if (!in_array($ext,$valid_extensions)) continue;
      	if (strpos($pdf_file, '_hi')) {
      		$pdf_files[] = array('file' => $pdf_file, 'label' => 'High resolution');
      	}
      	elseif (strpos($pdf_file, '_lo')) {
      		$pdf_files[] = array('file' => $pdf_file, 'label' => 'Low resolution');
      	}
      	else {
      		$pdf_files[] = array('file' => $pdf_file);
      	}
    	}
    }
    if ($metadata->ocr_text) {
    	foreach ($metadata->ocr_text->value as $ocr_text) {
    	  $ocr_text_files[] = $ocr_text->fileserver;
    	}
    }
    $node = array(
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
    	'publication_date_text' => $publication_date_text,
    	'publication_location' => $publication_location,
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
      'subject' => $subjects,
      'pdf_files' => $pdf_files,
      'pages' => $pages_array,
      'multivol' => $multivols,
    	'multivolume' => $multivolume,
    	'series' => $series,
    );
    $created = save_dlts_book($node);
    if ($cretePages && $created && isset($created->nid)) {
   	  // we only create pages for books with languages English or Undetermined
   	  if ($entity_language == 'en' || $entity_language == LANGUAGE_NONE) {
   	    // Load MongoDB util
   	    module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
   	    // check if the JSON document include pages
        if (isset($data->pages->page) && !empty($data->pages->page)) {
          foreach ($data->pages->page as $key => $page) {
        	  $sequence = $page->sequence;
        	  if (!is_array($page->sequence)) {
        		  $sequence = array($page->sequence);
        	  }
        	  $page->sequence = $sequence;
        	  dlts_viewer_mongodb_new_sequence($page, 'dlts_book_page');
          }
        }
        // check if the JSON document include stitched pages
        if (isset($data->stitched->page) && !empty($data->stitched->page)) {
      	  foreach ($data->stitched->page as $key => $page) {
            dlts_viewer_mongodb_new_sequence($page, 'dlts_book_stitched_page');
          }
        }
      }
    }
  }
}
