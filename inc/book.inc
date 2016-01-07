<?php

/**
 * @file
 * Books
 */

function dlts_viewer_dispatch_thumbnails($identifier) {
  $collection_name = 'dlts_book_page';
  $query = (object) array('isPartOf' => $identifier);
  $cursor = mongodb_collection($collection_name)->find($query);
  $count = $cursor->count();
  // no need to continue if request does not have items.
  if (!$count) {
    drupal_not_found();
    return;
  }
  $query_parameters = drupal_get_query_parameters();
  $page = 0;
  $per_page = 10;
  $height_of_each_thumbnail = 230;
  if (isset($query_parameters['page'])) { // page number can be part of the query parameter
    $page = filter_xss($query_parameters['page']);
  }
  if (isset($query_parameters['rows'])) { // app can overwrite the amount of items to display
    $per_page = filter_xss($query_parameters['rows']);
  }
  $is_single_page = TRUE; // we assume single page view
  $image_server_service = variable_get('dlts_viewer_djatoka_service', 'http://localhost:8080/adore-djatoka');
  $cursor->limit($per_page);
  $cursor = $cursor->skip($per_page * $page);
  $placeholder = url(drupal_get_path('module', 'dlts_viewer') . '/images/loadhold.png', array('absolute' => TRUE)); // we dont need this
  $items = array();
  foreach ($cursor as $result) { // iterate through the results
    // We will have a new structure. THIS WILL CHANGE leave this hack until new structure
    if (is_array($result['sequence'])) {
      $result['sequence'] = min($result['sequence']);
    }
    else {
      $result['sequence'] = $result['sequence'];
    }
    $result['url'] = url('books/' . $identifier . '/' . $result['sequence'], array('absolute' => TRUE));
    if (isset($result['cm']['uri'])) {
      $rft_id = file_create_url($result['cm']['uri']);
      $resolver = '/resolver?url_ver=Z39.88-2004&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/jpeg&svc.scale='. $height_of_each_thumbnail . '&rft_id=' . $rft_id;
      $result['imageServeURI'] = $image_server_service . $resolver;
    }
    $items[] = $result;
  }
  // Initialise the pager
  $current_page = pager_default_initialize($count, $per_page);
  // Pager
  $pager = theme('pager', array('quantity', $count));
  return theme('dlts_book_thumbnails',  array('items' => $items, 'placeholder' => $placeholder, 'pager' => $pager));
}

function dlts_viewer_full_book_view(&$entity) {
  $module_path = drupal_get_path('module', 'dlts_viewer');
  $js_common_config = array('type' => 'file', 'scope' => 'footer');
  $css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);
  drupal_add_css($module_path . '/css/dlts_viewer.css', $css_common_config);
  drupal_add_js('http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js', 'external', array('group' => JS_LIBRARY, 'weight' => -100));
  drupal_add_js($module_path . '/js/crossframe.js', $js_common_config);
  drupal_add_js($module_path . '/js/ui.crossframe.js', $js_common_config);
  drupal_add_js($module_path . '/js/modules/slider-base.js', $js_common_config);
  // drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);
  drupal_add_js($module_path . '/js/ui.components.yui.js', $js_common_config);
  drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY));
  return node_view($entity , 'metadata', $entity->language);
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
  );

  return theme('book_page', $vars);
}

