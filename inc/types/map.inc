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
    drupal_add_js($module_path . '/js/ui.keyboard.yui.js', $js_common_config);
    drupal_add_js($module_path . '/js/ui.map.components.yui.js', $js_common_config);
    drupal_add_js(variable_get('dlts_viewer_openlayers_source', 'sites/all/libraries/openlayers/build/OpenLayers.js'), array('group' => JS_LIBRARY));
    return node_view($entity , 'metadata', $entity->language);
}

function dlts_viewer_map_metadata_view($entity) {
    $element = node_view($entity, 'metadata');
    return drupal_render($element);
}

function dlts_viewer_pjax_map_view(&$entity) {
    $vars = array(
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

function dlts_viewer_map_dispatcher($entity, $arg) {
    // check request type
    $is_pjax = dlts_viewer_is_pjax();

    drupal_set_title($entity->title);
    // wrapp map entity with Entity API wrapper
    $book_wrapper = entity_metadata_wrapper('node', $entity);
    // map identifier
    $identifier = $book_wrapper->field_identifier->value();
    // set $identifier so that .tpl.php can use it
    $entity->identifier = $identifier;
    //  URL
    $book_url = url('node/' . $entity->nid , array('absolute' => TRUE));
    // set $url so that .tpl.php can use it
    $entity->url = $book_url;
    // check for type of request: 1) map page or 2) display pane
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

        /** fullscreen button */
        $button_fullscreen = theme('dlts_viewer_navbar_item', $button_fullscreen_options);
        /** Metadata button */
        $button_metadata = theme('dlts_viewer_navbar_item', $button_metadata_options);

        $controlpanel_button = theme('dlts_viewer_controlpanel');
        /** map page sequence number */
        $entity->book_page_sequence_number = 1;
        $navbar_options = array(
            'nav_buttons_left' => array($button_metadata),
            'nav_buttons_right' => array($button_fullscreen),
            'control_panel' => $controlpanel_button,
        );
        $loading_options = array('sequence_number' => $page);
        $entity->navbar = theme('dlts_viewer_navbar', $navbar_options);
        $entity->book_page_data = $arg;
        $entity->button_metadata = $button_metadata;
        $entity->loading = theme('dlts_viewer_loading', $loading_options);
        $entity->pane_metadata_hidden = FALSE;
        $entity->sequence = 1;
        $entity->sequence_count = 1;
        $entity->pageView = 'single';
        $arg['pageView'] = $entity->pageView;
        $book_page_options = array(
            'file' => $arg
        );
        $entity->book_page = theme('dlts_viewer_openlayers_image', $book_page_options);
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
