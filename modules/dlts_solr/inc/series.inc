<?php

function dlts_solr_series_books_nids($entity) {
  $query = new EntityFieldQuery;
  $keys = array();
  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_series_book')
                  ->fieldCondition('field_series', 'nid', array($entity->nid))
                  ->execute();
  if (!empty($result['node'])) {
    $keys = array_keys($result['node']);
  }
  return $keys;
}

function dlts_solr_book_get_series_book($entity) {
  $query = new EntityFieldQuery;
  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_series_book')
                  ->fieldCondition('field_book', 'nid', array($entity->nid))
                  ->execute();
  if (!empty($result['node'])) {
    $keys = array_keys($result['node']);
    return node_load(array_pop($keys));
  }
  return FALSE;
}

function dlts_solr_multibook_get_volume($entity) {
  $value = array();
  if (isset($entity->field_volume_number)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_volume_number->value();
  }
  return $value;
}

function dlts_solr_book_get_multivol_book($entity) {
  $query = new EntityFieldQuery;
  $keys = array();
  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_multivol_book')
                  ->fieldCondition('field_book', 'nid', array($entity->nid))
                  ->execute();
  if (!empty($result['node'])) {
    $keys = array_keys($result['node']);
    return node_load(array_pop($keys));
  }
  return FALSE;
}

function dlts_solr_series_get_identifier($entity) {
  $value = NULL;
  if (isset($entity->field_identifier)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_identifier->value();
  }
  return $value;
}

function dlts_solr_series_book_get_series($entity) {
  $value = array();
  if (isset($entity->field_series)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_series->value();
  }
  return $value;
}

/**
 * Use Entity API (module) to returns book partner field
 */
function dlts_solr_book_get_partner($entity) {
  $partners = array();
  if (isset($entity->field_partner)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    foreach ($ewp->field_partner->getIterator() as $delta => $w) {
      $partners[] = $w->field_name->value();
    }
  }
  return $partners;
}

/**
 * Author/Contributor
 * Use Entity API (module) to returns book authors field
 */
function dlts_solr_book_get_authors($entity) {
  $value = array();
  if (isset($entity->field_author)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_author->value();
  }
  return $value;
}

function dlts_solr_book_get_creator($entity) {
  $value = array();
  if (isset($entity->field_creator)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_creator->value();
  }
  return $value;
}

function dlts_solr_series_get_volume($entity) {
  $value = array();
  if (isset($entity->field_volume_number)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_volume_number->value();
  }
  return $value;
}

function dlts_solr_book_get_contributor($entity) {
  $value = array();
  if (isset($entity->field_contributor)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_contributor->value();
  }
  return $value;
}

function dlts_solr_book_get_publisher($entity) {
  $value = array();
  if (isset($entity->field_publisher)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_publisher->value();
  }
  return $value;
}

function dlts_solr_book_get_editor($entity) {
  $value = array();
  if (isset($entity->field_editor)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_editor->value();
  }
  return $value;
}

function dlts_solr_book_get_publication_location($entity) {
  $value = array();
  if (isset($entity->field_publication_location)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_publication_location->value();
  }
  return $value;
}

function dlts_solr_book_get_field_publication_date_text($entity) {
  $value = array();
  if (isset($entity->field_publication_date_text)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_publication_date_text->value();
  }
  return $value;
}

function dlts_solr_book_get_subjects($entity) {
  $value = array();
  if (isset($entity->field_subject)) {
    $ewp = entity_metadata_wrapper('node', $entity);
    $value = $ewp->field_subject->value();
  }
  return $value;
}

