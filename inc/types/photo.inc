<?php
/**
 * @file
 * photo.inc
 */
function dlts_viewer_full_photo_view(&$entity) {
    $module_path = drupal_get_path('module', 'dlts_viewer');
    $js_common_config = array('type' => 'file', 'scope' => 'footer');
    $css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);
    drupal_add_css($module_path . '/css/dlts_viewer.css', $css_common_config);
    drupal_add_js('http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js', 'external', array('group' => JS_LIBRARY, 'weight' => -100));
    drupal_add_js($module_path . '/js/crossframe.js', $js_common_config);
    drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);
    drupal_add_js($module_path . '/js/ui.components.yui.js', $js_common_config);
    drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY));
    $data_attributes = array(
        'uri' => $entity->args['uri'],
        'pageView' => 'single',
        'sequenceCount' => $entity->sequence_count,
        'sequence' => $entity->sequence,
        'title' => $entity->title,
        'width' => $entity->args['width'],
        'height' => $entity->args['height'],
        'levels' => $entity->args['levels'],
        'dwtLevels' => $entity->args['dwtLevels'],
        'compositingLayerCount' => $entity->args['compositingLayerCount'],
    );
    $vars = array(
        'compositingLayerCount' => $entity->args['compositingLayerCount'],
        'dwtLevels' => $entity->args['dwtLevels'],
        'fid' => $entity->args['identifier'],
        'height' => $entity->args['height'],
        'identifier' => $entity->identifier,
        'levels' => $entity->args['levels'],
        'title' => $entity->title,
        'uri' => $entity->args['uri'],
        'width' => $entity->args['width'],
        'pageView' => 'single',
        'data_attributes' => $data_attributes,
    );
    $viewer = array(
        //'slider' => $entity->slider,
        'book_page' => $entity->map,
        'map' => theme('viewer_map', $vars),
        'language_dir' => $entity->lang_dir,
        'pagemeta' => NULL,
        'navbar' => $entity->navbar,
        'url' => $entity->url,
        'identifier' => $entity->identifier,
        'title' => $entity->title,
        'loading' => $entity->loading,
    );
    return theme('viewer', $viewer);
}
function dlts_viewer_pjax_photo_view(&$entity) {
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
        'title' => $entity->title,
        'uri' => $entity->book_page_data['uri'],
        'width' => $entity->book_page_data['width'],
        'pageView' => $entity->pageView,
    );
    return theme('viewer_page', $vars);
}

function dlts_viewer_photo_set_view(&$entity)
{
    $module_path = drupal_get_path('module', 'dlts_viewer');
    $js_common_config = array('type' => 'file', 'scope' => 'footer');
    $css_common_config = array('group' => CSS_DEFAULT, 'every_page' => TRUE);
    drupal_add_css($module_path . '/css/dlts_viewer.css', $css_common_config);
    drupal_add_js('http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js', 'external', array('group' => JS_LIBRARY, 'weight' => -100));
    drupal_add_js($module_path . '/js/crossframe.js', $js_common_config);
    drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);


    drupal_set_title($entity->title);

        // count of thumbnails
        $count = $entity->count;
        // Items
        //$items = $entity->items;
        $rows = $entity->rows;
        // Initialise the pager
        $current_page = pager_default_initialize($count, $rows.length);
        // Pager
        $pager = theme('thumbnails_pager', array('quantity' => $count));

        $output = theme('dlts_viewer_set', array(
                'pager' => $pager,
                'current_page' => $current_page,
                'rows'=> $rows
            )
        );
        return $output;
}

