<?php

/**
 * @file
 * Books
 */

function dlts_viewer_dispatch_thumbnails($identifier) {

  $page = 0 ;

  $per_page = 10;

  $query_parameters = drupal_get_query_parameters();

  if (isset($query_parameters['page'] )) {
    $page = filter_xss($query_parameters['page']);
  }

  if (isset($query_parameters['rows'] )) {
    $per_page = filter_xss($query_parameters['rows']);
  }

  /** We assume single page view */
  $is_single_page = TRUE;

  $collection_name = 'dlts_book_page';

  $query = (object) array('isPartOf' => $identifier);

  $cursor = mongodb_collection($collection_name )->find($query);

  $count = $cursor->count();

  if ($count) {

    $cursor->limit($per_page);

    $cursor = $cursor->skip($per_page * $page);

    $items = array();

    $image_server_service = variable_get('dlts_image_djatoka_service', 'http://localhost:8080/adore-djatoka') ;

    $placeholder = url(drupal_get_path('module', 'dlts_viewer') . '/images/loadhold.png' , array('absolute' => TRUE ));

    // iterate through the results
    foreach ($cursor as $result) {

      $result['sequence'] = min($result['sequence'] ) ;

      $result['url'] = url('books/' . $identifier . '/' . $result['sequence'] , array('absolute' => TRUE ));

      if (isset($result['cm']['uri'] )) {
        $result['cm']['uri'] = file_create_url($result['cm']['uri'] ) ;
      }

      $result['imageServeURI'] = $image_server_service . '/resolver?url_ver=Z39.88-2004&rft_id=' . $result['cm']['uri'] . '&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/jpeg&svc.level=1&svc.rotate=0';

      if (isset($result['ocr']['uri'] )) {
        $result['ocr']['uri'] = file_create_url($result['ocr']['uri'] ) ;
      }

      $items[] = $result ;

    }

    // Initialise the pager
    $current_page = pager_default_initialize($count , $per_page);

    // Pager
    $pager = theme('pager', array('quantity', $count ));

    return theme('dlts_book_thumbnails' ,  array('items' => $items , 'placeholder' => $placeholder , 'pager' => $pager ) ) ;

  }

  else {
    drupal_not_found() ;
  }

}

function dlts_viewer_full_book_view(&$entity) {

  $js_common_config = array('type' => 'file', 'scope' => 'footer');

  $css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);

  drupal_add_css(drupal_get_path('module', 'dlts_viewer') . '/css/dlts_viewer.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));

  /** Add YUI Library from YUI Open CDN; should we add this as a setting in the theme form? */
  drupal_add_js('http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js', 'external', array('group' => JS_LIBRARY, 'weight' => -100));

  drupal_add_js(drupal_get_path('module', 'dlts_viewer') . '/js/crossframe.js', $js_common_config);

  drupal_add_js(drupal_get_path('module', 'dlts_viewer') . '/js/ui.crossframe.js', $js_common_config);

  drupal_add_js(drupal_get_path('module', 'dlts_viewer') . '/js/modules/slider-base.js', $js_common_config);

  // drupal_add_js(drupal_get_path('module', 'dlts_viewer') . '/js/ui.keyboard.yui.js', $js_common_config);

  drupal_add_js(drupal_get_path('module', 'dlts_viewer') . '/js/ui.components.yui.js', $js_common_config);

  return node_view($entity , 'metadata', $entity->language);

}

function dlts_viewer_pjax_book_view(&$entity) {
  $vars = array(
    'fid' => $entity->book_page_data['fid'],
    'identifier' => $entity->identifier,
    'title' => $entity->title,
    'sequence' => $entity->sequence,
    'sequence_count' => $entity->sequence_count,
    'uri' => $entity->book_page_data['uri'],
    'width' => $entity->book_page_data['djakota_width'],
    'height' => $entity->book_page_data['djakota_height'],
    'levels' => $entity->book_page_data['djakota_levels'],
    'dwtLevels' => $entity->book_page_data['djakota_dwtLevels'],
    'compositingLayerCount' => $entity->book_page_data['djakota_compositingLayerCount'],
    'button_previous' => $entity->button_previous,
    'button_next' => $entity->button_next,
    'button_togglepage' => $entity->button_togglepage,
  );
  return theme('book_page', $vars);
}

