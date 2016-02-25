<?php

function dlts_solr_index_book_translations(&$document, &$entity, $entity_type, $env_id, &$searchableFields) {
  if ($entity->tnid) {
    $translations = translation_node_get_translations($entity->tnid);
    foreach ($translations as $lang => $translation) {
    	$subjects = array();
      if ($translation->nid != $entity->nid) {
        // load node
        $node = node_load($translation->nid);
        $language = $node->language;
        $prefix = '_' . $language;
        // wrapp entity with Entity API wrapper
        $wrapper = entity_metadata_wrapper('node', $node);
        /** Title (field_title) */
        /** single-value field */
        $title_long = $wrapper->field_title->value();
        if (isset($entity->solr_multivolume)) {
        	$multivolume = $entity->solr_multivolume;
        	$volume = '';
        	// volume_number_str can be empty, if so, try volume_number
        	if (isset($multivolume['volume_number_str']) && !empty($multivolume['volume_number_str'])) {
        		$volume = $multivolume['volume_number_str'];
        	}
        	elseif (isset($multivolume['volume_number']) && !empty($multivolume['volume_number'])) {
        		$volume = t('Vol.') . ' ' .  $multivolume['volume_number'];
        	}
        	$title_long = $title_long . ' ' . $volume;
        }

        $title_long = trim($title_long);

        /** the fields are the same but I don't want to remove title_long right now as it might be used somewhere **/
        /**
         * aof @TODO: will add for now, we should be using title_long since title is a Drupal thing and not the metadata
         * this was wrong in the old site
         */
        $document->addField('ss' . $prefix . '_title', $title_long);

        $document->addField('ss' . $prefix . '_stitle', dlts_solr_remove_accents(strtolower($title_long)));
        $document->addField('iass' . $prefix . '_longlabel',  dlts_solr_remove_accents(strtolower($title_long)));
        $document->addField('ss' . $prefix . '_title_long', $title_long);
        $searchableFields[] = $title_long;
        unset($title_long);

        /** multi-value field */
        dlts_solr_index_book_collection($wrapper->field_collection->value(), $document, $node, $searchableFields);
        /** multi-value field */
        dlts_solr_index_book_partners($wrapper->field_partner->value(), $document, $node, $searchableFields);
        /** Author (field_author) */
        /** multi-value field */
        $authors = $wrapper->field_author->value();
        if (!empty($authors)) {
        	foreach ($authors as $index => $author) {
        		$searchableFields[] = $author;
        		if ($index == 0) {
        		  $document->addField('ss_' . $prefix . '_s' . $label, dlts_solr_remove_accents(strtolower($author)));
        		  $document->addField('iass_' . $prefix . '_' . $label, dlts_solr_remove_accents(strtolower($author)));
        		}
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