function dlts_viewer_photo_dispatcher($entity,$arg) {

    // check request type
    $is_pjax = dlts_viewer_is_pjax();
    // get query parameters
    $query_parameters = drupal_get_query_parameters();

    drupal_set_title($entity->title);
    // wrapp book entity with Entity API wrapper
    $set_wrapper = entity_metadata_wrapper('node', $entity);
    // book identifier
    $identifier = $set_wrapper->field_identifier->value();
    // set $identifier so that .tpl.php can use it
    $entity->identifier = $identifier;
    // book URL
    $set_url = url('photos/' . $entity->identifier , array('absolute' => TRUE));
    // set $url so that .tpl.php can use it
    $entity->url = $set_url;

    // check for type of request: 1) book page or 2) display pane
    if (!is_array($arg)) { //&& $arg == 'set') {

        /** Load helpers */
        module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
        // check request type

        $page = $query_parameters["page"];
        if(!isset($page)) {
            $page=0;
        }
        // default number of rows per page
        $rows_per_page=2;
        // default amount of items to request
        $per_page = 12;
        // request thumbnails from MondoDB
        $thumbnails = dlts_viewer_mongodb_load_thumbnails($identifier, $page, $per_page,'dlts_photo','photos');
        // no need to continue if request does not have items.
        if ($thumbnails) {
            // count of thumbnails
            $count = $thumbnails['count'];
            // Items
            $items = $thumbnails['items'];
            //slice items into rows
            $rows =array_chunk($items,$rows_per_page);

            $entity->count=$count;
            $entity->rows=$rows;
        }
        $display_type = 'photo_set';


    }
    else {
        // type of request
        $display_type = 'photo_page';
        if ($is_pjax) {
            $display_type = 'photo_page_pjax';
        }

        $thumbnails_rows_per_page = 1;
        $sequence = $arg['sequence'];
        if (isset($query_parameters['rows'])) {
            $thumbnails_rows_per_page = filter_xss($query_parameters['rows']);
        }
        if (is_array($sequence)) {
            /** get the first page of the requested sequence */
            $page = min($sequence);
            $sequence_max = max($sequence);
            $sequence_min = min($sequence);

        }
        else {
            /** get the first page of the requested sequence */
            $page = $sequence_max = $sequence_min = $sequence;

        }
        $sequence_count = $set_wrapper->field_sequence_count->value();

        $entity->read_order = 'ltr';
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
        $next_pager_side =  'pager-right';
        /** prev page */
        $previous_pager_side =  'pager-left';
        $button_previous_options = array(
            'id' => 'previous-page',
            'type' => 'dlts_photo',
            'text' => t('Go to previous page'),
            'url' => $set_url . '/' . $previous_page,
            'classes' => array('previous', $previous_pager_side, ($previous_page_active ? 'active' : 'inactive'))
        );
        $button_next_options = array(
            'id' => 'next-page',
            'type' => 'dlts_photo',
            'text' => t('Go to next page'),
            'url' => $set_url . '/' . $next_page ,
            'classes' => array('next', $next_pager_side, ($next_page_active ? 'active' : 'inactive'))
        );
        $button_fullscreen_options = array(
            'title' => t('Toggle fullscreen'),
            'path' => 'photos/' . $entity->identifier.'/' .$page,
            'attributes' => array(
                'data-title' => t('Toggle fullscreen'),
                'title' => t('Toggle fullscreen'),
                'class' => array('button', 'fullscreen', 'off'),
                'id' => array('button-fullscreen')
            ),
            'fragment' => 'fullscreen',
        );
        $button_thumbnails_options = array(
            'title' => t('Pages'),
            'path' => 'photos/' . $entity->identifier.'/set',
            'attributes' => array(
                'data-title' => t('Select page'),
                'title' => t('Select page'),
                'class' => array('button', 'thumbnails', 'off'),
                'id' => array('button-thumbnails')
            ),
            'fragment' => 'thumbnails',
        );
        $button_previous = theme('dlts_viewer_pager_button', $button_previous_options);
        $button_next = theme('dlts_viewer_pager_button', $button_next_options);
        /** fullscreen button */
        $button_fullscreen = theme('dlts_viewer_navbar_item', $button_fullscreen_options);
        /** thumbnails button */
        $button_thumbnails = theme('dlts_viewer_navbar_item', $button_thumbnails_options);

        $controlpanel_button = theme('dlts_viewer_controlpanel');
        /** Book page sequence number */
        $entity->sequence = $page;
        /** Set navbar pager buttons according to read direction */
        $nav_pager_button_left_arrow = $button_previous;
        $nav_pager_button_right_arrow = $button_next;

        $nav_pager_button_left_arrow = '<li class="navbar-item navbar-pager-left">' . $nav_pager_button_left_arrow . '</li>';
        $nav_pager_button_right_arrow = '<li class="navbar-item navbar-pager-right">' . $nav_pager_button_right_arrow . '</li>';
        $navbar_options = array(
            'nav_buttons_left' => array( $button_thumbnails),
            'nav_buttons_right' => array($button_fullscreen),
            'nav_buttons_arrows' => array($button_previous, $button_next),
            'nav_buttons_pager_left_arrow' => $nav_pager_button_left_arrow,
            'nav_buttons_pager_right_arrow' => $nav_pager_button_right_arrow,
            'control_panel' => $controlpanel_button
        );
        $loading_options = array('sequence_number' => $page);
        $entity->navbar = theme('dlts_viewer_navbar', $navbar_options);
        $entity->book_page_data = $arg;
        $entity->button_next = $button_next;
        $entity->button_previous = $button_previous;
        $entity->loading = theme('dlts_viewer_loading', $loading_options);
        $entity->sequence = $page;
        $entity->sequence_count = $sequence_count;
        $slider_options = array(
            'slider_id' => 'slider',
            'uri' => $set_url,
            'lang_dir' => $entity->read_order,
            'sequence_number' => $page,
            'sequence_count' => $sequence_count
        );
        $entity->thumbnailsPage = ceil($page / $thumbnails_rows_per_page) - 1;
        $entity->thumbnailsRows = $thumbnails_rows_per_page;
        $entity->thumbnailsURL = $set_url . '/set';
        $entity->pageView =  'single';
        $arg['pageView'] = $entity->pageView;
        $arg['thumbnailsUrl'] = $entity->thumbnailsURL;
        $arg['thumbnailsRows'] = $entity->thumbnailsRows;
        $arg['thumbnailsPage'] = $entity->thumbnailsPage;
        $book_page_options = array(
            'file' => $arg
        );
        $entity->map = theme('dlts_viewer_openlayers_image', $book_page_options);
        $entity->slider = theme('dlts_viewer_slider', $slider_options);
    }
    // types of returns
    switch ($display_type) {
        case 'photo_page':
            return dlts_viewer_full_photo_view($entity);
            break;
        case 'photo_page_pjax':
            return dlts_viewer_pjax_photo_view($entity);
            break;
        case 'photo_set':
            return dlts_viewer_photo_set_view($entity);
            break;
    }
}

