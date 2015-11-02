<?php

function dlts_book_api_content_types ( ) {

  $fields = $out = array();

  // I can make a admin interface for this
  $field_exclude_list = array(
    'field_body',
    'body',
    'field_image',
    'field_stitch_image',
    'field_service_copy',
    'field_cropped_master',
    'field_representative_image',
    'field_ocr_text',
    'field_description',
    'field_handle',
    'field_other_version',
    'field_pdf_file',
  );

  $field_list = $field_exclude_list;

  $types = node_type_get_types();

  foreach ( $types as $bundle_name => $entity_type ) {

    $field_info_instances = field_info_instances('node', $bundle_name);

    foreach ( $field_info_instances as $field_name => $field_info_instance ) {

      if ( !in_array( $field_name, $field_list ) ) {

        $field_list[] = $field_name;

        $info = field_info_field( $field_name );

        $fields[] = array(
          'label' => $field_info_instance['label'],
          'field_name' => $field_info_instance['field_name'],
          'type' => $field_info_instance['widget']['type'],
          'cardinality' => (string) $info['cardinality'],
          'settings' => array(
            'allowed_values' => isset ( $info['settings']['allowed_values'] ) ? $info['settings']['allowed_values'] : null
          ),
        );
      }

      $out['types'][$bundle_name] = array(
        'name' => $entity_type->name,
        'type' => $entity_type->type,
      );

    }

  }

  $out['fields'] = $fields;

  return $out;

}

function dlts_book_api_book( $node, $listPages = TRUE) {

	$book = dlts_book_api_book_metadata( $node );

	if ( $listPages ) {

		$book =  array_merge( (array) $book, (array) dlts_book_api_book_pages( $node ) ) ;

	}

	return $book;

}

function dlts_book_api_books ( $start = 0, $length = 25 ) {

	global $language;

	$request_uri = drupal_parse_url(request_uri());

	if ( isset($request_uri['query']) ) {
		if ( isset($request_uri['query']['rows']) && is_numeric($request_uri['query']['rows']) ) {
			$length = $request_uri['query']['rows'];
		}
		if ( isset($request_uri['query']['start']) && is_numeric($request_uri['query']['start']) ) {
			$start = $request_uri['query']['start'];
		}
	}

	$countQuery = new EntityFieldQuery();

	$entitiesQuery = new EntityFieldQuery();

	$count = $countQuery->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'dlts_book')
	->propertyCondition('status', 1)
	->count()
	->execute();

	$entities = $entitiesQuery->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'dlts_book')
	->propertyCondition('status', 1)
	->range($start, $length)
	->execute();

	$keys = array_keys($entities['node']);

	$nodes = entity_load('node', $keys);

	// try to mimic Solr response
	$response = array(
			'response' => array(
					'numFound' => $count,
					'start' => $start,
					'docs' => array()
			)
	);

	foreach ( (array) $nodes as $node) {
		$response['response']['docs'][] = dlts_book_api_book( $node, FALSE);
	}

	return $response;

}

function dlts_book_api_content_by_field ( $field_name ) {

	$out = array();

	$field_name = filter_xss ( $field_name );

	$info = field_info_field( $field_name );

	$query_parameters = drupal_get_query_parameters();

	if ( $info ) {

		$m = array_keys( $info['storage']['details']['sql']['FIELD_LOAD_CURRENT'] );

		$table = $m[0];

		$c = array_keys( $info['columns'] );

		$columns = $c[0];

		if  ( $columns == 'fid' ) return array() ;

		$seek = $field_name . '_' . $columns;

		$table = 'field_data_' . $field_name;

		$query = 'SELECT DISTINCT ' . $seek . ' AS value FROM {' . $table . '}';

		$result = db_query($query);

		foreach ( $result as $record ) {

			if ( $columns == 'nid' ) {

				$node = node_load( $record->value );

				$raw_value = $record->value;

				$value = $node->title;

			}

			elseif ( $columns == 'tid' ) {

				$term = taxonomy_term_load( $record->value );

				$raw_value = $record->value;

				$value = $term->name;

			}

			else {
				$raw_value = $value = $record->value;
			}

			$out[] = array(
					'value' => $value,
					'raw_value' => $raw_value,
			);
		}

	}

	return $out;

}

