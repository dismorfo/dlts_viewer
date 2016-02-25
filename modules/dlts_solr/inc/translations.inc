<?php

function dlts_solr_index_book_translations(&$document, &$entity, $entity_type, $env_id, &$searchableFields ) {
  if ($entity->tnid) {
    $translations = translation_node_get_translations($entity->tnid);
    foreach ($translations as $lang => $translation) {
    	$subjects = array();
      if ($translation->nid != $entity->nid) {
        // load node
        $node = node_load($translation->nid);
        $language = $node->language;
        $prefix = '_' . $language;
        $title = $node->title;
        // wrapp multi-volume entity with Entity API wrapper
        $wrapper = entity_metadata_wrapper('node', $node);
        /** Title (field_title) */
        /** single-value field */
        $title_long = $wrapper->field_title->value();
        if (!empty($title_long)) {
        	$title = trim($title_long);
        	$document->addField('ss' . $prefix . '_title_long', $title);
        	$document->addField('ss' . $prefix . '_stitle', dlts_solr_remove_accents(strtolower($title)));
        	$document->addField('iass' . $prefix . '_longlabel',  dlts_solr_remove_accents(strtolower($title)));
        }
        unset($title_long);
        $searchableFields[] = $title;
        /** multi-value field */
        dlts_solr_index_book_collection($wrapper->field_collection->value(), $document, $node, $searchableFields);
        /** multi-value field */
        dlts_solr_index_book_partners($wrapper->field_partner->value(), $document, $node, $searchableFields);
        /** Author (field_author) */
        /** multi-value field */
        $authors = $wrapper->field_author->value();
        if (!empty($authors)) {
        	foreach ($authors as $author) {
        		$searchableFields[] = $author;
        		$document->addField('sm' . $prefix . '_author', $author);
        	}
        }
        unset($authors);
        unset($author);
        /** Creator (field_creator) */
        /** multi-value field */
        $creators = $wrapper->field_creator->value();
        if (!empty($creators)) {
        	foreach ($creators as $creator) {
        		$document->addField('sm' . $prefix . '_creator', $creator);
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
        		$document->addField('sm' . $prefix . '_editor', $editor);
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
        		$document->addField('sm' . $prefix . '_contributor', $contributor);
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
        		$document->addField('sm' . $prefix . '_publisher', $publisher);
        		$searchableFields[] =  $publisher;
        	}
        }
        unset($publishers);
        unset($publisher);
        /** Publication location (field_publication_location) */
        /** single-value field */
        $publication_location = $wrapper->field_publication_location->value();
        if (!empty($publication_location)) {
        	$searchableFields[] = $publication_location;
        	$document->addField('ss' . $prefix . '_publication_location', $publication_location);
        	$document->addField('ss' . $prefix . '_spublocation', dlts_solr_remove_accents(strtolower($publication_location)));
        }
        unset($publication_location);
      }
    }
  }
}
