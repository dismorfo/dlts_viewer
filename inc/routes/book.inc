<?php
function _dlts_viewer_menu_book() {
    $menu = array(

        /**
         * Convinient way to edit a Book object
         * e.g., http://{{HOST}}/{{SITE}}/books/{{IDENTIFIER}}/edit
         */
        'books/%dlts_book/edit' => array(
            'title' => 'Edit book',
            'access arguments' => array('edit any dlts_book content'),
            'page callback' => '_dlts_viewer_book_edit_redirect',
            'page arguments' => array(1),
            'type' => MENU_CALLBACK,
        ),

        /**
         * Render thumbnails for book pages
         * No need to load the Book object % instead of %dlts_book
         * request accept ?page=INT(mimic Drupal's pager) and ?limit=INT
         */
        'books/%/thumbnails' => array(
            'title' => 'Book page',
            'access arguments' => array('access content'),
            'page callback' => 'dlts_viewer_dispatch_thumbnails',
            'page arguments' => array(1),
            'delivery callback' => 'drupal_deliver_html_page',
            'type' => MENU_CALLBACK,
        ),

        'books/%dlts_book/display' => array(
            'title' => 'Book metadata display',
            'access arguments' => array('access content'),
            'page callback' => 'dlts_viewer_dispatcher',
            'page arguments' => array(1, 2),
            'delivery callback' => 'drupal_deliver_html_page',
            'type' => MENU_CALLBACK,
        ),

        /**
         * Render book pages
         */
        'books/%dlts_book/%dlts_book_page' => array(
            'title' => 'Book page',
            'access arguments' => array('access content'),
            'page callback' => 'dlts_viewer_dispatcher',
            'page arguments' => array(1 , 2),
            'delivery callback' => 'drupal_deliver_html_page',
            'type' => MENU_CALLBACK,
        ),

        /**
         * JSON
         */
        'books.json' => array(
            'title' => 'All books in JSON format',
            'access arguments' => array('access content'),
            'page callback' => 'dlts_viewer_books',
            'delivery callback' => 'dlts_viewer_deliver_json',
            'file' => 'inc/endpoints.inc',
        ),
    );

    if (module_exists('devel')) {
        $menu['node/%node/json_view'] = array(
            'title' => 'JSON',
            'page callback' => 'dlts_viewer_load_object',
            'page arguments' => array('node', 1),
            'access arguments' => array('access content'),
            'type' => MENU_LOCAL_TASK,
            'weight' => 100,
        );
    }

    return $menu;
}