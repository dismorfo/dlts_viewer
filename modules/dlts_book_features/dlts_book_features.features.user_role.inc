<?php
/**
 * @file
 * dlts_book_features.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function dlts_book_features_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
  );

  // Exported role: collection curator.
  $roles['collection curator'] = array(
    'name' => 'collection curator',
    'weight' => 3,
  );

  return $roles;
}
