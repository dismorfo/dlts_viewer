<?php

/**
 * @TODO: Batch process has consumed in excess of 50% of available memory.
 * Starting new thread
 */

function dlts_solr_series_book_by_book_nid($entity) {
  $query = new EntityFieldQuery;
  $keys = array();
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_series_book')
    ->fieldCondition('field_book', 'nid', array($entity->nid))
    ->execute();
  if (!empty($result['node'])) {
  	$keys = array_keys($result['node']);
  }
  return $keys;
}

function dlts_solr_multivolume_book_by_book_nid($entity) {
  /** deal only with books */
	if ($entity->type == 'dlts_book') {
		$query = new EntityFieldQuery;
		$keys = array();
		$volumes = array();
		$result = $query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'dlts_multivol_book')
		  ->fieldCondition('field_book', 'nid', array($entity->nid))
		  ->execute();
		if (!empty($result['node'])) {
			$keys = array_keys($result['node']);
			$nodes = node_load_multiple($keys);
			if ($nodes) {
				foreach ($nodes as $node) {
					$wrapper = entity_metadata_wrapper('node', $node);
					$multivol = $wrapper->field_multivol->value();
					$volume_number = $wrapper->field_volume_number->value();
					$volume_number_str = $wrapper->field_volume_number_str->value();
					$volumes[] = array( 'multivol' => $multivol->nid, 'volume_number' => $volume_number, 'volume_number_str' => $volume_number_str,);
				}
			}
		}
	}
	return $volumes;
}

function dlts_solr_index_book_collection($collections, &$document, &$entity, &$searchableFields) {

	$language = $entity->language;

	$prefix = '';

	if ($language != 'en') {
		$prefix = $language . '_';
	}

	foreach ($collections as $collection) {

		$collection_wrapper = entity_metadata_wrapper('node', $collection);
		// sigle-value
		$collections_code = $collection_wrapper->field_code->value();
		if (!empty($collections_code)) {
			$document->addField('sm_' . $prefix . 'collection_code', $collections_code);
		}
		// sigle-value
		$collections_identifier = $collection_wrapper->field_identifier->value();
		if (!empty($collections_identifier)) {
			$document->addField('sm_' . $prefix . 'collection_identifier', $collections_identifier);
		}
		// sigle-value
		$collections_label = $collection_wrapper->field_name->value();
		if (!empty($collections_label)) {
			$document->addField('sm_' . $prefix . 'collection_label', $collections_label);
		}
		// multi-value
		$collections_partner = $collection_wrapper->field_partner->value();
		if ($collections_partner) {
			foreach ($collections_partner as $partner) {
				$collection_partner_wrapper = entity_metadata_wrapper('node', $partner);
				// sigle-value
				$collections_partner_code = $collection_partner_wrapper->field_code->value();
				if (!empty($collections_partner_code)) {
					$document->addField('sm_' . $prefix . 'collection_partner_code', $collections_partner_code);
				}
				// sigle-value
				$collections_partner_identifier = $collection_partner_wrapper->field_identifier->value();
				if (!empty($collections_identifier)) {
					$document->addField('sm_' . $prefix . 'collection_partner_identifier', $collections_partner_identifier);
				}
				// sigle-value
				$collections_partner_label = $collection_partner_wrapper->field_name->value();
				if (!empty($collections_partner_label)) {
					$searchableFields[] = $collections_partner_label;
					$document->addField('sm_' . $prefix . 'collection_partner_label', $collections_partner_label);
				}
			}
		}
	}
}

/** Provider (field_partner) */
function dlts_solr_index_book_partners($items = array(), &$document, &$entity, &$searchableFields) {
  
	$language = $entity->language;
	$label = '';
	$code = '';
	$prefix = '';
	if ($language != 'en') {
		$prefix = $language . '_';
	}
	foreach ($items as $item) {
		$wrapper = entity_metadata_wrapper('node', $item);
		
		$document->addField('sm_' . $prefix . 'provider_nid', $item->nid);

		// sigle-value
		$code = $wrapper->field_code->value();
		if (!empty($code)) {
			$document->addField('sm_' . $prefix . 'provider_code', $code);
		}
		// sigle-value
		$identifier = $wrapper->field_identifier->value();
		if (!empty($identifier)) {
			$document->addField('sm_' . $prefix . 'provider_identifier', $identifier);
		}
		// sigle-value
		$label = $wrapper->field_name->value();
		if (!empty($label)) {
			$searchableFields[] = $label;
			$document->addField('sm_' . $prefix . 'provider_label', $label);
		}
		if ($language == 'en') {
			$document->addField('sm_sprovider_label', strtolower($label));
			$document->addField('tum_provider_label', $label);
		  $document->addField('zm_partner',json_encode(array('name'=>$label,'code' => $code, 'nid' => $item->nid)));
		  $document->addField('zm_provider',json_encode(array('name'=>$label,'code' => $code, 'nid' => $item->nid)));
		}
	}
}