function dlts_book_api_discovery_enviorment ( $field_name ) {

	$discovery = array();

	if ( module_exists ( 'apachesolr' ) ) {

		$environment = apachesolr_default_environment();

		$environments = apachesolr_load_all_environments();

		$discovery = array(
				'name' => $environments[$environment]['name'],
				'url' => $environments[$environment]['url'] . '/select',
		);

	}

	return $discovery;

}

/**
 * Generate metadata about a book.
 *
 * @param $dlts_book
 * A dlts_book node object
 * @return
 * A keyed array to be converted into JSON
 *
 */
function dlts_book_api_book_metadata ( $book ) {

	module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.book');
	module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.book_page');
	module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.collection');

	$type = dlts_utilities_content_types_type( $book->type );

	$fields = array();

	if (!is_object($book)) {
		return array(
				'argument' => $book,
				'code' => 1,
				'message' => t('Invalid book'),
				'status' =>t('fail'),
		);
	}

	$metadata = $subjects = $collections = array();

	$base_url = url('', array('absolute' => TRUE));

	$ewrapper = entity_metadata_wrapper('node', $book);

	foreach ( $type['fields'] as $field ) {

		$name = str_replace('field_', '', $field['field_name']);

		$raw_value = $ewrapper->$field['field_name']->value();

		if ( $field['type'] == 'options_buttons' ) {
			$value = $field['settings']['allowed_values'][$raw_value];
		}

		else if ( $field['type'] == 'link_field' && isset( $raw_value['url'] ) ) {

			$value = $raw_value['url'];

		}

		else if ( $field['type'] == 'file_generic' ) {

			$value = array();

			foreach ( (array) $raw_value as $pdf_file) {
				$value[] = file_create_url($pdf_file['uri']);
			}

		}

		else if ( $field['type'] == 'date_text' ) {
			$raw_value['value'];
		}

		else if ( $field['type'] == 'image_image' ) {
			$value = file_create_url($raw_value['uri']);
		}

		else if ( $field['type'] == 'taxonomy_autocomplete' ) {

			$value = array();

			foreach ($raw_value as $taxonomy) {
				$value[] = $taxonomy->name;
			}

		}

		else {
			$value = $raw_value;
		}

		$fields[$name] = array (
				'label' => $field['label'],
				'value' => (array) $value,
				// 'raw_value' => $raw_value,
				// 'type' => $field['type'],
				// 'machine_name' => $field['field_name'],
		);

	}

	$identifier = $fields['identifier']['value'][0];

	return array(
			'entity_title' => !empty($title) ? $title : $book->title,
			'identifier' => $identifier,
			'entity_path' => url('books/' . $identifier . '/1', array('absolute' => TRUE)),
			'base_url' => $base_url,
			'entity_language' => $book->language,
			'entity_status' => $book->status,
			'entity_type' => $book->type,
			'metadata' => $fields,
	);

}

function dlts_book_sequence_path ( $sequence, $type ) {
	if ($nid = dlts_book_sequence_lookup($sequence, $type)) {
		return array(
				'nid' => $nid,
				'node_path' => drupal_lookup_path( 'alias', 'node/' . $nid ),
		);
	}
	return FALSE;
}

function dlts_book_sequence_lookup ( $sequence, $type ) {
	$sequences = preg_split('/-/', $sequence);

	if ( isset($sequences[0]) && !empty($sequences[0]) ) {
		$sequence = (int) $sequences[0];
	} else {
		watchdog('dlts_book_api', 'Bad Request: Error finding book by sequence page.');
		return $sequence;
	}

	$types = array('dlts_book_page', 'dlts_book_stitched_page');

	if ( in_array( $type, $types ) ) {
		$query = "
      SELECT n.nid
        FROM {node} n
        LEFT JOIN {field_data_field_sequence_number_left} l ON n.nid = l.entity_id
        LEFT JOIN {field_data_field_sequence_number_right} r ON n.nid = r.entity_id
        WHERE n.status = :status
        AND n.type = :type
        AND ( l.field_sequence_number_left_value = :sequence OR r.field_sequence_number_right_value = :sequence )
        LIMIT 1";

		$result =  db_query($query, array(':status' => 1, ':type' => $type, ':sequence' => $sequence ));

		/*
		 * We just need the first one
		* Look for a better way to do this
		*/
		foreach ($result as $record) {
			return $record->nid;
		}
	}
}
