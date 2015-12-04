<?php

/**
 * @file
 * DLTS Viewer
 */

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @see drush_parse_command()
 *   for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function dlts_viewer_drush_command() {
  $items = array();
  $items['import-book'] = array(
    'callback' => 'dlts_viewer_import_book',
    'description' => dt('Import book from a JSON document.'),
    'options' => array(
      'source' => dt('JSON document filepath.'),
    ),
 );
  $items['import-dummy-books'] = array(
    'callback' => 'dlts_viewer_dummy_books',
    'description' => dt('Import dummy book.'),
 );
  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section(prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function dlts_viewer_drush_help($section) {
  switch ($section) {
    case 'drush:import-book':
      return dt("Some day someone will document this module");
      break;
    case 'drush:import-dummy-books':
      return dt("Some day someone will document this module");
      break;
  }
}

function dlts_viewer_import_book() {
  include_once(__DIR__ . '/inc/common.inc');
  include_once(__DIR__ . '/inc/datasource.inc');
  include_once(__DIR__ . '/inc/create.inc');
  $source = drush_get_option('source', NULL);
  $source = array( (object) array('uri' => $source));
  create_dlts_book_from_json($source);
}

function dlts_viewer_dummy_books() {
  drush_log('Drush Import book from JSON document' , 'status');
  include_once(__DIR__ . '/inc/common.inc');
  include_once(__DIR__ . '/inc/datasource.inc');
  include_once(__DIR__ . '/inc/create.inc');
  $data = file_scan_directory(__DIR__ . '/data/', '/(.*)\.json$/', array('recurse' => FALSE));
  create_dlts_book_from_json($data);
}
