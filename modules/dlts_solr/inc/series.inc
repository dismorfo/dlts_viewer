<?php

function dlts_solr_index_series($document, $entity, $entity_type, $env_id) {

	$language = $entity->language;
	
	$prefix = '';
	
	if ($language != 'en') {
		$prefix = $language . '_';
	}

  $data = array();

  $data['collections'] = array();

  $data['books'] = array();

  $query = new EntityFieldQuery;

  $nids = array();

  $series_wrapper = entity_metadata_wrapper('node', $entity);
   
  $series_identifier = $series_wrapper->field_identifier->value();

  // field data from node    
  $document->addField('ss_identifier', $series_identifier);    
  // field data from node
  $document->addField('ss_series_identifier', $series_identifier);
  // field data from node    
  $document->addField('ss_series_label', $entity->title);    
  
  // find books associated with this series
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_series_book')
    ->fieldCondition('field_series', 'nid', array($entity->nid))
    ->execute();

  if (!empty($result['node'])) {

  	$nids = array_keys($result['node']);

		/** load the series book */
		$nodes = node_load_multiple($nids);

		foreach ($nodes as $node) {
		  
		  // Wrap it with Entity API
      $wrapper = entity_metadata_wrapper('node', $node);
			
			$collections = $wrapper->field_collection->value();
			
			foreach ($collections as $collection) {

			  $collection_wrapper = entity_metadata_wrapper('node', $collection);

			  $collection_label = $collection_wrapper->field_name->value();

			  $collection_identifier = $collection_wrapper->field_identifier->value();

			  $collection_code = $collection_wrapper->field_code->value();
			  
			  if (!isset($data['collections'][$collection_identifier])) {
	        $data['collections'][$collection_identifier] = array('label' => $collection_label, 'identifier' => $collection_identifier, 'code' => $collection_code);
			  }

			}
			
     }
   }
   
   foreach ($data['collections'] as $collection ) {
     $document->addField('sm_series_collection_identifier', $collection['identifier']);
	   $document->addField('sm_series_code', $collection['code']);	   
	 }
	 
	 $document->addField('zs_data', json_encode($data));         
   
}
