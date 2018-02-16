<?php

/**
 * Implements "a version" template_preprocess_node().
 * https://api.drupal.org/api/drupal/modules%21node%21node.module/function/template_preprocess_node/7
 * THIS is not a default Drupal hook
 */
function dlts_viewer_preprocess_node_dlts_book(&$vars) {
  $mode = $vars['view_mode'];
  switch ($mode) {
    case 'full' :
      $wrapper = entity_metadata_wrapper('node', $vars['node']);
      drupal_goto('books/' . $wrapper->field_identifier->value() . '/1');
      break;
    case 'metadata' :
      $args = arg();
      $count = count($args);
      if ($count === 3 && $args[2] == 'display') {
        $vars['theme_hook_suggestions'][] = 'node__display';
      }
      if (dlts_viewer_is_pjax()) {
        $vars['theme_hook_suggestions'][] = 'node__viewer_pjax';
      }
      break;
  }
  $vars['language_dir'] = dlts_viewer_viewer_language($vars);
}

function dlts_viewer_dispatch_thumbnails($identifier) {
    /** Load helpers */
    module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
    /** hold query arguments */
    $query_parameters = drupal_get_query_parameters();
    $sequence = 1;
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
    $thumbnails = dlts_viewer_mongodb_load_thumbnails($identifier, $page, $per_page, "dlts_books_page","books");
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
      $output = theme('dlts_viewer_thumbnails', array(
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

function dlts_viewer_full_book_view(&$entity, $arg) {
  dlts_viewer_dispatcher_book_page($entity, $arg);
  return node_view($entity , 'metadata', $entity->language);
}

function dlts_viewer_book_metadata_view($entity) {
  return drupal_render(node_view($entity, 'metadata'));
}

function dlts_viewer_pjax_book_view(&$entity, $page) {
  dlts_viewer_dispatcher_book_page($entity, $page);
  return theme('viewer_page', 
    array(
      'next' => $entity->button_next,
      'previous' => $entity->button_previous,
      'toggle' => $entity->button_togglepage,
      'dragon' => $entity->book_page,
    )
  );
}

function _language_field(&$entity) {
  // wrapp book entity with Entity API wrapper
  $wrapper = entity_metadata_wrapper('node', $entity);
  $entity->identifier = $wrapper->field_identifier->value();
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

function dlts_viewer_dispatcher_book_page(&$entity, &$arg) {

  // check request type
  $is_pjax = dlts_viewer_is_pjax();

  // wrapp book entity with Entity API wrapper
  $wrapper = entity_metadata_wrapper('node', $entity);

  // get query parameters
  $query_parameters = drupal_get_query_parameters();

  // we assume single page view
  $is_single_page = (count($arg['records']) > 1) ? FALSE : TRUE;

  $entity->pageView = ($is_single_page) ? 'single' : 'double';  

  // book identifier
  // set $entity->identifier so that .tpl.php can use it
  $entity->identifier = $wrapper->field_identifier->value();

  // book URL
  // set $url so that .tpl.php can use it
  $entity->book_url = $entity->url = url('node/' . $entity->nid , array('absolute' => TRUE));

  $entity->pane_metadata_hidden = FALSE;  
    
  $thumbnails_rows_per_page = 10;

  $int_read_order = isset($wrapper->field_read_order) ? $wrapper->field_read_order->value() : 0;

  if (isset($query_parameters['rows'])) {
    $thumbnails_rows_per_page = filter_xss($query_parameters['rows']);
  }

  // IIIF rotation support
  $entity->rotation = FALSE;
  $entity->degrees = 0;
  
  // We check for "iiif-degrees"
  if (isset($query_parameters['iiif-degrees'])) {
    // rotate image
    $entity->rotation = TRUE;
    // degrees
    $entity->degrees = (int) filter_xss($query_parameters['iiif-degrees']);
  }

  if ($is_single_page) {
    $sequenceRecord = array_shift($arg['records']);
    /** get the first page of the requested sequence */
    $page = 
      $sequence_max = 
      $sequence_min = 
      $sequence = 
      $sequenceRecord['sequence'] = array_shift($sequenceRecord['sequence']);
      $entity->tiles = array($sequenceRecord['data']);
  }
  // view for &page_view=double
  else {
    $sequenceRecord = array_shift($arg['records']);
    $sequenceRecordMax = array_shift($arg['records']);
    /** get the first page of the requested sequence */
    $sequence_max = min($sequenceRecord['sequence']);
    $sequence_min = max($sequenceRecord['sequence']);
    $sequence = $page = array_shift($sequenceRecord['sequence']);
    $sequenceRecord['sequence'] = array_shift($sequenceRecord['sequence']);
    $entity->tiles = array_merge(
      array($sequenceRecord['data']), 
      array($sequenceRecordMax['data'])
    );
    unset($sequenceRecordMax);
  }

  $entity->sequence_count = $wrapper->field_sequence_count->value();
    
  $entity->read_order = ($int_read_order == 1) ? 'rtl' : 'ltr';

  /** book direction */
  $rtl = ($entity->read_order == 'rtl');
    
  $next_page = $sequence_max + 1;
    
  $previous_page = $sequence_min - 1;
    
  $previous_page_active = TRUE;
    
  $next_page_active = TRUE;
    
  if ($previous_page < 1) {
    $previous_page = 1;
  }
    
  if ($next_page > $entity->sequence_count) {
    $next_page = $entity->sequence_count;
    $next_page_active = FALSE;
  }
    
  if ($sequence_max == 1) {
    $previous_page_active = FALSE;
  }

  /** next page */
  $next_pager_side = ($entity->read_order === 'ltr') ? 'pager-right' : 'pager-left';

  /** prev page */
  $previous_pager_side = ($entity->read_order === 'ltr') ? 'pager-left' : 'pager-right';
  
  /** 
   * zoom out button 
   */
  $entity->button_zoomout = theme('dlts_viewer_navbar_item', array(
    'title' => t('Zoom out'),
    'path' => 'node/' . $entity->nid,
    'attributes' => array(
      'id' => array('control-zoom-out'),
      'title' => t('Zoom out'),
      'data-title' => t('Zoom out'),
      'class' => array('button', 'zoom-out'),
    ),
    'fragment' => 'zoomout',
    'events' => array(
      'emmit' => array('viewer-zoomout'),
    ),
  ));

  /** 
   * zoom in button 
   */
  $entity->button_zoomin = theme('dlts_viewer_navbar_item', array(
    'title' => t('Zoom in'),
    'path' => 'node/' . $entity->nid,
    'attributes' => array(
      'id' => array('control-zoom-in'),
      'title' => t('Zoom in'),
      'data-title' => t('Zoom in'),      
      'class' => array('button', 'zoom-in'),      
    ),
    'fragment' => 'zoomin',
    'events' => array(
      'emmit' => array('viewer-zoomin'),
    ),
  ));  

    /** 
   * rotate button 
   */
  $entity->button_rotate = theme('dlts_viewer_navbar_item', array(
    'title' => t('Rotate'),
    'path' => 'node/' . $entity->nid,
    'attributes' => array(
      'id' => array('control-rotate'),
      'title' => t('Rotate'),
      'data-title' => t('Rotate'),
      'class' => array('button', 'rotate'),      
    ),
    'fragment' => 'rotate',
    'events' => array(
      'emmit' => array('viewer-rotate'),
    ),
  )); 

  /** 
   * previous button 
   */
  $entity->button_previous = theme('dlts_viewer_pager_button', array(
    'attributes' => array(
      'id' => 'previous-page',
      'title' => t('Go to previous page'),
      'href' => $entity->book_url . '/' . $previous_page,
      'class' => array(
        'previous', 
        $previous_pager_side, 
        ($previous_page_active ? 'active' : 'inactive'),
        ($is_pjax ? 'pjax' : 'full-view'),
      ),
    ),
    'query' => array(
      'page_view' => (($is_single_page ) ? 'single' : 'double'),
    ),    
    'events' => array(
      'emmit' => array('viewer-previous'),
    ),
  ));

  /** 
   * next button 
   */
  $entity->button_next = theme('dlts_viewer_pager_button', array(
    'attributes' => array(
      'id' => 'next-page',
      'title' => t('Go to next page'),      
      'href' => $entity->book_url . '/' . $next_page,
      'class' => array(
        'next', 
        $next_pager_side, 
        ($next_page_active ? 'active' : 'inactive'),
        ($is_pjax ? 'pjax' : 'full-view'),
      ),
    ),
    'query' => array(
      'page_view' => (($is_single_page ) ? 'single' : 'double'),
    ),
    'events' => array(
      'emmit' => array('viewer-next'),
    ),
  ));  

  /** 
   * metadata button 
   */
  $entity->button_metadata = theme('dlts_viewer_navbar_item', array(
    'title' => t('Metadata'),
    'path' => 'node/' . $entity->nid,
    'attributes' => array(
      'id' => array('button-metadata'),
      'title' => t('Show/hide metadata'),
      'data-title' => t('Metadata'),      
      'class' => array('button', 'metadata', 'on'),      
    ),
    'fragment' => 'metadata',
    'events' => array(
      'emmit' => array('viewer-metadata'),
    ),
  ));

  /** 
   * toggle between Single and Double page button 
   */
  $entity->button_togglepage = theme('dlts_viewer_navbar_item', array(
    'title' => t('Toggle page'),
    'path' => $entity->book_url . '/' . $sequence_min,
    'attributes' => array(
      'id' => 'toggle-page',
      'title' => t('Toggle between single or double page'),
      'data-title' => t('Toggle page'),
      'class' => array('button', 'toggle', 'active', ($is_single_page) ? ' page-double' : ' page-single'),
    ),
    'query' => array(
      'page_view' => (($is_single_page ) ? 'double' : 'single'),
    ),    
    'fragment' => 'toggle',
    'events' => array(
      'emmit' => array('viewer-toggle'),
    ),
  ));  

  /** 
   * fullscreen button 
   */
  $entity->button_fullscreen = theme('dlts_viewer_navbar_item', array(
    'title' => t('Toggle fullscreen'),
    'path' => 'node/' . $entity->nid,
    'attributes' => array(
      'data-title' => t('Toggle fullscreen'),
      'title' => t('Toggle fullscreen'),
      'class' => array('button', 'fullscreen', 'off'),
      'id' => array('button-fullscreen')
    ),
    'fragment' => 'fullscreen',
    'events' => array(
      'emmit' => array('viewer-fullscreen'),
    ),    
  ));

  /** 
   * thumbnails button 
   */

  $entity->thumbnailsPage = ceil($page / $thumbnails_rows_per_page) - 1;

  $entity->thumbnailsRows = $thumbnails_rows_per_page;
   
  $entity->thumbnailsUrl = url('books/' . $entity->identifier . '/thumbnails', array(
      'absolute' => TRUE,
      'query' => array(
        'page' => $entity->thumbnailsPage,
        'rows' => $entity->thumbnailsRows,
        'sequence' => $page,
      ),
    )
  );

  $entity->button_thumbnails = theme('dlts_viewer_navbar_item', array(
    'title' => t('Pager'),
    'path' => $entity->thumbnailsUrl,
    'attributes' => array(
      'data-title' => t('Select page'),
      'data-state' => 'off',
      'title' => t('Select page'),
      'class' => array('button', 'thumbnails', 'off'),
      'id' => array('button-thumbnails')
    ),
    'events' => array(
      'emmit' => array('viewer-thumbnails'),
    ),
  ));
        
  /** Book page sequence number */
  $entity->book_page_sequence_number = $page;
    
  /** Set navbar pager buttons according to read direction */
  $nav_pager_button_left_arrow = '<li class="navbar-item navbar-pager-left">' . ($rtl ? $entity->button_next : $entity->button_previous) . '</li>';

  $nav_pager_button_right_arrow = '<li class="navbar-item navbar-pager-right">' . ($rtl ? $entity->button_previous : $entity->button_next) . '</li>';

  $entity->navbar = theme('dlts_viewer_navbar', 
    array(
      'nav_buttons_left' => array(
        $entity->button_metadata,
        $entity->button_thumbnails,
        $entity->button_togglepage
      ),
      'nav_buttons_right' => array(
        $entity->button_fullscreen
      ),
      'nav_buttons_arrows' => array(
        $entity->button_previous,
        $entity->button_next
      ),
      'nav_buttons_pager_left_arrow' => $nav_pager_button_left_arrow,
      'nav_buttons_pager_right_arrow' => $nav_pager_button_right_arrow,
      'control_panel' => array(
        $entity->button_zoomin,
        $entity->button_zoomout,
        $entity->button_rotate,
      ),
    )
  );

  $entity->book_page_data = $sequenceRecord;

  $entity->sequence = $page;

  $slider_options = array(
    'slider_id' => 'slider',
    'uri' => $entity->book_url,
    'lang_dir' => $entity->read_order,
    'sequence_number' => $page,
    'sequence_count' => $entity->sequence_count
  );

  $merge = array(
    'sequence' => $page,
    'title' => $entity->title,
    'tiles' => $entity->tiles,
    'pageView' => $entity->pageView,
    'thumbnailsUrl' => $entity->thumbnailsUrl,
    'thumbnailsRows' => $entity->thumbnailsRows,
    'thumbnailsPage' => $entity->thumbnailsPage,
    'identifier' => $entity->identifier,
    'sequenceCount' => $entity->sequence_count,
    'rotation' => $entity->rotation,
    'degrees' => $entity->degrees,  // @TODO: need to implement
  );

  $entity->book_page = theme('dlts_viewer_openlayers_image', array_merge($sequenceRecord, $merge));

  $entity->slider = theme('dlts_viewer_slider', $slider_options);

}

function dlts_viewer_book_title(&$entity) {
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
  drupal_set_title($entity->title);  
}

function dlts_viewer_dispatcher($entity, $arg) {
  // check request type
  $is_pjax = dlts_viewer_is_pjax();
  // set book title
  dlts_viewer_book_title($entity);
  // check for type of request: 1) book page or 2) display pane
  if (!is_array($arg) && $arg == 'display') {
    $type = (!$is_pjax) ? 'book_metadata' : 'book_metadata_pjax';
  }
  else {
    $type = (!$is_pjax) ? 'book_page' : 'book_page_pjax';
  }
  // set language
  _language_field($entity);
  // types of returns
  switch ($type) {
    case 'book_page' :
      return dlts_viewer_full_book_view($entity, $arg);
    case 'book_page_pjax' :
      return dlts_viewer_pjax_book_view($entity, $arg);
    case 'book_metadata' :
      return dlts_viewer_full_book_view($entity, $arg);
    case 'book_metadata_pjax' :
      return dlts_viewer_book_metadata_view($entity, $arg);
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
  return node_view($node , 'metadata' , $node->language);
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
  // remove duplication -- check ...
  $multivol_book = dlts_viewer_is_multivolume_book($object);

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