<?php

/** needs work */

$series_data = array () ;

$series_data['collections'] = array () ;

$series_data['books'] = array () ;

/** add series utilities functions */
module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.series');

/** find the nids of all the books that belong to this series */
$books_nids = dlts_utilities_series_books_nids ( $entity ) ;

	  if ( $books_nids ) {

			/** load the series book */
		  $series_books = node_load_multiple ( $books_nids ) ;

		  foreach ( $series_books as $series_book ) {

		  	// Wrap it with Entity API
            $series_book_wrapper = entity_metadata_wrapper('node', $series_book);

			$collections = $series_book_wrapper->field_collection->value();

			foreach ( $collections as $collection ) {

			  $collection_wrapper = entity_metadata_wrapper('node', $collection);

			  $collection_label = $collection_wrapper->field_name->value();

			  $collection_identifier = $collection_wrapper->field_identifier->value();

			  $collection_code = $collection_wrapper->field_code->value();

			  if ( ! isset ( $series_data['collections'][$collection_identifier] ) ) {
	            $series_data['collections'][$collection_identifier] = array (
			      'label' => $collection_label,
			      'identifier' => $collection_identifier,
			      'code' => $collection_code,
  			    );
			  }

			}

			$book = $series_book_wrapper->field_book->value();

			if ( $book ) {

			  $book_wrapper = entity_metadata_wrapper('node', $book);

			  $volume = $series_book_wrapper->field_volume_number->value();

			  $book_identifier = $book_wrapper->field_identifier->value() ;

			  $book_representative_image = $book_wrapper->field_representative_image->value() ;

			  $book_label = $book_wrapper->field_title->value() ;

			  if ( ! isset ( $series_data['books'][$book_identifier] ) ) {

	 		    $series_data['books'][$book_identifier] = array () ;

				$series_data['books'][$book_identifier]['identifier'] = $book_identifier;

				$series_data['books'][$book_identifier]['label'] = $book_label;

                $partners = dlts_utilities_book_get_partner($book);

                $authors = dlts_utilities_book_get_authors($book);

                $creators = dlts_utilities_book_get_creator($book);

                $contributors = dlts_utilities_book_get_contributor($book);

                $publishers = dlts_utilities_book_get_publisher($book);

                $editors = dlts_utilities_book_get_editor($book);

                $publication_location = dlts_utilities_book_get_publication_location($book);

                $publication_date = dlts_utilities_book_get_field_publication_date_text($book);

                $subjects_list = dlts_utilities_book_get_subjects($book);

                if ( $multivol = dlts_utilities_book_get_multivol_book( $book ) ) {
                  if ( $volume = dlts_utilities_multibook_get_volume( $multivol ) ) {
                    $book_field_title = t('@title Vol. @volume', array( '@title' => $book_field_title, '@volume' => $volume ) );
                  }
                }

	            if ( $series = dlts_utilities_book_get_series_book( $book ) ) {
                  if ( $series_volume = dlts_utilities_series_get_volume ( $series ) ) {
                    $book_series_title = t('@title ser. @volume', array( '@title' => $book_field_title, '@volume' => $series_volume ) );
                  }
	            }

				$series_data['books'][$book_identifier]['partners'] = array();
				foreach ( $partners as $partner )  {
				  $series_data['books'][$book_identifier]['partners'][] = array ( 'value' => $partner);
				}

				$series_data['books'][$book_identifier]['authors'] = array();
				foreach ( $authors as $author )  {
				  $series_data['books'][$book_identifier]['authors'][] = array ( 'value' => $author);
				}

				$series_data['books'][$book_identifier]['contributors'] = array();
				foreach ( $contributors as $contributor )  {
				  $series_data['books'][$book_identifier]['contributors'][] = array ( 'value' => $contributor);
				}

                $series_data['books'][$book_identifier]['creators'] = array();
				foreach ( $creators as $creator )  {
				  $series_data['books'][$book_identifier]['creators'][] = array ( 'value' => $creator);
				}

                $series_data['books'][$book_identifier]['publishers'] = array();
				foreach ( $publishers as $publisher )  {
				  $series_data['books'][$book_identifier]['publishers'][] = array ( 'value' => $publisher);
				}

                $series_data['books'][$book_identifier]['editors'] = array();
				foreach ( $editors as $editor )  {
				  $series_data['books'][$book_identifier]['editors'][] = array ( 'value' => $editor);
				}

				$series_data['books'][$book_identifier]['subjects'] = array();
				foreach ( $subjects_list as $subject )  {
				  $series_data['books'][$book_identifier]['subjects'] = array ( 'tid' => $subject->tid, 'label' => $subject->name ) ;
				}

				$series_data['books'][$book_identifier]['publication_location'] = $publication_location ;

				$series_data['books'][$book_identifier]['publication_date'] = $publication_date ;

			    if ( $book_representative_image ) {
			      $series_data['books'][$book_identifier]['representative_image_medium'] = image_style_url('medium', $book_representative_image['uri']) ;
				  $series_data['books'][$book_identifier]['representative_image_thumbnail'] = image_style_url('thumbnail', $book_representative_image['uri']) ;
			    }

			  }

			}

		  }

		}

      $series_wrapper = entity_metadata_wrapper('node', $entity);

      $document->addField('ss_series_label', $entity->title ) ;

	  $series_identifier = $series_wrapper->field_identifier->value() ;

	  $series_representative_image = $series_wrapper->field_representative_image->value() ;

	  if  ( $series_representative_image ) {
  	    $styles = array_keys ( image_styles() ) ;
	    foreach ( $styles as $style ) {
          $document->addField('zs_image_' . $style , image_style_url( $style, $series_representative_image['uri'] ) ) ;
	    }
	  }

	  $document->addField('ss_identifier', $series_identifier ) ;

      foreach ( $series_data['collections'] as $collection ) {
         $document->addField('sm_series_collection_identifier', $collection['identifier']) ;
	     $document->addField('sm_series_code', $collection['code']) ;
	  }

	   $document->addField('zs_data', json_encode( $series_data )) ;
