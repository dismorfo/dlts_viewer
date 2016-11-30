<?php

/**
 * Include types
 */
foreach (file_scan_directory(DLTS_VIEWER_MODULE . '/inc/types', '/.*\.inc$/') as $absolute => $file) {
  include_once $absolute;
}
