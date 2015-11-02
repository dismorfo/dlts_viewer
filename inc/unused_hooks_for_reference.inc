<?php

/**
 * Implements hook_init().
 */
// function dlts_book_api_init ( ) {}

/**
 * Implements hook_entity_view().
 * http://www.drupalcontrib.org/api/drupal/contributions!entity!entity.module/function/entity_view/7
 */
// function dlts_book_api_entity_view ( $entity, $type, $view_mode, $langcode ) { }

/**
 * List of templates we overwrite. See hook_theme for more information of the templates.
 */

/**
 * Implements template_preprocess_block
 * https://api.drupal.org/api/drupal/modules!block!block.module/function/template_preprocess_block/7
 */
// function dlts_book_api_preprocess_block ( &$vars ) { }

/**
 * Implements template_preprocess_region
 * https://api.drupal.org/api/drupal/includes%21theme.inc/function/template_preprocess_region/7
 */
// function dlts_book_api_preprocess_region ( &$vars ) { }


/**
 'books/%dlts_book/metadata' => array(
 'title' => 'Book metadata',
 'access arguments' => array('access content'),
 'page callback' => 'dlts_book_api_book_metadata_display',
 'page arguments' => array(1),
 'delivery callback' => 'dlts_utilities_deliver_json',
 'type' => MENU_CALLBACK,
 'file' => 'inc/book.inc',
 ),
 */
/**
 'books/%dlts_book/display' => array (
 'title' => 'Book metadata',
 'access arguments' => array('access content'),
 'page callback' => 'dlts_book_api_book_metadata_display',
 'page arguments' => array(1),
 'delivery callback' => 'drupal_deliver_html_page',
 'type' => MENU_CALLBACK,
 ),
 */