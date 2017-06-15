<?php

/**
 * @file
 * Books
 */

module_load_include('inc', 'dlts_viewer', 'inc/datasource'); // see if I can include just when needed

function dlts_viewer_dispatch_thumbnails($identifier) {
	/**
	 * Load helpers
	 */
	module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
  // hold query arguments
	$query_parameters = drupal_get_query_parameters();
	if (isset($query_parameters['sequence'])) {
    $sequence = $query_parameters['sequence'];
	}
  // start page
	$page = 0;
  // default amount of items to request
	$per_page = 10;
	// page number can be part of the query parameter
	if (isset($query_parameters['page'])) {
		$page = filter_xss($query_parameters['page']);
	}
	// app can overwrite the amount of items to display
	if (isset($query_parameters['rows'])) {
		$per_page = filter_xss($query_parameters['rows']);
	}
  // request thumbnails from MondoDB
	$thumbnails = dlts_viewer_mongodb_load_thumbnails($identifier, $page, $per_page);
  // no need to continue if request does not have items.
  if ($thumbnails) {
  	// count of thumbnails
  	$count = $thumbnails['count'];
  	// Items
  	$items = $thumbnails['items'];
    // Initialise the pager
    $current_page = pager_default_initialize($count, $per_page);
    // Pager
    $pager = theme('thumbnails_pager', array('quantity' => $count));

    $placeholder = $GLOBALS['base_url'] . '/' . drupal_get_path('module', 'dlts_viewer') . '/images/loadhold.png';

    $output = theme('dlts_book_thumbnails', array(
      'items' => $items,
      'pager' => $pager,
      'sequence' => $sequence,
      'placeholder' => $placeholder,
      )
    );

    return $output;
  }
  else {
    drupal_exit();
  }
}

function dlts_viewer_full_book_view(&$entity) {
  $module_path = drupal_get_path('module', 'dlts_viewer');
  $js_common_config = array('type' => 'file', 'scope' => 'footer');
  $css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);
  drupal_add_css($module_path . '/css/dlts_viewer.css', $css_common_config);
  drupal_add_js('http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js', 'external', array('group' => JS_LIBRARY, 'weight' => -100));
  drupal_add_js($module_path . '/js/crossframe.js', $js_common_config);
  drupal_add_js($module_path . '/js/modules/slider-base.js', $js_common_config);
  drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);
  drupal_add_js($module_path . '/js/ui.components.yui.js', $js_common_config);
  drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY));
  return node_view($entity , 'metadata', $entity->language);
}

function dlts_viewer_book_metadata_view($entity) {
	$element = node_view($entity, 'metadata');
	return drupal_render($element);
}

function dlts_viewer_pjax_book_view(&$entity) {
  $vars = array(
    'button_next' => $entity->button_next,
    'button_previous' => $entity->button_previous,
    'button_togglepage' => $entity->button_togglepage,
    'compositingLayerCount' => $entity->book_page_data['compositingLayerCount'],
    'dwtLevels' => $entity->book_page_data['dwtLevels'],
    'fid' => $entity->book_page_data['fid'],
    'height' => $entity->book_page_data['height'],
    'identifier' => $entity->identifier,
    'levels' => $entity->book_page_data['levels'],
    'sequence' => $entity->sequence,
    'sequence_count' => $entity->sequence_count,
    'thumbnailsPage' => $entity->thumbnailsPage,
    'thumbnailsRows' => $entity->thumbnailsRows,
    'thumbnailsURL' => $entity->thumbnailsURL,
    'title' => $entity->title,
    'uri' => $entity->book_page_data['uri'],
    'width' => $entity->book_page_data['width'],
  	'pageView' => $entity->pageView,
  );
  return theme('book_page', $vars);
}

