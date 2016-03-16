<?php
/**
 * @file
 * dlts_book_features.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dlts_book_features_taxonomy_default_vocabularies() {
  return array(
    'geographic_subject' => array(
      'name' => 'Geographic Subject',
      'machine_name' => 'geographic_subject',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 0,
    ),
    'subjects' => array(
      'name' => 'Subjects',
      'machine_name' => 'subjects',
      'description' => 'Use subject tags to group books.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 0,
    ),
    'tags' => array(
      'name' => 'Terms',
      'machine_name' => 'tags',
      'description' => 'Use tags to group articles on similar topics into categories.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 0,
    ),
  );
}
