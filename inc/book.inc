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
		$entity->title = t('@title @volume', array('@title' => $multivolume_book->title, '@volume' => $volume_number_str));
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
  	}

  	if ($next_page >= $sequence_count) {
  		$next_page_active = FALSE;
  	}

  	if ($sequence_max == 1) {
  		$previous_page_active = FALSE;
  	}

  	if ($next_page >= $sequence_count ) {
  		$next_page_active = FALSE;
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
