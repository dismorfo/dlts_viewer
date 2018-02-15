<?php

/**
 * Views (module)
 *
 * https://www.drupal.org/project/views
 * https://api.drupal.org/api/views/views.api.php
 */
/**
 * https://api.drupal.org/api/views/views.api.php/function/hook_views_pre_render/7.x-3.x
 */
function dlts_viewer_views_pre_render(&$view) {
  if ($view->name == 'mv') {
    $results = $view->result;
    foreach ($results as $key => $result) {
      if (isset($result->_field_data['field_multivol_node_nid']['entity'])) {
        $node = $result->_field_data['field_multivol_node_nid']['entity'];
        $wrapper = entity_metadata_wrapper('node', $node);
        $number = $wrapper->field_volume_number_str->value();
        $markup = $result->field_field_book_1[0]['rendered']['#markup'];
        if (!strpos($markup, $number) !== false) {
          $result->field_field_book_1[0]['rendered']['#markup'] = $markup . ' ' . $number;
        }
      }
    }
  }
}