function dlts_solr_index_series(&$document, &$entity) {

  module_load_include('inc', 'dlts_viewer', 'inc/djatoka');

  $document->addField('ss_series_label', $entity->title);

  /** wp stand for wrapper */

  /** series */
  $series_wp = entity_metadata_wrapper('node', $entity);

  $document->addField('ss_identifier', $series_wp->field_identifier->value());

  /** stand for series data */
  $sd = array();

  $sd['collections'] = array();

  $sd['books'] = array();

  /** find the nids of all the books that belong to this series */
  $books_nids = dlts_solr_series_books_nids($entity);

  if ($books_nids) {
		/** load the series book */
		$series_books = node_load_multiple($books_nids);
		foreach ($series_books as $series_book) {
      $series_book_wp = entity_metadata_wrapper('node', $series_book);
      foreach ($series_book_wp->field_collection->value() as $collection) {
			  $collection_wp = entity_metadata_wrapper('node', $collection);
			  $collection_label = $collection_wp->field_name->value();
			  $collection_identifier = $collection_wp->field_identifier->value();
			  $collection_code = $collection_wp->field_code->value();
			  if (!isset($sd['collections'][$collection_identifier])) {
	        $sd['collections'][$collection_identifier] = array (
			      'label' => $collection_label,
			      'identifier' => $collection_identifier,
			      'code' => $collection_code,
  			  );
			  }
			}
			$book = $series_book_wp->field_book->value();
			if ($book) {
			  $book_wp = entity_metadata_wrapper('node', $book);
			  $volume = $series_book_wp->field_volume_number->value();
        /** $bi stand for book identifier */
        $bi = $book_wp->field_identifier->value();
        if (!isset($sd['books'][$bi])) {
          $sd['books'][$bi] = array();
          $sd['books'][$bi]['partners'] = array();
          $sd['books'][$bi]['authors'] = array();
          $sd['books'][$bi]['creators'] = array();
          $sd['books'][$bi]['contributors'] = array();
          $sd['books'][$bi]['publishers'] = array();
          $sd['books'][$bi]['editors'] = array();
          $sd['books'][$bi]['subjects'] = array();
          $sd['books'][$bi]['identifier'] = $bi;
				  $sd['books'][$bi]['label'] = $book_wp->field_title->value();
          $partners = dlts_solr_book_get_partner($book);
          $authors = dlts_solr_book_get_authors($book);
          $creators = dlts_solr_book_get_creator($book);
          $contributors = dlts_solr_book_get_contributor($book);
          $publishers = dlts_solr_book_get_publisher($book);
          $editors = dlts_solr_book_get_editor($book);
          $publication_location = dlts_solr_book_get_publication_location($book);
          $publication_date = dlts_solr_book_get_field_publication_date_text($book);
          $subjects_list = dlts_solr_book_get_subjects($book);
					$multivol = dlts_solr_book_get_multivol_book($book);
          if ($multivol) {
						$volume = dlts_solr_multibook_get_volume($multivol);
            if ($volume) {
              $book_field_title_arg = array('@title' => $book_field_title, '@volume' => $volume);
              $book_field_title = t('@title Vol. @volume', $book_field_title_arg);
            }
          }
          $series = dlts_solr_book_get_series_book($book);
          if ($series) {
            $series_volume = dlts_solr_series_get_volume($series);
            if ($series_volume) {
              $book_series_title_arg = array('@title' => $book_field_title, '@volume' => $series_volume);
              $book_series_title = t('@title ser. @volume', $book_series_title_arg);
            }
	        }
          foreach ($partners as $partner) {
				    $sd['books'][$bi]['partners'][] = array('value' => $partner);
				  }
          foreach ($authors as $author) {
				    $sd['books'][$bi]['authors'][] = array('value' => $author);
				  }
          foreach ($contributors as $contributor) {
				    $sd['books'][$bi]['contributors'][] = array('value' => $contributor);
				  }
          foreach ($creators as $creator) {
				    $sd['books'][$bi]['creators'][] = array('value' => $creator);
				  }
				  foreach ($publishers as $publisher) {
				    $sd['books'][$bi]['publishers'][] = array('value' => $publisher);
				  }
				  foreach ($editors as $editor) {
				    $sd['books'][$bi]['editors'][] = array('value' => $editor);
				  }
				  foreach ($subjects_list as $subject) {
				    $sd['books'][$bi]['subjects'] = array('tid' => $subject->tid, 'label' => $subject->name);
				  }
				  $sd['books'][$bi]['publication_location'] = $publication_location;
				  $sd['books'][$bi]['publication_date'] = $publication_date;

          /** $bri stand for book representativeimage */
          //$bri = $book_wp->field_representative_image->value();
          //if (!empty($bri)) {
          //	$pathinfo = pathinfo($bri['uri']);
          //	/** load Image Server util */
          //  $bri_i = '';
          //	if ($pathinfo['extension'] == 'jpg') {
          //		$bri_i = file_create_url($bri['uri']);
          //	}
          //	elseif ($pathinfo['extension'] == 'jp2') {
          //		$bri_i = dlts_viewer_djatoka_url($bri);
          //	}
          //	$document->addField('ss_representative_image', $bri_i);
          //}
          
			  }
			}
		}
    /** $c stand for collection */
    foreach ($sd['collections'] as $c) {
      $document->addField('sm_series_collection_identifier', $c['identifier']);
      $document->addField('sm_series_code', $c['code']);
  	}
  }

  $to_json = json_encode($sd);

	$document->addField('zs_data', $to_json);

}
