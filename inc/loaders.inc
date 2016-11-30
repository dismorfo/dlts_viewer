<?php

/**
 * Include wildcard loaders
 */
foreach (file_scan_directory(DLTS_VIEWER_MODULE . '/inc/loaders', '/.*\.inc$/') as $absolute => $file) {
  include_once $absolute;
}
