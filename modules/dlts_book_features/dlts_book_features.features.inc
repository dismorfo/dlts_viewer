<?php
/**
 * @file
 * dlts_book_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dlts_book_features_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dlts_book_features_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function dlts_book_features_node_info() {
  $items = array(
    'dlts_book' => array(
      'name' => t('Book'),
      'base' => 'node_content',
      'description' => t('Anything derived from either a bound set of pages designed to be read (books, pamphlets, etc.), or an electronic format such as ePub or PDF that uses virtual sections or pages, or both. Examples include: AWDL; NYUP; Morgan Claypool'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_book_section' => array(
      'name' => t('Book Section'),
      'base' => 'node_content',
      'description' => t('A non-overlapping subset of pages of a book. '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('This content type is not intended to carry information about rhetorical sections of a book - preface, etc. It is only intended to hold data derived from \'post-pagination\' sources such as ePub. A book can have pages and sections but only if it is derived from diverse materials that follow both models. A book derived from scanning or from dismemberment of paginated PDFs only will not have sections.

In early NYUP documentation, this was called \'_chapter\' - section is replacing \'_chapter\' because it is more generic. '),
    ),
    'dlts_collection' => array(
      'name' => t('Collection'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_map' => array(
      'name' => t('Map'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_multivol' => array(
      'name' => t('Multi-Volume'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_multivol_book' => array(
      'name' => t('Multi-Volume Book'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_partner' => array(
      'name' => t('Partner'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_series' => array(
      'name' => t('Series'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dlts_series_book' => array(
      'name' => t('Series Book'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
