<?php

/*
 * Book getters. If you need any of this functionalities load with module_load_include
 * e.g., module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.book');
 */

/** Returns book identifier */
function dlts_utilities_book_get_identifier($node) {
  $field = dlts_utilities_field_get_first_item('node', $node, 'field_identifier');
  if (isset($field['safe_value']) && !empty($field['safe_value'])) {
    return $field['safe_value'];
  }
  return FALSE;
}

function dlts_utilities_book_get_multivol_book($node) {

  $query = new EntityFieldQuery;
  
  $result_multivol_identifier = $query
                  ->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_multivol_book')
                  ->fieldCondition('field_book', 'nid', array($node->nid))
                  ->execute();
  
  if (!empty($result_multivol_identifier['node'])) {

    $keys = array_keys($result_multivol_identifier['node']);

    return node_load(array_pop($keys));

  }
  
  else {
  	return FALSE;
  }

}

function dlts_utilities_book_get_series_book($node) {

  $query = new EntityFieldQuery;
  
  $result = $query
                  ->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_series_book')
                  ->fieldCondition('field_book', 'nid', array($node->nid))
                  ->execute();
  
  if ( !empty($result['node']) ) {

    $keys = array_keys($result['node']);

    return node_load(array_pop($keys));

  }
  
  else {
  	return FALSE;
  }

}

/**
 * Returns book is part of collection
 * @TODO: This is wrong, we need to remove all calls to this and
 * fix the return value. See: dlts_utilities_book_get_collection_identifiers
 */
function dlts_utilities_book_get_collection_identifier($node) {

  $field = dlts_utilities_field_get_first_item('node', $node, 'field_collection');
  
  if (isset($field['nid'])) {
    
    // Load the node by NID

    $entity = node_load($field['nid']);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);
    
    return $ewrapper->field_identifier->value();
    
  }

}

/**
 * Returns book is part of collection
 */
function dlts_utilities_book_get_collection_identifiers($node) {

  $collections = array() ;

  // Wrap it with Entity API
  $node_wrapper = entity_metadata_wrapper('node', $node);
  
  foreach ( $node_wrapper->field_collection->getIterator() as $entity ) {
    // Wrap it with Entity API
    $entity_wrapper = entity_metadata_wrapper('node', $entity->value());
    
    $collections[] = $entity_wrapper->field_identifier->value() ;
    
  }
  
  return $collections ;
  
}

/**
 * Returns book page sequence number
 */
function dlts_utilities_book_get_sequence_count($node) {
  $field = dlts_utilities_field_get_first_item('node', $node, 'field_sequence_count');
  return $field['value'];
}

/**
 * Returns book volume field
 */
function dlts_utilities_book_get_volume($node) {
  $field = dlts_utilities_field_get_first_item('node', $node, 'field_volume');
  return $field['value'];
}

/**
 * Returns book number field
 */
function dlts_utilities_book_get_number($node) {
	$field = dlts_utilities_field_get_first_item('node', $node, 'field_number');
	return $field['value'];
}

/**
 * Returns book pub date
 */
function dlts_utilities_book_get_publication_date($node) {
	$field = dlts_utilities_field_get_first_item('node', $node, 'field_publication_date');
	return $field['value'];
}

function dlts_utilities_book_get_description($node) {
  $field = dlts_utilities_field_get_first_item('node', $node, 'field_description');
  
  if (isset($field['safe_value']) && !empty($field['safe_value'])) {
    return $field['safe_value'];
  }
  else {
    return FALSE;
  }
}

function dlts_utilities_book_get_title($node) {
  $field = dlts_utilities_field_get_first_item('node', $node, 'field_title');
  
  if (isset($field['safe_value']) && !empty($field['safe_value'])) {
    return $field['safe_value'];
  }
  else {
    return t('Untitled');
  }
}

/*
 * 0 | Left to right
 * 1 | Right to left
 */

function dlts_utilities_book_get_read_order($node) {

  $field = dlts_utilities_field_get_first_item('node', $node, 'field_read_order');
  
  return (isset($field['value']) && !empty($field['value'])) ? $field['value'] : 0;
  
}

/**
 * Returns book representative image
 */
