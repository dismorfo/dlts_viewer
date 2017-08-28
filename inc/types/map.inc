<?php
/**
 * @file
 * Books
 */
// function dlts_viewer_process_node_dlts_book(&$vars) {}
/**
 * Implements "a version" template_preprocess_node().
 * https://api.drupal.org/api/drupal/modules%21node%21node.module/function/template_preprocess_node/7
 * THIS is not a default Drupal hook
 */
function dlts_viewer_preprocess_node_dlts_map(&$vars) {
    /** We don't render a DLTS Book object, redirect to a "Book page" */
    if ($vars['view_mode'] == 'full') {
        $wrapper = entity_metadata_wrapper('node', $vars['node']);
        $identifier = $wrapper->field_identifier->value();
        drupal_goto('maps/' . $identifier . '/1');
    }
    $vars['language_dir'] = dlts_viewer_viewer_language($vars);
    if ($vars['view_mode'] == 'metadata') {
        $args = arg();
        $count = count($args);

        if ($count === 3 && $args[2] == 'display') {

            $vars['theme_hook_suggestions'][] = 'node__display';
        }

        if (dlts_viewer_is_pjax()) {

            $vars['theme_hook_suggestions'][] = 'page__viewer_pjax';
        }

    }
}

function dlts_viewer_full_map_view(&$entity) {
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

function dlts_viewer_map_metadata_view($entity) {
    $element = node_view($entity, 'metadata');
    return drupal_render($element);
}

function dlts_viewer_pjax_map_view(&$entity) {
    $vars = array(
        'button_next' => $entity->button_next,
        'button_previous' => $entity->button_previous,
        'compositingLayerCount' => $entity->book_page_data['compositingLayerCount'],
        'dwtLevels' => $entity->book_page_data['dwtLevels'],
        'fid' => $entity->book_page_data['fid'],
        'height' => $entity->book_page_data['height'],
        'identifier' => $entity->identifier,
        'levels' => $entity->book_page_data['levels'],
        'sequence' => $entity->sequence,
        'sequence_count' => $entity->sequence_count,
        'title' => $entity->title,
        'uri' => $entity->book_page_data['uri'],
        'width' => $entity->book_page_data['width'],
        'pageView' => $entity->pageView,
    );
    return theme('viewer_page', $vars);
}
function map_language_field(&$entity) {
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
function dlts_viewer_map_dispatcher($entity, $arg) {
    // check request type
    $is_pjax = dlts_viewer_is_pjax();
    // get query parameters
    $query_parameters = drupal_get_query_parameters();

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
    map_language_field($entity);
    // check for type of request: 1) book page or 2) display pane
    if (!is_array($arg) && $arg == 'display') {
        // type of request
        $display_type = 'map_metadata';
        if ($is_pjax) {
            $display_type = 'map_metadata_pjax';
        }
    }
    else {
        // type of reque
        $display_type = 'map_page';
        if ($is_pjax) {
            $display_type = 'map_page_pjax';
        }

        $is_single_page = TRUE;

        $sequence = 1;
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
        $sequence_count = 1;
        $int_read_order = 0;
        $entity->read_order = ($int_read_order == 1) ? 'rtl' : 'ltr';
        $next_page = 1;
        $previous_page =  1;
        $previous_page_active = FALSE;
        $next_page_active = FALSE;

        /** next page */
        $next_pager_side = ($entity->read_order === 'ltr') ? 'pager-right' : 'pager-left';
        /** prev page */
        $previous_pager_side = ($entity->read_order === 'ltr') ? 'pager-left' : 'pager-right';
        $button_previous_options = array(
            'id' => 'previous-page',
            'type' => 'dlts_book_page',
            'text' => t('Go to previous page'),
            'url' => $book_url . '/' . $previous_page . ((!$is_single_page) ? '?page_view=double' : ''),
            'classes' => array('previous', $previous_pager_side, ($previous_page_active ? 'active' : 'inactive'))
        );
        $button_next_options = array(
            'id' => 'next-page',
            'text' => t('Go to next page'),
            'url' => $book_url . '/' . $next_page . ((!$is_single_page) ? '?page_view=double' : ''),
            'classes' => array('next', $next_pager_side, ($next_page_active ? 'active' : 'inactive'))
        );
        $button_metadata_options = array(
            'title' => t('Metadata'),
            'path' => 'node/' . $entity->nid,
            'attributes' => array(
                'data-title' => t('Metadata'),
                'title' => t('Show/hide metadata'),
                'class' => array('button', 'metadata', 'on'),
                'id' => array('button-metadata')
            ),
            'fragment' => 'metadata',
        );
        $button_fullscreen_options = array(
            'title' => t('Toggle fullscreen'),
            'path' => 'node/' . $entity->nid,
            'attributes' => array(
                'data-title' => t('Toggle fullscreen'),
                'title' => t('Toggle fullscreen'),
                'class' => array('button', 'fullscreen', 'off'),
                'id' => array('button-fullscreen')
            ),
            'fragment' => 'fullscreen',
        );
        $button_previous = theme('dlts_viewer_pager_button', $button_previous_options);
        $button_next = theme('dlts_viewer_pager_button', $button_next_options);
        /** fullscreen button */
        $button_fullscreen = theme('dlts_viewer_navbar_item', $button_fullscreen_options);
        /** Metadata button */
        $button_metadata = theme('dlts_viewer_navbar_item', $button_metadata_options);

        $controlpanel_button = theme('dlts_viewer_controlpanel');
        /** Book page sequence number */
        $entity->book_page_sequence_number = 1;
        /** Set navbar pager buttons according to read direction */
        $nav_pager_button_left_arrow = $button_previous;
        $nav_pager_button_right_arrow = $button_next;
        if ($entity->read_order == 'rtl') {
            $nav_pager_button_left_arrow = $button_next;
            $nav_pager_button_right_arrow = $button_previous;
        }
        $nav_pager_button_left_arrow = '<li class="navbar-item navbar-pager-left">' . $nav_pager_button_left_arrow . '</li>';
        $nav_pager_button_right_arrow = '<li class="navbar-item navbar-pager-right">' . $nav_pager_button_right_arrow . '</li>';

        $navbar_options = array(
            'nav_buttons_left' => array($button_metadata),
            'nav_buttons_right' => array($button_fullscreen),
            'nav_buttons_arrows' => array($button_previous, $button_next),
            'nav_buttons_pager_left_arrow' => $nav_pager_button_left_arrow,
            'nav_buttons_pager_right_arrow' => $nav_pager_button_right_arrow,
            'control_panel' => $controlpanel_button,
        );
        $loading_options = array('sequence_number' => $page);
        $entity->navbar = theme('dlts_viewer_navbar', $navbar_options);
        $entity->book_page_data = $arg;
        $entity->button_metadata = $button_metadata;
        $entity->button_next = $button_next;
        $entity->button_previous = $button_previous;
        $entity->loading = theme('dlts_viewer_loading', $loading_options);
        $entity->pane_metadata_hidden = FALSE;
        $entity->sequence = 1;
        $entity->sequence_count = 1;
        $slider_options = array(
            'slider_id' => 'slider',
            'uri' => $book_url,
            'lang_dir' => $entity->read_order,
            'sequence_number' => $page,
            'sequence_count' => $sequence_count
        );
        $entity->pageView = 'single';
        $arg['pageView'] = $entity->pageView;
        $book_page_options = array(
            'file' => $arg
        );
        $entity->book_page = theme('dlts_viewer_openlayers_image', $book_page_options);
        $entity->slider = theme('dlts_viewer_slider', $slider_options);
    }
    // types of returns
    switch ($display_type) {
        case 'map_page':
            return dlts_viewer_full_map_view($entity);
            break;
        case 'map_page_pjax':
            return dlts_viewer_pjax_map_view($entity);
            break;
        case 'map_metadata':
            return dlts_viewer_full_map_view($entity);
            break;
        case 'map_metadata_pjax':
            return dlts_viewer_map_metadata_view($entity);
            break;
    }
}
/**
 * Route callback for books edit page
 * http://{site}/books/{identifier}/edit
 */
function _dlts_viewer_map_edit_redirect($node) {
    if (is_object($node)) {
        $wrapper = entity_metadata_wrapper('node', $node);
        $identifier = $wrapper->field_identifier->value();
        if ($identifier) {
            $path = drupal_lookup_path('source', 'maps/' . $identifier);
            if ($path) {
                drupal_goto($path . '/edit');
            }
        }
    }
    drupal_not_found();
}
function dlts_viewer_map_metadata_display($node) {
    return node_view($node , 'metadata' , $node->language);
}
/**
 * Menu callback; prints the loaded structure of the current node/user.
 */
function dlts_viewer_load_map_object($type, $object, $name = NULL) {
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


    return $out;
}