function _language_field(&$entity) {
	$languages = language_list('enabled');
	$languages = $languages[1];
	$entity->lang_dir = (!isset($languages[$entity->language]->direction) || isset($languages[$entity->language]->direction) && $languages[$entity->language]->direction == 0) ? 'ltr' : 'rtl';
	$entity->lang_language = isset($languages[$entity->language]->language) ? $languages[$entity->language]->language : 'en';
	$entity->lang_name = isset($languages[$entity->language]->name) ? $languages[$entity->language]->name : 'English';
	$translations = translation_path_get_translations('node/' . $entity->nid);
	if (count($translations) > 1) {
		$entity->lang_options = array('#type' => 'markup', '#prefix' => '<select class="language">', '#suffix' => '</select>', '#markup' => '');
		foreach ($translations as $key => $index) {
			$url = url('books/' . $entity->identifier . '/display', array('absolute' => true, 'query' => array('lang' => $key)));
			$markup = array(
				'#tag' => 'option',
				'#theme' => 'html_tag',
				'#attributes' => array(
					'data-title' => t('@lang', array('@lang' => $languages[$key]->native)),
					'data-language' => $key,
					'title' => t('@lang', array('@lang' => $languages[$key]->native)),
					'class' => array('language', $key),
					'data-url' => $url,
					'value' => $url,
				),
				'#value' => t('@lang', array('@lang' => $languages[$key]->native)),
			);
			if ($entity->lang_language == $key) {
				$markup['#attributes']['selected'] = 'selected';
			}
			$entity->lang_options['#markup'] .= theme('html_tag', $markup);
		}
  }
}