function dlts_utilities_book_get_representative_image($node, $image_style = 'medium') {

	$field = dlts_utilities_field_get_first_item('node', $node, 'field_representative_image');
	
	// book does not have a representative image; assume sequence number 1 is "visible" and have a service copy image
    if ( !$field ) {

      $query = new EntityFieldQuery;

      $result = $query
                  ->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_book_page')
                  ->propertyCondition('status', 1)
                  ->fieldCondition('field_is_part_of', 'value', dlts_utilities_book_get_identifier($node), '=')
				  ->fieldCondition('field_visible', 'value', TRUE, '=')
				  ->fieldCondition('field_sequence_number', 'value', 1, '=')
                  ->execute();

      if (!empty($result['node'])) {
        $keys = array_keys($result['node']);

        $page_node = node_load( array_pop( $keys ) );
		$field = dlts_utilities_field_get_first_item('node', $page_node, 'field_service_copy');
      }
    }
	
    $field['path'] = image_style_path($image_style, $field['uri']);
		
	$field['style_name'] = $image_style;
		
	return theme('image_style', $field);

}

/**
 * Returns book representative image
 */
function dlts_utilities_book_get_rights($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_rights->value();

}

/**
 * Returns series book volume
 */
function dlts_utilities_series_get_volume($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_volume_number->value();

}

/**
 * Returns multi book volume
 */
function dlts_utilities_multibook_get_volume($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_volume_number->value();

}

/**
 * Returns book publisher field
 */
function dlts_utilities_book_get_publisher($entity) {
  if (isset($entity->field_publisher)) {
    $ewrapper = entity_metadata_wrapper('node', $entity);
    return $ewrapper->field_publisher->value();
  }
}

/**
 * Returns book publication location field
 */
function dlts_utilities_book_get_publication_location($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_publication_location->value();

}

/**
 * Returns book publication date (text)
 */
function dlts_utilities_book_get_field_publication_date_text($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_publication_date_text->value();

}

/**
 * Returns book publication date (object)
 */
function dlts_utilities_book_get_field_publication_date ($entity) {
  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  return $ewrapper->field_publication_date->value();
}

/**
 * Returns book partner field
 */
function dlts_utilities_book_get_partner($entity) {
  $partners = array();
  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  if (isset($entity->field_partner)) {
    foreach ($ewrapper->field_partner->getIterator() as $delta => $wrapper) {
      $partners[] = $wrapper->field_name->value();
    }
  }  
  return $partners;
}

/**
 * Returns book subjects field
 */
function dlts_utilities_book_get_subjects($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_subject->value();

}

/**
 * Returns book subjects field
 */
function dlts_utilities_book_get_topic($entity) {
  if (isset($entity->field_topic)) {
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);  
    return $ewrapper->field_topic->value();
  }
}

/**
 * Returns book author
 */
function dlts_utilities_book_get_authors($entity) {
  if (isset($entity->field_author)) {
    $ewrapper = entity_metadata_wrapper('node', $entity);
    return $ewrapper->field_author->value();
  }
}

/**
 * Returns book author
 */
function dlts_utilities_book_get_creator($entity) {
  if (isset($entity->field_creator)) {
    $ewrapper = entity_metadata_wrapper('node', $entity);
    return $ewrapper->field_creator->value();
  }
}

/**
 * Returns book author
 */
function dlts_utilities_book_get_editor($entity) {
  if (isset($entity->field_editor)) {
    $ewrapper = entity_metadata_wrapper('node', $entity);
    return $ewrapper->field_editor->value();
  }
}

/**
 * Returns book contributor
 */
function dlts_utilities_book_get_contributor($entity) {
  if (isset($entity->field_editor)) {
    $ewrapper = entity_metadata_wrapper('node', $entity);
    return $ewrapper->field_editor->value();
  }
}

function dlts_utilities_book_get_isbn($entity) {
  if (isset($entity->field_isbn)) {
    $ewrapper = entity_metadata_wrapper('node', $entity);
    return $ewrapper->field_isbn->value();
  }
}

function dlts_utilities_book_get_subtitle($entity) {

  // Wrap it with Entity API
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  return $ewrapper->field_subtitle->value();

}

/**
 * Returns book author
 */
