<?php

/**
 * @file
 * Datasource
 */

// @TODO: duplication
function dlts_viewer_multivolume_book($nid) {
	$query = new EntityFieldQuery;
	$result = $query
	->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'dlts_multivol_book')
	->fieldCondition('field_book', 'nid', $nid, '=')
	->execute();
	if (empty($result['node'])) return FALSE;
	$keys = array_keys($result['node']);
	return node_load_multiple($keys);
}

function dlts_viewer_is_multivolume_book($entity) {
	$query = new EntityFieldQuery;
	$result = $query
	->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'dlts_multivol_book')
	->fieldCondition('field_book', 'nid', array($entity->nid))
	->execute();
	if (!empty($result['node'])) {
		$keys = array_keys($result['node']);
		$nid = array_pop($keys);
		return node_load($nid);
	}
	return FALSE;
}

function dlts_viewer_is_series_book(&$entity) {
	$is_part_of = array();
	$entity->isSeriesBook = false;
	$entity_type = 'node';
	$bundle = 'dlts_series_book';
	$book_nid = $entity->nid;
	$book_entity = $entity;
	$book_wrapper = entity_metadata_wrapper($entity_type, $entity);
	$book_long_title = $book_wrapper->field_title->value();
	$collections = $book_wrapper->field_collection->value(); // multi-value field
	foreach ($collections as $key => $collection) {
		$collection_wrapper = entity_metadata_wrapper($entity_type, $collection);
		$collection_label = $collection_wrapper->field_name->value(); // single-value field
		$collection_code = $collection_wrapper->field_code->value(); // single-value field
		$query = new EntityFieldQuery;
		$result = $query
		  ->entityCondition('entity_type', $entity_type)
		  ->entityCondition('bundle', $bundle)
		  ->fieldCondition('field_book', 'nid', array($book_nid)) // single-value field
		  ->fieldCondition('field_collection', 'nid', array($collection->nid)) // multi-value field
		  ->execute();
		if (!empty($result['node'])) {
			$entity->isSeriesBook = true;
			$keys = array_keys($result['node']);
			$nid = array_pop($keys);
			$series_book_node = node_load($nid);
			$series_book_wrapper = entity_metadata_wrapper($entity_type, $series_book_node);
			$volume_number = $series_book_wrapper->field_volume_number->value();
			$volume_number_str = $series_book_wrapper->field_volume_number_str->value();
			if (!empty($volume_number_str)) {
				$book_long_title = $book_long_title . ' ' . $volume_number_str;
			}
			elseif (!empty($volume_number)) {
				$book_long_title = $book_long_title . ' ' . $volume_number;
			}
			$is_part_of[] = array(
				'volume_label' => $book_long_title,
			  'volume_number' => $volume_number,
			  'volume_number_str' => $volume_number_str,
				'collection_label' => $collection_label,
				'collection_code' => $collection_code,
			);
		}
	}
	$entity->seriesBook = $is_part_of;
}

function dlts_viewer_series_book($nid) {
	$query = new EntityFieldQuery;
	$result = $query
	->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'dlts_series_book')
	->fieldCondition('field_book', 'nid', $nid, '=')
	->execute();
	if (empty($result['node'])) {
		return FALSE;
	}
	$keys = array_keys($result['node']);
	return node_load_multiple($keys);
}