function dlts_viewer_dispatcher($entity, $arg) {
	// check request type
	$is_pjax = dlts_viewer_is_pjax();
	// get query parameters
	$query_parameters = drupal_get_query_parameters();
	// check if this books is part of a multi-volume
	$multivolume_book = dlts_viewer_is_multivolume_book($entity);
	// if book is part of multi-volume; use append volume value to entity title
	if ($multivolume_book) {
		// wrapp multi-volume entity with Entity API wrapper
		$wrapper = entity_metadata_wrapper('node', $multivolume_book);
		// get volume value
		$volume_number_str = $wrapper->field_volume_number_str->value();
		// change entity title
		//$entity->title = t('@title @volume', array('@title' => html_entity_decode($multivolume_book->title, ENT_QUOTES), '@volume' => $volume_number_str));
		$entity->title = html_entity_decode(t('@title @volume', array('@title' => $multivolume_book->title, '@volume' => $volume_number_str)),ENT_QUOTES);
	}
	// check if this books is part of a series
	// $series_book = dlts_viewer_is_series_book($entity);
  // not sure if this is right. the book can part of multiple series book
  // figure out the right way to pick the title
	// maybe we can do something along the lines of: Book Title (also known as Series book title 1, Series book title 2)
	//if ($entity->isSeriesBook) {
		//dpm($entity->seriesBook);
		//foreach ($entity->seriesBook as $series_book) {
			// for now just get the last one (since we just have one for now)
			// talk about this with the group and fix later.
			//$entity->title = $series_book['volume_label'];
		//}
	//}

  // set entity title
	drupal_set_title($entity->title);
	// wrapp book entity with Entity API wrapper
	$book_wrapper = entity_metadata_wrapper('node', $entity);
	// book identifier
	$identifier = $book_wrapper->field_identifier->value();
	// set $identifier so that .tpl.php can use it
	$entity->identifier = $identifier;
	// book URL
	$book_url = url('node/' . $entity->nid , array('absolute' => TRUE));
	// set $url so that .tpl.php can use it
	$entity->url = $book_url;
	// set language
	_language_field($entity);
	// check for type of request: 1) book page or 2) display pane
  if (!is_array($arg) && $arg == 'display') {
  	// type of request
  	$display_type = 'book_metadata';
  	if ($is_pjax) {
  		$display_type = 'book_metadata_pjax';
  	}
  }
  else {
  	// type of request
  	$display_type = 'book_page';
  	if ($is_pjax) {
  		$display_type = 'book_page_pjax';
  	}
  	// we assume single page view
  	$is_single_page = TRUE;
  	$thumbnails_rows_per_page = 10;
  	$sequence = $arg['sequence'];
  	if (isset($query_parameters['rows'])) {
  		$thumbnails_rows_per_page = filter_xss($query_parameters['rows']);
  	}
  	if (is_array($sequence)) {
  	  /** get the first page of the requested sequence */
  		$page = min($sequence);
  		$sequence_max = max($sequence);
  		$sequence_min = min($sequence);
  		/** check if this is a double page view */
  		if (count($sequence) > 1) {
  			$is_single_page = FALSE;
  		}
  	}
  	else {
  		/** get the first page of the requested sequence */
  		$page = $sequence_max = $sequence_min = $sequence;
  		$is_single_page = TRUE;
  	}

  	$sequence_count = $book_wrapper->field_sequence_count->value();

  	$int_read_order = $book_wrapper->field_read_order->value();

  	$entity->read_order = ($int_read_order == 1) ? 'rtl' : 'ltr';

  	$next_page = $sequence_max + 1;

		$previous_page = $sequence_min - 1;

		$previous_page_active = TRUE;

		$next_page_active = TRUE;

  	if ($previous_page < 1) {
  		$previous_page = 1;
  	}

  	if ($next_page > $sequence_count) {
  		$next_page = $sequence_count;
  		$next_page_active = FALSE;
  	}

  	if ($sequence_max == 1) {
  		$previous_page_active = FALSE;
  	}

  	/** next page */
  	$next_pager_side = ($entity->read_order === 'ltr') ? 'pager-right' : 'pager-left';

  	$button_next = theme('dlts_viewer_pager_button' , array(
  	  'id' => 'next-page',
  		'text' => t('Go to next page'),
  		'url' => $book_url . '/' . $next_page . ((!$is_single_page) ? '?page_view=double' : ''),
  		'classes' => array('next', $next_pager_side, ($next_page_active ? 'active' : 'inactive'))
  	));

  	/** prev page */
  	$previous_pager_side = ($entity->read_order === 'ltr') ? 'pager-left' : 'pager-right';
  	$button_previous = theme('dlts_viewer_pager_button' , array(
      'id' => 'previous-page',
  		'type' => 'dlts_book_page',
  		'text' => t('Go to previous page'),
  		'url' => $book_url . '/' . $previous_page . ((! $is_single_page ) ? '?page_view=double' : ''),
  		'classes' => array('previous', $previous_pager_side, ($previous_page_active ? 'active' : 'inactive'))
  	));

  	/** Toggle between Single and Double page button */
  	$button_togglepage_operation = ($is_single_page) ? ' page-double' : ' page-single';
  	$button_togglepage = theme('dlts_viewer_pager_button' , array(
  			'id' => 'toggle-page',
  			'type' => 'dlts_book_page',
  			'text' => t('Toggle between single or double page'),
  			'url' => $book_url . '/' . $sequence_min . (($is_single_page ) ? '?page_view=double' : ''),
  			'classes' => array('toggle', 'active', $button_togglepage_operation),
  	)
  	);

  	/** Metadata button */
  	$button_metadata = theme('dlts_viewer_navbar_item', array(
  			'title' => t('Metadata'),
  			'path' => 'node/' . $entity->nid,
  			'attributes' => array(
  				'data-title' => t('Metadata'),
  				'title' => t('Show/hide metadata'),
  				'class' => array('button', 'metadata', 'on'),
  				'id' => array('button-metadata')
  			),
  			'fragment' => 'metadata',
  	)
  	);

  	/** fullscreen button */
  	$button_fullscreen = theme('dlts_viewer_navbar_item', array(
  			'title' => t('Toggle fullscreen'),
  			'path' => 'node/' . $entity->nid,
  			'attributes' => array(
  					'data-title' => t('Toggle fullscreen'),
  					'title' => t('Toggle fullscreen'),
  					'class' => array('button', 'fullscreen', 'off'),
  					'id' => array('button-fullscreen')
  			),
  			'fragment' => 'fullscreen',
  	)
  	);

  	/** thumbnails button */
  	$button_thumbnails = theme('dlts_viewer_navbar_item', array(
  			'title' => t('Pages'),
  			'path' => 'node/' . $entity->nid,
  			'attributes' => array(
  					'data-title' => t('Select page'),
  					'title' => t('Select page'),
  					'class' => array('button', 'thumbnails', 'off'),
  					'id' => array('button-thumbnails')
  			),
  			'fragment' => 'thumbnails',
  	)
  	);

  	/** Book page sequence number */
  	$entity->book_page_sequence_number = $page;

  	// Set navbar pager buttons according to read direction
  	if ($entity->read_order === 'ltr') {
  		$nav_pager_button_left_arrow = $button_previous;
  		$nav_pager_button_right_arrow = $button_next;
  	}
  	elseif ($entity->read_order === 'rtl') {
  		$nav_pager_button_left_arrow = $button_next;
  		$nav_pager_button_right_arrow = $button_previous;
  	}
  	else {
  		// Should never get here, but just in case, default to 'ltr' config
  		$nav_pager_button_left_arrow = $button_previous;
  		$nav_pager_button_right_arrow = $button_next;
  	}

  	$nav_pager_button_left_arrow = '<li class="navbar-item navbar-pager-left">' . $nav_pager_button_left_arrow . '</li>';
  	$nav_pager_button_right_arrow = '<li class="navbar-item navbar-pager-right">' . $nav_pager_button_right_arrow . '</li>';

  	$entity->navbar = theme('dlts_book_navbar', array(
  			'nav_buttons_left' => array($button_metadata , '<li class="navbar-item">' . $button_togglepage . '</li>' , $button_thumbnails, ),
  			'nav_buttons_right' => array($button_fullscreen, ),
  			'nav_buttons_arrows' => array($button_previous, $button_next),
  			'nav_buttons_pager_left_arrow' => $nav_pager_button_left_arrow,
  			'nav_buttons_pager_right_arrow' => $nav_pager_button_right_arrow,
  			'control_panel' => theme('dlts_book_controlpanel'),
  	));

  	$entity->book_page_data = $arg;

  	$entity->button_metadata = $button_metadata;

  	$entity->button_next = $button_next;

  	$entity->button_previous = $button_previous;

  	$entity->button_togglepage = $button_togglepage;

  	$entity->loading = theme('dlts_book_loading', array('sequence_number' => $page));

  	$entity->pane_metadata_hidden = FALSE;

  	$entity->sequence = $page;

  	$entity->sequence_count = $sequence_count;

  	$entity->slider = theme('dlts_book_slider' , array('slider_id' => 'slider', 'uri' => $book_url, 'lang_dir' => $entity->read_order, 'sequence_number' => $page, 'sequence_count' => $sequence_count));

  	$entity->thumbnailsPage = ceil($page / $thumbnails_rows_per_page) - 1;

  	$entity->thumbnailsRows = $thumbnails_rows_per_page;

  	$entity->thumbnailsURL = $book_url . '/thumbnails';

  	$entity->pageView = ($is_single_page) ? 'single' : 'double';

  	$arg['pageView'] = $entity->pageView;

  	$arg['thumbnailsUrl'] = $entity->thumbnailsURL;

  	$arg['thumbnailsRows'] = $entity->thumbnailsRows;

  	$arg['thumbnailsPage'] = $entity->thumbnailsPage;

  	$entity->book_page = theme('dlts_viewer_openlayers_image' , array('file' => $arg));

  }

  // types of returns
  switch ($display_type) {
  	case 'book_page':
  		return dlts_viewer_full_book_view($entity);
  	  break;
  	case 'book_page_pjax':
  		return dlts_viewer_pjax_book_view($entity);
  		break;
  	case 'book_metadata':
  		return dlts_viewer_full_book_view($entity);
  		break;
  	case 'book_metadata_pjax':
  		return dlts_viewer_book_metadata_view($entity);
  		break;
  }
}