function dlts_utilities_book_get_series($entity) {

  // Wrap it with Entity API
  $book_wrapper = entity_metadata_wrapper('node', $entity);
  
  $collections_nid = array();

  $items = $output = '';
  
  $collections = $book_wrapper->field_collection->value();
  
  foreach ( $collections as $collection ) {  
    $collections_nid[] = $collection->nid;
  }
  
  $query = new EntityFieldQuery;

  $result = $query
              ->entityCondition('entity_type', 'node')
              ->entityCondition('bundle', 'dlts_series_book')
              ->propertyCondition('status', 1)
              ->fieldCondition('field_book', 'nid', $entity->nid)
			  ->fieldCondition('field_collection', 'nid', $collections_nid)
              ->execute();
              
  if ( !empty( $result['node'] ) ) {
  
    $keys = array_keys( $result['node'] );
    
    $nodes = node_load_multiple( $keys );
    
    foreach ( $nodes as $node ) {
  
      // Wrap it with Entity API
      $series_book_wrapper = entity_metadata_wrapper('node', $node);
      
      $a = $series_book_wrapper->field_series->value();
      
      $ea = entity_metadata_wrapper('node', $a);
      
      $series_title = $ea->title->value();

      $series_book_volume_number_str = $series_book_wrapper->field_volume_number_str->value();  
      
      if ( !empty( $series_book_volume_number_str ) ) {
        $series_book_volume_number_str =  ', ' . $series_book_volume_number_str;
      }
      
      $items .= '<div class="field-item field-serie">' . $series_title . $series_book_volume_number_str . '</div>';      
      
    }
  
  } 
  
  if (!empty($items) ) {
    $output = '
      <div class="field field-name-series clearfix">
        <div class="field-label">' . t('Series') . ':</div>
        <div class="field-items"> ' . $items . '</div>
      </div>';
  }

  return $output;  
  
}

/**
 * Returns book representative image
 */
function dlts_utilities_book_get_representative_image_uri($entity, $image_style = 'medium') {

    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);
    
    $field = $ewrapper->field_representative_image->value();
    
    if (!$field) {

      $query = new EntityFieldQuery;

      $result = $query
                  ->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_book_page')
                  ->propertyCondition('status', 1)
                  ->fieldCondition('field_is_part_of', 'value', dlts_utilities_book_get_identifier($entity), '=')
				  ->fieldCondition('field_visible', 'value', TRUE, '=')
				  ->fieldCondition('field_sequence_number', 'value', 1, '=')
                  ->execute();

      if (!empty($result['node'])) {

        $keys = array_keys($result['node']);

        $page_node = node_load( array_pop($keys) );
		
		$field = dlts_utilities_field_get_first_item('node', $page_node, 'field_service_copy');

      } 
      else {
        return FALSE;
      }

    }
	
    $field['path'] = image_style_path($image_style, $field['uri']);
		
	$field['style_name'] = $image_style;
		
	return file_create_url($field['uri']);

}



/**
 * Returns book first page
 */
function dlts_utilities_book_get_first_page($node) {
      	
  $query = new EntityFieldQuery;
	  
  $result = $query
              ->entityCondition('entity_type', 'node')
              ->entityCondition('bundle', 'dlts_book_page')
              ->propertyCondition('status', 1)
              ->fieldCondition('field_is_part_of', 'value', dlts_utilities_book_get_identifier($node), '=')
			  ->fieldCondition('field_visible', 'value', TRUE, '=')
			  ->fieldCondition('field_sequence_number', 'value', 1, '=')
              ->execute();

  if (!empty($result['node'])) {
    $keys = array_keys($result['node']);

    $page_node = node_load( array_pop($keys) );
		
    return url('node/'. $page_node->nid);
  } 
  else {
    return FALSE;
  }

  return $field['value'];
}

/**
 * Returns book representative image
 */
function dlts_utilities_book_get_representative_image_file ($node) {

	$field = dlts_utilities_field_get_first_item('node', $node, 'field_representative_image');
	
	// book does not have a representative image; assume sequence number 1 is "visible" and have a service copy image
    if ( !$field ) {

      $query = new EntityFieldQuery;

      $result = $query
                  ->entityCondition('entity_type', 'node')
                  ->entityCondition('bundle', 'dlts_book_page')
                  ->propertyCondition('status', 1)
                  ->fieldCondition('field_is_part_of', 'value', dlts_utilities_book_get_identifier($node), '=')
				  ->fieldCondition('field_visible', 'value', TRUE, '=')
				  ->fieldCondition('field_sequence_number', 'value', 1, '=')
                  ->execute();

      if ( !empty($result['node'])) {
        $keys = array_keys($result['node']);

        $page_node = node_load( array_pop( $keys ) );
		
		$field = dlts_utilities_field_get_first_item('node', $page_node, 'field_service_copy');

      } 

    }
	
    return $field;
		
}

