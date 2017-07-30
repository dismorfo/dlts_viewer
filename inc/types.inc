<?php
/**
 * Include types
 */
foreach (file_scan_directory(drupal_get_path('module', 'dlts_viewer') . '/inc/types', '/.*\.inc$/') as $absolute => $file) {
    include_once $absolute;
}