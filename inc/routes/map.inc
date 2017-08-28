<?php
function _dlts_viewer_menu_map() {
    $menu = array(

        /**
         * Convinient way to edit a Book object
         * e.g., http://{{HOST}}/{{SITE}}/books/{{IDENTIFIER}}/edit
         */
        'maps/%dlts_map/edit' => array(
            'title' => 'Edit book',
            'access arguments' => array('edit any dlts_book content'),
            'page callback' => '_dlts_viewer_map_edit_redirect',
            'page arguments' => array(1),
            'type' => MENU_CALLBACK,
        ),

        'maps/%dlts_map/display' => array(
            'title' => 'Book metadata display',
            'access arguments' => array('access content'),
            'page callback' => 'dlts_viewer_map_dispatcher',
            'page arguments' => array(1, 2),
            'delivery callback' => 'drupal_deliver_html_page',
            'type' => MENU_CALLBACK,
        ),

        /**
         * Render book pages
         */
        'maps/%dlts_map/%dlts_map_page' => array(
            'title' => 'Book page',
            'access arguments' => array('access content'),
            'page callback' => 'dlts_viewer_map_dispatcher',
            'page arguments' => array(1 , 2),
            'delivery callback' => 'drupal_deliver_html_page',
            'type' => MENU_CALLBACK,
        ),
    );

    return $menu;
}