function dlts_viewer_dispatcher($entity, $arg) {
  $is_pjax = dlts_viewer_is_pjax();
  $query_parameters = drupal_get_query_parameters();
  $is_single_page = TRUE; // we assume single page view
  $thumbnails_rows_per_page = 10;
  if (isset($query_parameters['rows'])) {
    $thumbnails_rows_per_page = filter_xss($query_parameters['rows']);
  }
  $sequence = $arg['sequence'];
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

  drupal_set_title($entity->title);

  $book_wrapper = entity_metadata_wrapper('node', $entity);

  $identifier = $book_wrapper->field_identifier->value();

  /** language */
  $languages = language_list('enabled');
  $languages = $languages[1];
  $entity->lang_dir = (!isset($languages[$entity->language]->direction) || isset($languages[$entity->language]->direction) && $languages[$entity->language]->direction == 0) ? 'ltr' : 'rtl';
  $entity->lang_language = isset($languages[$entity->language]->language) ? $languages[$entity->language]->language : 'en';
  $entity->lang_name = isset($languages[$entity->language]->name) ? $languages[$entity->language]->name : 'English';
  $translations = translation_path_get_translations('node/' . $entity->nid);
  if (count($translations) > 1) {
  	$entity->lang_options = array('#type' => 'markup', '#prefix' => '<select class="language">', '#suffix' => '</select>', '#markup' => '');
  	foreach ($translations as $key => $index) {
  		$url = url('books/' . $identifier . '/display', array('absolute' => true, 'query' => array('lang' => $key)));
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
  /** END language */

  $book_url = url('node/' . $entity->nid , array('absolute' => TRUE));

  $title = $book_wrapper->field_title->value();

  $sequence_count = $book_wrapper->field_sequence_count->value();
  $int_read_order = $book_wrapper->field_read_order->value();
  $entity->read_order = ($int_read_order == 1 ) ? 'rtl' : 'ltr';
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
  $button_next = theme('dlts_viewer_pager_button' , array(
    'id' => 'next-page',
    'active' => $next_page_active,
    'text' => t('Go to next page'),
    'url' => $book_url . '/' . $next_page . ((!$is_single_page) ? '?page_view=double' : ''),
    'operation' => 'next',
    'pager side' => $entity->read_order === 'ltr' ? 'pager-right' : 'pager-left',
    )
  );
  /** prev page */
  $button_previous = theme('dlts_viewer_pager_button' , array(
    'id' => 'previous-page',
    'active' => $previous_page_active,
    'type' => 'dlts_book_page',
    'text' => t('Go to previous page'),
    'url' => $book_url . '/' . $previous_page . ((! $is_single_page ) ? '?page_view=double' : ''),
    'operation' => 'previous',
    'pager side' => $entity->read_order === 'ltr' ? 'pager-left' : 'pager-right',
    )
  );

  /** Toggle between Single and Double page button */
  $button_togglepage = theme('dlts_viewer_pager_button' , array(
    'id' => 'toggle-page',
    'active' => TRUE,
    'type' => 'dlts_book_page',
    'text' => t('Toggle between single or double page'),
    'url' => $book_url . '/' . $sequence_min . (($is_single_page ) ? '?page_view=double' : ''),
    'operation' => ($is_single_page) ? ' page-double' : ' page-single',
    )
  );

  /** Metadata button */
  $button_metadata = theme('dlts_viewer_navbar_item', array(
    'title' => t('Metadata'),
    'path' => 'node/' . $entity->nid,
    'attributes' => array(
      'data-title' => t('Metadata'),
      'title' => t('Show/hide metadata'),
      'class' => array('button', 'metadata', 'on',),
      'id' => array('button-metadata',)
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

  $nav_pager_button_left_arrow = '<li class="navbar-item navbar-pager-left">' .
                                 $nav_pager_button_left_arrow                 .
                                 '</li>';
  $nav_pager_button_right_arrow = '<li class="navbar-item navbar-pager-right">' .
                                 $nav_pager_button_right_arrow                 .
                                 '</li>';

  $entity->navbar = theme('dlts_book_navbar', array(
    'nav_buttons_left' => array($button_metadata , '<li class="navbar-item">' . $button_togglepage . '</li>' , $button_thumbnails, ),
    'nav_buttons_right' => array($button_fullscreen, ),
    'nav_buttons_arrows' => array($button_previous, $button_next),
    'nav_buttons_pager_left_arrow' => $nav_pager_button_left_arrow,
    'nav_buttons_pager_right_arrow' => $nav_pager_button_right_arrow,
    'control_panel' => theme('dlts_book_controlpanel'),
  ));

  $entity->book_page = theme('dlts_viewer_openlayers_image' , array('file' => $arg));
  $entity->book_page_data = $arg;
  $entity->button_metadata = $button_metadata;
  $entity->button_next = $button_next;
  $entity->button_previous = $button_previous;
  $entity->button_togglepage = $button_togglepage;
  $entity->identifier = $identifier;
  $entity->loading = theme('dlts_book_loading', array('sequence_number' => $page));
  $entity->pane_metadata_hidden = FALSE;
  $entity->sequence = $page;
  $entity->sequence_count = $sequence_count;
  $entity->slider = theme('dlts_book_slider' , array('slider_id' => 'slider', 'uri' => $book_url, 'lang_dir' => $entity->read_order, 'sequence_number' => $page, 'sequence_count' => $sequence_count));
  $entity->thumbnailsPage = ceil($page / $thumbnails_rows_per_page) - 1;
  $entity->thumbnailsRows = $thumbnails_rows_per_page;
  $entity->thumbnailsURL = $book_url . '/thumbnails';
  $entity->url = $book_url;

  if (!$is_pjax) {
  	return dlts_viewer_full_book_view($entity);
  }
  else {
  	return dlts_viewer_pjax_book_view($entity);
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