function dlts_solr_index_book_series($document, $entity, $searchableFields) {

  $language = $entity->language;

  $prefix = '';

  if ($language != 'en') {
		$prefix = $language . '_';
	}

	/**
	 * Pass entity as reference and check if this books is part of a series.
	 * If book belong to a series, populat isSeriesBook
	 */
	dlts_viewer_is_series_book($entity);
	
	if ($entity->isSeriesBook) {
		/**
	   * Book is part of a series
	   */
		$document->addField('is_ispartofseries', 1);

		foreach ($entity->seriesBook as $series) {
		  $searchableFields[] = $series['series'];
		  $searchableFields[] = $series['volume_label'];
		  $searchableFields[] = $series['collection_label'];
		  $document->addField('sm_' . $prefix . 'series_identifier', $series['series_identifier']);
		  $document->addField('sm_' . $prefix . 'series', $series['series']);
		  $document->addField('sm_' . $prefix . 'series_volume_label', $series['volume_label']);
		  $document->addField('sm_' . $prefix . 'series_collection_label', $series['collection_label']);
		  $document->addField('sm_' . $prefix . 'series_collection_code', $series['collection_code']);
		  $series['label'] = $series['series'];
		  $document->addField('zm_' . $prefix . 'series_data', json_encode($series));
	  }
  }
  
	$m = array();
	
	$entity_type = 'node';
	
	$bundle = 'dlts_series_book';	

	// https://jira.nyu.edu/jira/browse/AWDL-211
  // Let's find out if this one have a Series books
  
  $YYY = dlts_viewer_series_book($entity->nid);
  
  if (!$YYY) return;

  foreach ($YYY as $__) {
    $__wrapper_dlts_series_book = entity_metadata_wrapper($entity_type, $__);
    $___wrapper = entity_metadata_wrapper($entity_type, $__wrapper_dlts_series_book->field_series->value());
    $_series = $__wrapper_dlts_series_book->field_series->value();
    $series_node = node_load($_series->nid);
    $series_wrapper = entity_metadata_wrapper($entity_type, $series_node);
    $series_book_collections = array();
    $series_book_collection = $__wrapper_dlts_series_book->field_collection->value();
    foreach ($series_book_collection as $value) {
      $collection_wrapper = entity_metadata_wrapper($entity_type, $value);
      $series_book_collections[] = array(
        'label' => $value->title,
        'code' => $collection_wrapper->field_code->value(),
        'name' => $collection_wrapper->field_name->value(),        
        'identifier' => $collection_wrapper->field_identifier->value(),             
      );
    }
    $m[] = array(
      'series_book_nid' => $__->nid,
      'series_book_label' => $__->title,
      'series_book_identifier' => $__wrapper_dlts_series_book->field_identifier->value(),
      'series_book_volume_number' =>  $__wrapper_dlts_series_book->field_volume_number->value(),
      'series_book_volume_number_str' => $__wrapper_dlts_series_book->field_volume_number_str->value(),
      'series_book_collections' => $series_book_collections,
      'series_nid' => $_series->nid,
      'series_identifier' => $series_wrapper->field_identifier->value(),      
      'series_label' => $series_node->title,
    );
  }
  
  $document->addField('zm_series_data_x', json_encode($m));

  foreach($m as $b) {
    foreach(array_keys($b) as $c) {
      if (is_array($b[$c])) {
        foreach ($b[$c] as $d) {
          foreach (array_keys($d) as $e) {
            if(!empty($d[$e])) {
              $document->addField('sm_' . $prefix . $c . '_' . $e, $d[$e]);
            }
          }
        }
      }
      else {
        if(!empty($b[$c])) {
          $document->addField('sm_' . $prefix . $c, $b[$c]);
        }
      }
    }
  }
  
}