function dlts_viewer_dispatch_photo_thumbnails($entity) {
    /** Load helpers */
    module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
    // check request type
    $is_pjax = dlts_viewer_is_pjax();
    // get query parameters
    $query_parameters = drupal_get_query_parameters();

    drupal_set_title($entity->title);
    // wrapp book entity with Entity API wrapper
    $set_wrapper = entity_metadata_wrapper('node', $entity);
    // book identifier
    $identifier = $set_wrapper->field_identifier->value();
    // set $identifier so that .tpl.php can use it
    $entity->identifier = $identifier;
    // book URL
    $set_url = url('photos/' . $entity->identifier , array('absolute' => TRUE));
    // set $url so that .tpl.php can use it
    $entity->url = $set_url;
    /** hold query arguments */

    // start page
    $page = 0;
    // default amount of items to request
    $per_page = 4;

    // request thumbnails from MondoDB
    $thumbnails = dlts_viewer_mongodb_load_thumbnails($identifier, $page, $per_page,'dlts_photo','photos');
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
                //'sequence' => 4,
                'placeholder' => $placeholder,
                'current_page' => $current_page,
            )
        );
        return $output;
    }
    else {
        drupal_exit();
    }
}

function dlts_viewer_preprocess_html_photos(&$vars) {
    dlts_viewer_preprocess_html_photo($vars);
}
function dlts_viewer_preprocess_html_photo(&$vars) {
    $vars['classes_array'][] = 'photo';
}