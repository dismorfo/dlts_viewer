<?php

/**
 * @file
 * Provides fileserver:// streamwrapper.
 */

/**
 * Stream wrapper class to handle fileserver:// streams.
 *
 * A "stream" is an important Unix concept for the reading and writing of
 * files and other devices. Reading or writing a "stream" just means that you
 * open some device, file, internet site, or whatever, and you don't have to
 * know at all what it is. All the functions that deal with it are the same.
 * You can read/write more from/to the stream, seek a position in the stream,
 * or anything else without the code that does it even knowing what kind
 * of device it is talking to. This Unix idea is extended into PHP's
 * mindset.
 *
 * The idea of "stream wrapper" is that this can be extended indefinitely.
 * The classic example is HTTP: With PHP you can do a
 * file_get_contents("http://drupal.org/projects") as if it were a file,
 * because the scheme "http" is supported natively in PHP. So Drupal adds
 * the public:// and private:// schemes, and contrib modules can add any
 * scheme they want to. This module adds the fileserver:// scheme, which allows
 * reading(later on writting) files from the FileServer.
 *
 * @ingroup dlts_viewer
 */
class FileFilesServerStreamWrapper extends DrupalPublicStreamWrapper {
	/**
	 * Return FileServer URL
	 */
	function getFileServerUrl() {
    $base_url = $GLOBALS['base_url'] . '/' . variable_get('file_public_path', conf_path() . '/files');
    return variable_get('dlts_viewer_fileserver_host', $base_url);
  }
  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a public file.
   */
  function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    return self::getFileServerUrl() . '/' . drupal_encode_path($path);
  }
}