function dlts_solr_index_book(&$document, &$entity, $entity_type, $env_id) {
	module_load_include('inc', 'dlts_viewer', 'inc/datasource');
	$language = $entity->language;
	// skip if entity language is not English
	if ($language != 'en') return;
	/** add translations helper */
	module_load_include('inc', 'dlts_solr', 'inc/translations');
	/** add image server helper */
	module_load_include('inc', 'dlts_viewer', 'inc/djatoka');
	// wrapp multi-volume entity with Entity API wrapper
	$wrapper = entity_metadata_wrapper('node', $entity);
	/** single-value field */
	$identifier = $wrapper->field_identifier->value();
	$book_field_title = $entity->title;
  /** DLTS book should have a identifier */
  if (!$identifier) return;
  $document->addField('ss_book_identifier', $identifier);
  $document->addField('ss_uri', url('books/' . $identifier . '/1', array('absolute' => TRUE)));
  /** searchable content; dump all you want to be searchable in this array */
  $searchableFields = array($document->content);
  /** content type fields */
  /** Collection (field_collection) */
  /** multi-value field */
  dlts_solr_index_book_collection($wrapper->field_collection->value(), $document, $entity, $searchableFields);
  /** Provider (field_partner) */

  /** multi-value field */
  dlts_solr_index_book_partners($wrapper->field_partner->value(), $document, $entity, $searchableFields);

  dlts_solr_index_book_series($document, $entity, $searchableFields);

  /** ISBN (field_isbn) */
  /** multi-value field */
  $isbns = $wrapper->field_isbn->value();
  if (!empty($isbns)) {
  	foreach ($isbns as $isbn) {
  		$document->addField('sm_isbn', $isbn);
  	}
  }
  unset($isbns);
  unset($isbn);
  /** Permanent Link (field_handle) */
  /** single-value field */
  $handle = $wrapper->field_handle->value();
  if (!empty($handle)) {
  	$document->addField('ss_handle', $handle['url']);
  }
  unset($handle);
  /** Title (field_title) */
  /** single-value field */
  $title_long = $wrapper->field_title->value();
  if (!empty($title_long)) {
  	$book_field_title = trim($title_long);
  }
  unset($title_long);
  /** figure out if this is a Multi Volume book */
  $multivolumes = dlts_solr_multivolume_book_by_book_nid($entity);
  // @TODO: Size might be > that 0. For now just deal with 0 and then fix after
  // we have the other bulk of the work done.
  if (!empty($multivolumes)) {
  	$multivolume = $multivolumes[0];
  	// add reference to entity so that translaletion don't do the same job again.
  	$entity->solr_multivolume = $multivolume;
  	$volume = '';
  	// volume_number_str can be empty, if so, try volume_number
  	if (isset($multivolume['volume_number_str']) && !empty($multivolume['volume_number_str'])) {
  		$volume = $multivolume['volume_number_str'];
  	}
  	elseif (isset($multivolume['volume_number']) && !empty($multivolume['volume_number'])) {
  		$volume = 'Vol. ' . $multivolume['volume_number'];
  	}
  	$book_field_title = t('@title @volume', array( '@title' => $book_field_title, '@volume' => $volume));
  	$document->addField('is_ispartof_multivol', 1);
  	if (!empty($volume)) {
  		$document->addField('ss_multivol_number', $volume);
  	}
  	$document->addField('ss_multivol_label', $book_field_title);
  }
  else {
  	$document->addField('is_ispartof_multivol', 0) ;
  }
  // we need to wait until we figure out if this is a multi volume.
  // by now, we already test for multi volume and title should be the right one.
  $document->addField('ss_title_long', $book_field_title);
  $searchableFields[] = dlts_solr_remove_accents($book_field_title);
  /** Subtitle (field_subtitle) */
  /** single-value field */
  $subtitle = $wrapper->field_subtitle->value();
  if (!empty($subtitle)) {
  	$searchableFields[] = $subtitle;
  	$document->addField('ss_subtitle', $subtitle);
  }
  unset($subtitle);
  /** Description (field_description) */
  /** single-value field */
  $description = $wrapper->field_description->value();
  if (!empty($description)) {
  	$searchableFields[] = $description;
    $document->addField('ss_description', $description);
  }
  unset($description);
  /** Call number (field_call_number) */
  /** single-value field */
  $call_number = $wrapper->field_call_number->value();
  if (!empty($call_number)) {
  	$document->addField('ss_call_number', $call_number);
  	$searchableFields[] = $call_number;
  }
  unset($call_number);
  /** Volume (field_volume) */
  /** single-value field */
  $volume = $wrapper->field_volume->value();
  if (!empty($volume)) {
  	$document->addField('ss_volume', $volume);
  }
  unset($volume);
  /** Number (field_number) */
  /** single-value field */
  $number = $wrapper->field_number->value();
  if (!empty($number)) {
  	$document->addField('ss_number', $number);
  }
  unset($number);
  /** Representative image (field_representative_image) */
  /** single-value field */
  $representative_image = $wrapper->field_representative_image->value();
  if (!empty($representative_image)) {
  	$pathinfo = pathinfo($representative_image['uri']);
  	/** load Image Server util */
    $ri = '';
  	if ($pathinfo['extension'] == 'jpg') {
  		$ri = file_create_url($representative_image['uri']);
  	}
  	elseif ($pathinfo['extension'] == 'jp2') {
  		$ri = dlts_viewer_djatoka_url($representative_image);
  	}
      $dirname = $pathinfo['dirname'];
      $dir_path = pathinfo($dirname);
      $basename = $dir_path['basename'];
      $site_image = $basename.'/'.$pathinfo['filename'];

  	$document->addField('ss_representative_image', $ri);
  	$document->addField('ss_site_image', $site_image);
  }
  unset($ri);
  unset($dirname);
  unset($dir_path);
  unset($base_name);
  unset($site_image);
  unset($representative_image);
  /** Author (field_author) */
  /** multi-value field */
  $authors = $wrapper->field_author->value();
  if (!empty($authors)) {
	  foreach ($authors as $index => $author) {
  		$searchableFields[] = $author;
		  if ($index == 0) {
			  $document->addField('ss_sauthor', strtolower(dlts_solr_remove_accents($author)));
			  $document->addField('iass_author', $author);
		  }
  		$document->addField('sm_author', $author);
  	}
  }
  unset($authors);
  unset($author);
  /** Creator (field_creator) */
  /** multi-value field */
  $creators = $wrapper->field_creator->value();
  if (!empty($creators)) {
  	foreach ($creators as $creator) {
  		$document->addField('sm_creator', $creator);
  		$searchableFields[] = $creator;
  	}
  }
  unset($creators);
  unset($creator);
  /** Editor (field_editor) */
  /** multi-value field */
  $editors = $wrapper->field_editor->value();
  if (!empty($editors)) {
  	foreach ($editors as $editor) {
  		$document->addField('sm_editor', $editor);
  		$searchableFields[] = $editor;
  	}
  }
  unset($editors);
  unset($editor);
  /** Contributor (field_contributor) */
  /** multi-value field */
  $contributors = $wrapper->field_contributor->value();
  if (!empty($contributors)) {
  	foreach ($contributors as $contributor) {
  		$document->addField('sm_contributor', $contributor);
  		$searchableFields[] =  $contributor;
  	}
  }
  unset($editors);
  unset($editor);
  /** Publisher (field_publisher) */
  /** multi-value field */
  $publishers = $wrapper->field_publisher->value();
  if (!empty($publishers)) {
  	foreach ($publishers as $publisher) {
  		$document->addField('sm_publisher',  $publisher);
  		$searchableFields[] =  $publisher;
  	}
  }
  unset($publishers);
  unset($publisher);
  /** Publication date (field_publication_date) */
  /** single-value field */
  $publication_date = $wrapper->field_publication_date->value();
  $publication_date_text = $wrapper->field_publication_date_text->value();
  if (!empty($publication_date) && !empty($publication_date['value'])) {
  	/** convert ISO format into UNIX  timestamp */
  	$timestamp = strtotime($publication_date['value']);
  	$searchableFields[] = $publication_date['value'];
  	/** use date() to get just the year */
  	$publication_year = date('Y', $timestamp);
  	if ($publication_year) {
  		$document->addField('sort_pubyear', $publication_year);
  		$document->addField('iass_pubyear', $publication_year);
  		$document->addField('iass_timestamp', $timestamp);
        $document->addField('ds_timestamp', date('Y-m-d\TG:i:s\Z', strtotime($publication_date['value'])));
  	} else {
        if (!empty($publication_date_text)) {
            $publication_date_text_norm=preg_replace('(u|?)','0',$publication_date_text);
            preg_match_all('\b\d{4}\b', $publication_date_text_norm, $years);
            if(!empty($years)) {
                foreach ($years as $index => $publication_year) {
                    $searchableFields[] = $publication_year;
                    if ($index == 0) {
                        $document->addField('sort_pubyear', $publication_year);
                        $document->addField('iass_pubyear', $publication_year);
                    }
                    $document->addField('dm_pubyear', date('Y-m-d\TG:i:s', strtotime($publication_year)));
                }
            }
        }

      }
  }
  unset($publication_date);
  unset($publication_year);
  /** Publication date (field_publication_date) */
  /** single-value field */

  if (!empty($publication_date_text)) {
  	/** @TODO: Find the right field for this field */
  	$document->addField('ss_publication_date_text', $publication_date_text);
  	$document->addField('ss_pubdate', $publication_date_text); // remove this?
  	$document->addField('ss_spubdate', strtolower($publication_date_text));
  } else {

  }
  unset($publication_date_text);
  /** Publication location (field_publication_location) */
  /** single-value field */
  $publication_location = $wrapper->field_publication_location->value();
  if (!empty($publication_location)) {
  	$searchableFields[] = $publication_location;
  	$document->addField('ss_publication_location', $publication_location);
  	$document->addField('ss_publocation', $publication_location); // remove this? @kate- when search options are fixed
  }
  unset($publication_location);
  /** Subject (field_subject) */
  /** multi-value field */
  $subjects = $wrapper->field_subject->value();
  if (!empty($subjects)) {
  	foreach ($subjects as $index=>$subject) {
  		$document->addField('sm_subject_label', $subject->name);
  		$searchableFields[] = $subject->name;
		if ($index == 0) {
			$document->addField('iass_subject', $subject->name);
		}
  		$document->addField('zm_subject',json_encode(array('name'=>$subject->name,'tid' => $subject->tid, 'vid' => $subject->vid)));
  	}
  }
  unset($subjects);
  unset($subject);
  /** Topic (field_topic) */
  /** multi-value field */
  $topics = $wrapper->field_topic->value();
  if (!empty($topics)) {
  	foreach ($topics as $topic) {
  		$searchableFields[] =  $topic;
  		$document->addField('sm_topic', $topic);
  	}
  }
  unset($topics);
  unset($topic);
  /** Language code (field_language_code) */
  /** multi-value field */
  $language_codes = $wrapper->field_language_code->value();
  if (!empty($language_codes)) {
  	foreach ($language_codes as $language_code) {
  		$document->addField('sm_language_code', $language_code);
  	}
  }
  unset($language_codes);
  unset($language_code);
  /** Language (field_language) */
  /** multi-value field */
  $field_language = $wrapper->field_language->value();
  if (!empty($field_language)) {
  	foreach ($field_language as $language) {
  		$document->addField('sm_language', $language);
  	}
  }
  unset($field_language);
  unset($language);

  /** Single value fields */
  $document->addField('ss_title', $book_field_title);
  // Strip HTML Tags
  $title_sort = strip_tags($book_field_title);
  // Trim the string of leading/trailing space
  $title_sort = trim($title_sort);
  $title_sort = dlts_solr_remove_accents($title_sort);
  // Clean up things like &amp;
  $title_sort = html_entity_decode($title_sort);
  // Strip out any url-encoded stuff
  $title_sort = urldecode($title_sort);
  // Replace Multiple spaces with single space
  $title_sort = preg_replace('/ +/', ' ', $title_sort);
  // To lower-case
  $title_sort = strtolower($title_sort);
  /** Sort fields */
  $document->addField('ss_longlabel', $title_sort);
  $document->addField('iass_longlabel', $title_sort);
  /** add translations */
  dlts_solr_index_book_translations($document, $entity, $entity_type, $env_id, $searchableFields);
  unset($document->content);
  $document->addField('content', implode($searchableFields, ' '));
}
function mbstring_binary_safe_encoding( $reset = false ) {
	static $encodings = array();
	static $overloaded = null;
	if ( is_null( $overloaded ) )
		$overloaded = function_exists( 'mb_internal_encoding' ) && ( ini_get( 'mbstring.func_overload' ) & 2 );
	if ( false === $overloaded )
		return;
	if ( ! $reset ) {
		$encoding = mb_internal_encoding();
		array_push( $encodings, $encoding );
		mb_internal_encoding( 'ISO-8859-1' );
	}
	if ( $reset && $encodings ) {
		$encoding = array_pop( $encodings );
		mb_internal_encoding( $encoding );
	}
}
function reset_mbstring_encoding() {
	mbstring_binary_safe_encoding( true );
}