/**
 * Route callback for books edit page
 * http://{site}/books/{identifier}/edit
 */
function _dlts_viewer_book_edit_redirect($node) {
  if (is_object($node)) {
    $wrapper = entity_metadata_wrapper('node', $node);
    $identifier = $wrapper->field_identifier->value();
    if ($identifier) {
      $path = drupal_lookup_path('source', 'books/' . $identifier);
      if ($path) {
      	drupal_goto($path . '/edit');
      }
    }
  }
  drupal_not_found();
}

function dlts_viewer_book_metadata_display($node) {
  return node_view($node , 'metadata' , $book->language );
}

/**
 * Menu callback; prints the loaded structure of the current node/user.
 */
function dlts_viewer_load_object($type, $object, $name = NULL) {
  $out = '';
  $name = isset($name) ? $name : $type;
  $entity = entity_metadata_wrapper('node', $object);
  $identifier = $entity->field_identifier->value();
  $title = $entity->title->value();
  $results = db_query("SELECT hash, document, language, created FROM {dlts_viewer_books_json} WHERE nid = :nid ORDER BY created DESC", array(':nid' => $object->nid));
  $header = array('Hash', 'Document', 'Language', 'Imported');
  foreach ($results as $key => $result) {
  	$document = $result->document;
  	$hash = $result->hash;
  	if ($key == 0) {
  		$json_str = file_get_contents($document);
  	}
  	if (file_exists($document)) {
  	  $result->document = basename($result->document);
  	  $result->created = date('F jS, Y h:i:s A',$result->created);
  	  $rows[] = (array) $result;
  	}
  }
  $out .= theme('table', array('header' => $header, 'rows' => $rows));
  $out .= '<h1>' . t('Node object') . '</h1>';
  $out .= kdevel_print_object($object, '$'. $name .'->');
  $out .= '<h1>' . t('JSON document (@hash)', array('@hash'=>$hash)) . '</h1>';
  $json = json_decode($json_str, true);
  $out .= kdevel_print_object($json, '$'. $name .'->');

  $series_book = dlts_viewer_series_book($object->nid);
  if (!empty($series_book)) {
  	$out .= '<h1>' . t('Series book') . '</h1>';
    $header = array('Node Id', 'Title', 'Is part of', 'Collections is part of');
    $rows = array();
    foreach ($series_book as $book) {
      $wrapper = entity_metadata_wrapper('node', $book);
      $collection = $wrapper->field_collection->value();
      $series = $wrapper->field_series->value();
      $collections = array();
      foreach ($collection as $item) {
    	  $collections[] = l($item->title, 'node/' . $item->nid);
      }
  	  $rows[] = array($book->nid,$book->title,l($series->title, 'node/' . $series->nid), implode(', ',$collections));
    }
    $out .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  $multivol_book = dlts_viewer_multivolume_book($object->nid);
  if (!empty($multivol_book)) {
  	$out .= '<h1>' . t('Multi Volume book') . '</h1>';
    $header = array('Node Id', 'Title', 'Is part of', 'Collections is part of');
    $rows = array();
    foreach ($multivol_book as $book) {
  	  $wrapper = entity_metadata_wrapper('node', $book);
  	  $collection = $wrapper->field_collection->value();
  	  $series = $wrapper->field_multivol->value();
  	  $collections = array();
  	  foreach ($collection as $item) {
  		  $collections[] = l($item->title, 'node/' . $item->nid);
  	  }
  	  $rows[] = array($book->nid,$book->title,l($book->title, 'node/' . $book->nid), implode(', ',$collections));
    }
    $out .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  return $out;
}

function dlts_book_api_book_metadata($book) {

  module_load_include('inc', 'dlts_utilities', 'inc/dlts_utilities.book');
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

  $metadata = $subjects = $partners = $dlts_collections=array();

  $ewrapper = entity_metadata_wrapper('node', $book);

  foreach ( $type['fields'] as $field ) {

    $name = str_replace('field_', '', $field['field_name']);

    $raw_value = $ewrapper->$field['field_name']->value();

    if ( $field['type'] == 'options_buttons' ) {
      $value = $field['settings']['allowed_values'][$raw_value];
      //convert values to binaries for some fields
      $raw_values=array( 
                       'scan_order'=>'Left to right',
                       'read_order'=>'Left to right',
                       'binding_orientation'=>'Horizontal', 
                       );
        if( array_key_exists($name,$raw_values)) {
          if( in_array($value,$raw_values) ) {
            $value='0';
          }
          else {
            $value='1';
          }
        }
    }

    else if ( $field['type'] == 'link_field' && isset( $raw_value['url'] ) ) {

      $value = $raw_value['url'];

    }

    else if ( $field['type'] == 'field_fileserver_link' ) {

      $value = array();

      foreach ( (array) $raw_value as $pdf_file) {
        if($name=='pdf_file') {
          $value[] = $pdf_file['uri'];
        }
        else {
          $value = $raw_value['uri'];
        }
      }
    }

    else if ( $field['type'] == 'field_fileserver_image' ) {
          $value =  $raw_value['uri'] ;
    }

    else if ( $field['type'] == 'date_text' ) {
          $value = $raw_value['value'];
    }

    else if ( $field['type'] == 'taxonomy_autocomplete' ) {

      $value = array();

      foreach ($raw_value as $taxonomy) {
        $value[] = $taxonomy->name;
      }

    }

    else if ( $field['type'] == 'node_reference_autocomplete' ) {

      $value = array( );

      foreach ($raw_value as $dlts_ref_node) {
        $ref_node_value = array(
                  'title'=>$dlts_ref_node->title,
                  'type'=>$dlts_ref_node->type,
                  'language'=> $dlts_ref_node->language,
                  'identifier'=> $dlts_ref_node->field_identifier['und'][0]['safe_value'],
                  'code'=>$dlts_ref_node->field_code['und'][0]['safe_value'],
                  'name'=>$dlts_ref_node->field_name['und'][0]['safe_value'],
                   );
       if( $dlts_ref_node->type=='dlts_collection' ) {
         $ref_node_value['partner']=$dlts_ref_node->field_partner['und'][0]['nid'];
       }
       $value[]=$ref_node_value;
      }
   }

    else {
       $value = $raw_value;
    }


    $fields[$name] = array (
      'label' => $field['label'],
      'value' => (array) $value,
       'type' => $field['type'],
       'machine_name' => $field['field_name'],
    );

  }
    //add partner information to collection
    $dlts_partners=$fields['partner']['value'];
    $dlts_collections=$fields['collection']['value'];

    foreach( $dlts_collections as &$dlts_collection) {
      $dlts_partner=dlts_book_get_partner_by_nid($dlts_collection['partner']);
      $dlts_collection['partner']= array(
                                     'title'=>$dlts_partner->title,
                                     'type'=>$dlts_partner->type,
                                     'language'=> $dlts_partner->language,
                                     'identifier'=> $dlts_partner->field_identifier['und'][0]['safe_value'],
                                     'code'=>$dlts_partner->field_code['und'][0]['safe_value'],
                                     'name'=>$dlts_partner->field_name['und'][0]['safe_value'],
                                   );

    }

    $fields['collection']['value']=$dlts_collections;

    $identifier = $fields['identifier']['value'][0];

  return array(
    'entity_title' => !empty($title) ? $title : $book->title,
    'identifier' => $identifier,
    'entity_language' => $book->language,
    'entity_status' => $book->status,
    'entity_type' => $book->type,
    'metadata' => $fields,
  );

}


function dlts_book_get_partner_by_nid ( $nid ) {

  $query = new EntityFieldQuery;

  $result = $query->entityCondition('entity_type', 'node')
                  ->entityCondition('entity_id', $nid)
                  ->entityCondition('bundle', 'dlts_partner')
                  ->execute();

 if ( ! empty ( $result['node'] ) ) {
    $keys = array_keys( $result['node'] );

    return node_load ( array_shift( $keys ) );
  }

}

function dlts_book_api_series($entity) {
	$nodes = array();
        $entity_wrapper =  entity_metadata_wrapper('node', $entity);
	$query = new EntityFieldQuery();
	$entities = $query->entityCondition('entity_type', 'node')
	  ->entityCondition('bundle', 'dlts_series_book')
	  //->propertyCondition('status', 1)
	  //->fieldCondition('field_book', 'nid', array('799003'), '=') // to test, we know this book (nyu_aco000177 and nid 799003 in stage) have series books
	  ->fieldCondition('field_book', 'nid', array($entity->nid), '=')
	  ->execute();
	if ($entities) {
		$keys = array_keys($entities['node']);
		$objects = entity_load('node', $keys);
		foreach($objects as $object) {
			$wrapper = entity_metadata_wrapper('node', $object);
			$book = $wrapper->field_book->value();
			$collections = array();
			$series = $wrapper->field_series->value();
			$series_wrapper = entity_metadata_wrapper('node', $series);
			$isPartOf = array(
        'title' => $series->title,
        'type' => $series->type,
        'language' => $series->language,
        'identifier' => $series_wrapper->field_identifier->value(),
			);
			foreach ((array)$entity_wrapper->field_collection->value() as $collection) {
				$collection_wrapper = entity_metadata_wrapper('node', $collection);
				$partners = array();
				foreach ((array)$collection_wrapper->field_partner->value() as $partner) {
					$partner_wrapper = entity_metadata_wrapper('node', $partner);
					$partners[] = array(
						'title' => $partner->title,
						'type' => $partner->type,
						'language' => $partner->language,
						'identifier' =>  $partner_wrapper->field_identifier->value(),
						'code' => $partner_wrapper->field_code->value(),
						'name' => $partner_wrapper->field_name->value(),
					);
				}
				$collections[] = array(
				  'title' => $collection->title,
          'type' => $collection->type,
          'language' => $collection->language,
          'identifier' => $collection_wrapper->field_identifier->value(),
          'code' => $collection_wrapper->field_code->value(),
					'name' => $collection_wrapper->field_name->value(),
					'partner' => $partners, // number of value is set to unlimited
				);
			}
			$nodes[] = array(
				'identifier' => $wrapper->field_identifier->value(),
				'type' => $object->type,
				'title' => $object->title,
			  'book' => $book->nid,
			  'volume_number' => $wrapper->field_volume_number_str->value().trim(),
			  'volume_number_str' => $wrapper->field_volume_number_str->value(),
				'isPartOf' => $isPartOf, // number of value is set to 1. map to field_series
				'collection' => $collections, // number of value is set to unlimited
			);
		}
	}
	return array('series' => $nodes);
}

function dlts_book_api_book($node, $listPages = FALSE) {
        $metadata = (array) dlts_book_api_book_metadata($node);
        $multivol = (array) dlts_book_api_book_multivol($node);
        //$series = (array) dlts_viewer_series_book($node->nid);
        $series = (array) dlts_book_api_series($node);
        $pages = array();
        $stitched = array();
  /*$query_parameters = drupal_get_query_parameters();
  if ($listPages || isset($query_parameters['listPages'])) {
    $pages =  (array) dlts_book_api_book_pages($node);
    $stitched = (array) dlts_book_api_dlts_book_stitched_page($node);
  }*/
  return array_merge($metadata, $multivol, $series, $pages, $stitched);
}

function dlts_book_api_book_multivol($entity) {
   $multivol = array('multivolume' => array('volume' => array()));
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol_book')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_book', 'nid', array($entity->nid), '=')
    ->execute();
  if ($entities) {
    $nid = array_pop(array_keys($entities['node']));
    $nodes = entity_load('node',array($nid));
    $node_wrapper = entity_metadata_wrapper('node', array_pop($nodes));
    $book_wrapper = entity_metadata_wrapper('node', $entity);
    $field_collection = $book_wrapper->field_collection->value();
    $collections = array();
    if ($field_collection) {
      foreach ($field_collection as $collection) {
        $collection_wrapper = entity_metadata_wrapper('node', $collection);
        $partner = $collection_wrapper->field_partner->value() ;
        $partner_wrapper = entity_metadata_wrapper('node', $partner[0]);
        $collections[] = array (
          'title' => $collection_wrapper->title->value(),
          'type' => $collection_wrapper->type->value(),
          'language' => $collection_wrapper->language->value(),
          'identifier' => $collection_wrapper->field_identifier->value(),
          'code' => $collection_wrapper->field_code->value(),
          'name' => $collection_wrapper->field_name->value(),
          'partner' => array(
            'title' => $partner_wrapper->title->value(),
            'type' => $partner_wrapper->type->value(),
            'language' => $partner_wrapper->language->value(),
            'identifier' => $partner_wrapper->field_identifier->value(),
            'code' => $partner_wrapper->field_code->value(),
            'name' => $partner_wrapper->field_name->value(),
          )
        );
      }
    }
    $field_multivol = $node_wrapper->field_multivol->value();
    $multivolume = array();
    if ($field_multivol) {
      $m_wrapper = entity_metadata_wrapper('node', $field_multivol);
      $multivolume[] = array(
        'title' => $m_wrapper->title->value(),
        'type' => $m_wrapper->type->value(),
        'language' => $m_wrapper->language->value(),
        'identifier' => $m_wrapper->field_identifier->value(),
      );
    }
    $multivol['multivolume']['volume'][] = array(
      'identifier' => $node_wrapper->field_identifier->value(),
      'volume_number' =>  $node_wrapper->field_volume_number->value(),
      'volume_number_str' => $node_wrapper->field_volume_number_str->value(),
      'collection' => $collections,
      'isPartOf' => $multivolume,
    );

  }
  return $multivol;
}