function dlts_viewer_dispatcher($entity , $arg) {

  $is_pjax = dlts_viewer_is_pjax();

  $query_parameters = drupal_get_query_parameters();

  /** We assume single page view */
  $is_single_page = TRUE;

  $thumbnails_rows_per_page = 10;

  if (isset($query_parameters['rows'])) {
    $thumbnails_rows_per_page = filter_xss($query_parameters['rows']);
  }

  /** get the first page of the requested sequence */
  $page = min($arg['sequence']);

  $sequence_max = max($arg['sequence']);

  $sequence_min = min($arg['sequence']);

  /** check if this is a double page view */
  if (count($arg['sequence'] ) > 1) {
    $is_single_page = FALSE ;
  }

  /** do we really want this as the page title? */
  $book_title = t('@title page num. @page' , array('@title' => $entity->title , '@page' => $page));

  drupal_set_title($book_title);

  $book_url = url('node/' . $entity->nid , array('absolute' => TRUE));

  $book_wrapper = entity_metadata_wrapper('node', $entity);

  $title = $book_wrapper->field_title->value();

  $identifier = $book_wrapper->field_identifier->value();

  $sequence_count = $book_wrapper->field_sequence_count->value();

  $int_read_order = $book_wrapper->field_read_order->value();

  $entity->read_order =($int_read_order == 1 ) ? 'rtl' : 'ltr';

  $next_page = $sequence_max + 1;

  $previous_page = $sequence_min - 1;

  $previous_page_active = TRUE;

  $next_page_active = TRUE;

  if ($previous_page < 1) $previous_page = 1;

  if ($next_page > $sequence_count) $next_page = $sequence_count;

  if ($next_page >= $sequence_count) $next_page_active = FALSE;

  if ($sequence_max == 1) $previous_page_active = FALSE;

  if ($next_page >= $sequence_count ) $next_page_active = FALSE;

  /** next page */
  $button_next = theme('dlts_viewer_pager_button' , array(
      'id' => 'next-page', // next-page, last-page, previous-page, first-page, toggle-page
      'active' => $next_page_active, // TRUE, FALSE
      'text' => t('Go to next page'), // string
      'url' => $book_url . '/' . $next_page . ((! $is_single_page ) ? '?page_view=double' : '' ),
      'operation' => 'next',
    )
  ) ;

  /** prev page */
  $button_previous = theme('dlts_viewer_pager_button' , array(
      'id' => 'previous-page', // next-page, last-page, previous-page, first-page, toggle-page
      'active' => $previous_page_active, // TRUE, FALSE
      'type' => 'dlts_book_page', // page-double, page-single
      'text' => t('Go to previous page'), // string
      'url' => $book_url . '/' . $previous_page . ((! $is_single_page ) ? '?page_view=double' : '' ),
      'operation' => 'previous',
    )
  ) ;

  /** Toggle between Single and Double page button */
  $button_togglepage = theme('dlts_viewer_pager_button' , array(
      'id' => 'toggle-page', // next-page, last-page, previous-page, first-page, toggle-page
      'active' => TRUE, // TRUE, FALSE
      'type' => 'dlts_book_page', // page-double, page-single ... I need this?
      'text' => t('Toggle between single or double page'),
      'url' => $book_url . '/' . $sequence_min . (($is_single_page ) ? '?page_view=double' : '' ),
      'operation' => ($is_single_page) ? ' page-double' : ' page-single' ,
    )
 );

  /** Metadata button */
  $button_metadata = theme('dlts_viewer_navbar_item', array(
      'title' => t('Metadata'),
      'path' => 'node/' . $entity->nid,
      'attributes' => array(
        'data-title' => t('Metadata'),
        'title' => t('Show/hide metadata'),
        'class' => array('button', 'metadata', 'on', ),
        'id' => array('button-metadata', )
      ),
      'fragment' => 'metadata',
    )
  ) ;

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
  ) ;

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
  $entity->book_page_sequence_number = $page ;

  $entity->navbar = theme('dlts_book_navbar' , array(
    // must fix this ...
    'nav_buttons_left' => array($button_metadata , '<li class="navbar-item">' . $button_togglepage . '</li>' , $button_thumbnails, ),
    'nav_buttons_right' => array($button_fullscreen, ),
    'nav_buttons_arrows' => array($button_previous, $button_next),
     /** Zoom in and out buttons */
    'control_panel' => theme('dlts_book_controlpanel'),
  ));

  $entity->url = $book_url;

  $entity->thumbnailsURL = $book_url . '/thumbnails';

  $entity->thumbnailsRows = $thumbnails_rows_per_page;

  $entity->identifier = $identifier;

  /** Book sequence count */
  $entity->sequence_count = $sequence_count;

  $entity->sequence = $page;

  $entity->thumbnailsPage = ceil(($page / $thumbnails_rows_per_page ) ) - 1 ;

  $entity->pane_metadata_hidden = FALSE;

  $entity->loading = theme('dlts_book_loading', array('sequence_number' => $page ) ) ;

  /** YUI! 3 Slider container */
  $entity->slider = theme('dlts_book_slider' , array('slider_id' => 'slider', 'uri' => $book_url, 'lang_dir' => $entity->read_order, 'sequence_number' => $page, 'sequence_count' => $sequence_count ) ) ;

  $entity->book_page = theme('dlts_viewer_openlayers_image' , array('file' => $arg ) ) ;

  $entity->book_title = $book_title ;

  $entity->button_metadata = $button_metadata  ;

  $entity->button_togglepage = $button_togglepage;

  $entity->button_previous = $button_previous  ;

  $entity->book_page_data = $arg ;

  $entity->button_next = $button_next  ;

  $entity->button_previous = $button_previous ;

  $entity->button_next = $button_next ;

  if (! $is_pjax ) return dlts_viewer_full_book_view($entity ) ;

  else return dlts_viewer_pjax_book_view($entity ) ;

}

/**
 * Route callback for books edit page
 * http://{site}/books/{identifier}/edit
 */
function _dlts_viewer_book_edit_redirect($node) {

  if (is_object($node )) {

    $wrapper = entity_metadata_wrapper('node', $node ) ;

    $identifier = $wrapper->field_identifier->value() ;

    if ($identifier) {
      $path = drupal_lookup_path('source', 'books/' . $identifier ) ;
    }
  }

  if ($path ) drupal_goto($path . '/edit' ) ;

  drupal_not_found() ;

}

function dlts_viewer_book_metadata_display($node) {
  return node_view($node , 'metadata' , $book->language ) ;
}
