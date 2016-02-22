<?php

function dlts_solr_index_book_translations ( &$document, &$entity, $entity_type, $env_id, &$searchableFields ) {

  if ( $entity->tnid ) {
    $translations = translation_node_get_translations ( $entity->tnid ) ;

    foreach ( $translations as $lang => $translation )  {

      if ($translation->nid != $entity->nid ) {

        $subjects = array();

        $tnode = node_load($translation->nid);

        $title = dlts_utilities_book_get_title($tnode);

        $partners = dlts_utilities_book_get_partner($tnode);

        $authors = dlts_utilities_book_get_authors($tnode);

        $publishers = dlts_utilities_book_get_publisher($tnode);

        $book_translations_field_title = dlts_utilities_book_get_title($tnode);

        $publication_location = dlts_utilities_book_get_publication_location($tnode);

        $publication_date = dlts_utilities_book_get_field_publication_date_text($tnode);

        $subjects_list = dlts_utilities_book_get_subjects($tnode);

        if ( $multivol = dlts_utilities_book_get_multivol_book( $tnode ) ) {
          if ( $volume = dlts_utilities_multibook_get_volume( $multivol ) ) {
            $title = t('@title Vol. @volume', array( '@title' => $title,'@volume' => $volume ) );
          }
        }
  
        foreach ( $subjects_list as $subject ) {
          $subjects[] = $subject->name;
        }  
  
        $multi_value_fields = array(
          'partner' => $partners,
          'author' => $authors,    
          'publisher' => $publishers,
          'subject' => $subjects,
        );
  
        /** Multi value fields */
        foreach ( $multi_value_fields as $label => $fields ) {
          foreach ( $fields as $index => $value ) {
            $searchableFields[] = $value;
            /** sort does not work with multiple values; just get the first one */
            /** we might not want this */
            if ( $index == 0 ) {
              $document->addField('ss_' . $lang . '_s' . $label, dlts_solr_remove_accents( strtolower($value) ));
              $document->addField('iass_' . $lang . '_' . $label, dlts_solr_remove_accents( strtolower($value) ));  
            }
            $document->addField('sm_' . $lang . '_' . $label, $value);
          }
        }

        /** Single value fields */
        $document->addField('ss_' . $lang . '_title', $title);
        $document->addField('ss_' . $lang . '_publocation', $publication_location);
        $document->addField('ss_' . $lang . '_pubdate', $publication_date);  
  
        /** sort */
        $document->addField('ss_' . $lang . '_stitle', dlts_solr_remove_accents( strtolower($title) ) );
        $document->addField('iass_' . $lang . '_longlabel',  dlts_solr_remove_accents( strtolower($title) ) );  
    
        $document->addField('ss_' . $lang . '_spublocation', dlts_solr_remove_accents( strtolower($publication_location) ) );
        $document->addField('ss_' . $lang . '_spubdate', dlts_solr_remove_accents( strtolower($publication_date) ) );  
  
        /** to be dump to content */

        $searchableFields[] = $title;
        $searchableFields[] = $publication_date;
        $searchableFields[] = $publication_location;  
      }
    }
  }
}
