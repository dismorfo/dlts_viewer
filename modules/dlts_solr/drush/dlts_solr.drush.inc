<?php

/**
 * @file
 *   drush integration for apachesolr.
 */

/**
 * Implements hook_drush_command().
 *
 * @return array
 *   An associative array describing your command(s).
 */
function dlts_solr_drush_command() {
  $items = array();
  $items['ocr-mark-all'] = array(
    'callback' => 'dlts_solr_drush_ocr_mark_for_reindex',
    'description' => dt('Marks OCR content for reindexing. Can take identifier as parameter.'),
    'arguments' => array(
      'identifier' => dt('Optional. A space delimited list of identifiers to be marked for reindexing.'),
    ),
  );
  $items['ocr-index'] = array(
    'callback' => 'dlts_solr_drush_ocr_index',
    'description' => dt('Reindexes OCR content marked for (re)indexing.'),
    'options' => array(
      'limit' => 'The total number of documents to index',
    ),
  );
  $items['ocr-search'] = array(
    'callback' => 'dlts_solr_drush_ocr_search',
    'description' => dt('Search OCR for keywords using Apache Solr'),
    'arguments' => array(
      'keywords' => dt('One or more keywords, separated by spaces.'),
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param string $section
 *   A string with the help section (prepend with 'drush:')
 *
 * @return string
 *   A string with the help text for your command.
 */
function dlts_solr_drush_help($section) {
  switch ($section) {
    case 'drush:ocr-mark-all':
      return dt("Used without parameters, this command marks all of the content in the OCR MongoDB for
        reindexing. Used with parameters for identifier, it marks just the books that are specified.
        Reindexing is done on future cron runs.");
    case 'drush:ocr-index':
      return dt("Reindexes content marked for (re)indexing.");
    case 'drush:ocr-search':
      return dt('Executes a search against the site\'s Apache Solr search index and returns the results.');
  }
  return '';
}

/**
 * Mark all of a specific environment id for reindexing
 */
function dlts_solr_drush_ocr_mark_for_reindex() {

  return;

  // Apache Solr search index name
  // see: admin/config/search/apachesolr/settings/search_book_pages/edit
  $env_id = 'search_book_pages';  

  //module_load_include('inc', 'apachesolr', 'apachesolr.index');
  
  $args = func_get_args();
  
  apachesolr_index_mark_for_reindex($env_id);

  drush_print(t('Marked content for reindexing'));
}



function dlts_solr_index_batch_index_entities(&$context) {
  // Apache Solr search index name
  // see: admin/config/search/apachesolr/settings/search_book_pages/edit
  if (empty($context['sandbox'])) {
    try {
      dlts_solr_index_ocr();
    }
    catch (Exception $e) {
      watchdog('Apache Solr', $e->getMessage(), NULL, WATCHDOG_ERROR);
      return FALSE;
    }
    
    $documents_count = dlts_solr_index_status();
    
    $context['sandbox']['progress'] = 0;
    
    $context['sandbox']['submitted'] = 0;

    // How many items do we want to index? All or a limited set of items
    $context['sandbox']['max'] = $documents_count;
  }

  // We can safely process the apachesolr_cron_limit nodes at a time without a
  // timeout or out of memory error.
  $limit = 50;

  // Reduce the limit for our final batch if we would be processing more than had been requested
  if ($limit + $context['sandbox']['progress'] > $context['sandbox']['max']) {
    $limit = $context['sandbox']['max'] - $context['sandbox']['progress'];
  }

  if ($context['sandbox']['max'] >= $context['sandbox']['progress'] + $limit) {
    $context['sandbox']['progress'] += $limit;
  }
  else {
    $context['sandbox']['progress'] = $context['sandbox']['max'];
  }
  
  $arguments = array(
    '@current' => $context['sandbox']['progress'], 
    '@total' => $context['sandbox']['max'], 
  );

  $context['message'] = t('Inspected @current of @total entities.', $arguments);

  // Inform the batch engine that we are not finished, and provide an
  // estimation of the completion level we reached.
  $context['finished'] = empty($context['sandbox']['max']) ? 1 : $context['sandbox']['progress'] / $context['sandbox']['max'];

  // Put the total into the results section when we're finished so we can
  // show it to the admin.
  if ($context['finished']) {
    $context['results']['count'] = $context['sandbox']['progress'];
  }
}

function dlts_solr_index_batch_index_finished($success, array $results, array $operations) {
  $message = '';
  // $results['count'] will not be set if Solr is unavailable.
  if (isset($results['count'])) {
    $message .= format_plural($results['count'], '1 item processed successfully. ', '@count items successfully processed. ');
  }
  $message .= dt('Documents successfully sent to Solr. @count documents successfully sent to Solr.', array('@count'=>$results['count']));

  if ($success) {
    $type = 'status';
  }
  
  else {
    // An error occurred. $operations contains the unprocessed operations.
    $error_operation = reset($operations);
    $message .= ' ' . t('An error occurred while processing @num with arguments: @args', array('@num' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE)));
    $type = 'error';
  }
  
  drupal_set_message($message, $type);
}

function dlts_solr_index_batch_index_remaining() {
  $batch = array(
    'operations' => array(array('dlts_solr_index_batch_index_entities', array())), 
    'finished' => 'dlts_solr_index_batch_index_finished', 
    'title' => t('Indexing OCR content'), 
    'init_message' => t('Preparing to submit OCR content to Solr for indexing...'), 
    'progress_message' => t('Submitting OCR content to Solr...'), 
    'error_message' => t('Solr OCR indexing has encountered an error.'),
  );
  batch_set($batch);
}

/**
 * Index all the items in the queue using a batch command
 */
function dlts_solr_drush_ocr_index() {
  dlts_solr_index_batch_index_remaining();
  drush_backend_batch_process();
}

/**
 * Search the solr index using Drush
 */
function dlts_solr_drush_ocr_search() {

  drush_log(__FUNCTION__,'ok');
  return;
  
  $args = func_get_args();
  $keys = implode(' ', $args);
  
  foreach (apachesolr_search_search_execute($keys) as $result) {
    $output = $result['fields']['path'];
    if(isset($result['user']) && isset($result['node']->is_uid)) {
      $output .= ' ' . dt('by @name (user/@uid)', array('@name' => strip_tags($result['user']), '@uid' => $result['node']->is_uid));
    }
    $output .= "\n";
    $output .= dt('title: ') . $result['title'] . "\n";
    $output .= trim(preg_replace('/\s+/', ' ', strip_tags($result['snippet']))) . "\n\n";
    drush_print($output);
  }
  
}
