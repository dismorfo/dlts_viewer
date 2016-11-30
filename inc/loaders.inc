<?php

/**
 * Include wildcard loaders
 */
foreach (file_scan_directory(drupal_get_path('module', 'dlts_viewer') . '/inc/loaders', '/.*\.inc$/') as $absolute => $file) {
  include_once $absolute;
}
