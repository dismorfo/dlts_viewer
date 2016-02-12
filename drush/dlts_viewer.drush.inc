<?php

/**
 * @file
 * DLTS Viewer
 */

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @see drush_parse_command()
 *   for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function dlts_viewer_drush_command() {  
  $items = array();  
  $items['import-book'] = array(
    'callback' => 'dlts_viewer_import_book',
    'description' => dt('Import book from a JSON document.'),
    'options' => array(
      'source' => dt('JSON document filepath.'),
      'pages' => dt('Create pages.'),
    ),
  );  
  $items['git-pull-books'] = array(
    'callback' => 'dlts_viewer_pull_books_from_git_repo',
  	'description' => dt('Import book from a git repository'),
  	'options' => array(
      'git-dir' => dt('Git repo directory path'),
      'git-worktree' => dt('Git repo directory  work tree'),
      'git-branch' => dt('Project branch'), // @TODO: not implemented
  	),
  );  
  $items['import-queue'] = array(
    'callback' => 'dlts_viewer_free_json_documents_queue',
    'description' => dt('Import book from a git repository'),
    'options' => array(  		
      'pages' => dt('Create pages.'),
    ),
  );  
  $items['import-dummy-books'] = array(
    'callback' => 'dlts_viewer_dummy_books',
    'description' => dt('Import dummy book.'),
 );  
  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section(prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function dlts_viewer_drush_help($section) {
  switch ($section) {
    case 'drush:import-book':
      return dt("Some day someone will document this module");
      break;
    case 'drush:import-dummy-books':
      return dt("Some day someone will document this module");
      break;
    case 'drush:git-pull-books':
      return dt("Some day someone will document this module");
      break;
    case 'drush:import-book-free-queue':
      return dt("Some day someone will document this module");
      break;
  }
}

function dlts_viewer_import_book($source = NULL) {
  include_once(__DIR__ . '/inc/common.inc');
  include_once(__DIR__ . '/inc/datasource.inc');
  include_once(__DIR__ . '/inc/create.inc');
  if ($source == NULL) {
    $source = drush_get_option('source', NULL);
    if (empty($source)) return;
  }  
  $pages = drush_get_option('pages', FALSE);  
  if ($pages) $create_pages = TRUE;  
  return create_dlts_book_from_json( array((object) array('uri' => $source)), $create_pages);
}

function dlts_viewer_dummy_books() {
  include_once(__DIR__ . '/inc/common.inc');
  include_once(__DIR__ . '/inc/datasource.inc');
  include_once(__DIR__ . '/inc/create.inc');
  drush_log( dt('Import books from JSON documents inside directory @directory', array('@directory'=> __DIR__ . '/data')) , 'status');
  watchdog('DRUSH', "Import books from JSON documents inside directory %directory", array('%directory' => __DIR__ . '/data'), WATCHDOG_INFO);
  $en_data = file_scan_directory(__DIR__ . '/data', '/(.*)\.en.json$/', array('recurse' => FALSE));
  create_dlts_book_from_json($en_data);
  $ar_data = file_scan_directory(__DIR__ . '/data', '/(.*)\.ar.json$/', array('recurse' => FALSE));
  create_dlts_book_from_json($ar_data);
}

function dlts_viewer_free_json_documents_queue() {
	$results = db_query("SELECT document FROM {dlts_viewer_books_json} WHERE proceed = :proceed AND language = :language GROUP BY document ORDER BY created DESC", array(':proceed' => 0, ':language' => 'en'));
	foreach ($results as $key => $result) {
		$nodes = dlts_viewer_import_book($result->document);
	  if (isset($nodes['success'][0])) {
	  	$node = $nodes['success'][0];
	  	if ($node->nid) {
	  		db_update('dlts_viewer_books_json')->fields(array('nid' => $node->nid,'proceed' => 1, 'created' => $node->timestamp))->condition('document', $result->document, '=')->execute();
	  	}
	  }
	  else {
	  	drush_log(dt('Error importing document @document.', array('@document'=> $nodes['error'][0])), 'error');
	  }
	}
	$results = db_query("SELECT document FROM {dlts_viewer_books_json} WHERE proceed = :proceed AND language = :language GROUP BY document ORDER BY created DESC", array(':proceed' => 0, ':language' => 'ar'));
	foreach ($results as $key => $result) {
		$nodes = dlts_viewer_import_book($result->document);
	  if (isset($nodes['success'][0])) {
	  	$node = $nodes['success'][0];
	  	if ($node->nid) {
	  		db_update('dlts_viewer_books_json')->fields(array('nid' => $node->nid,'proceed' => 1, 'created' => $node->timestamp))->condition('document', $result->document, '=')->execute();
	  	}
    }
    else {
      drush_log(dt('Error importing document @document.', array('@document'=> $nodes['error'][0])), 'error');
    }
  }
}

function dlts_viewer_add_json_documents_to_queue($files) {
  foreach ($files as $hash => $languages) {
    foreach ($languages as $language => $documents) {
      foreach ($documents as $document) {
      	if (file_exists($document)) {
          db_insert('dlts_viewer_books_json')->fields(array('nid' => null, 'hash' => $hash, 'document'=> $document, 'language' => $language, 'proceed' => 0, 'created' => time()))->execute();
      	}
      }
    }
  }
}

// drush git-pull-books --git-worktree=/Users/aortiz/tools/projects/dlts_viewer_distro --git-dir=/Users/aortiz/tools/projects/dlts_viewer_distro/.git --user=1

function dlts_viewer_pull_books_from_git_repo() {
  $pwd = getcwd();
  $result = array();
  $verbose = drush_get_option('verbose', NULL);
  $git_work_tree = drush_get_option('git-worktree', NULL);
  $git_dir = drush_get_option('git-dir', NULL);	
  $git_branch = drush_get_option('git-branch', 'master');
  if (!$git_dir || !$git_work_tree) {
    drush_log('Error','error');
    return;
  }
  chdir($git_work_tree);	
	// UNIX command
  if ($verbose) drush_log(dt('Inside @dir', array('@dir' => getcwd())), 'ok');
  $command = 'git log -n 10  --pretty=format:"%H"';
  exec($command, $result);
  foreach ($result as $commit) {
  	$c = $d = $files = array();
  	$result = db_query("SELECT * FROM {dlts_viewer_drush_git_commits} WHERE hash = :hash", array(':hash' =>$commit))->fetchField();
  	if (!$result) {
		  if ($verbose) drush_log(dt('Commit @commit', array('@commit'=>$commit)), 'ok');
		  $date = 'git show -s --format=%ci ' . $commit;
		  exec($date, $d);
		  $timestamp = strtotime($d[0]);
  		$comment = 'git show -s --format=%s ' . $commit;
  		exec($comment, $c);
  		$label = $c[0];
  		$command = 'git --git-dir=' . $git_dir . ' --work-tree=' . $git_work_tree . ' diff-tree --no-commit-id --name-only -r ' . $commit;
  		exec($command, $files);
  		$clean_files = array($commit => array('en' => array(), 'ar' => array()));
  		// figure out the language using the filename
  		foreach ($files as $file) {
  			if (strpos($file, '.json') !== false) { // "make sure" this is a JSON file
  			  if (strpos($file, '.ar.json') !== false) {
  				  $clean_files[$commit]['ar'][] = $git_work_tree . '/' . $file;
  			  }
  			  else {
  				  $clean_files[$commit]['en'][] = $git_work_tree . '/' . $file;
  			  }
  			}
  	  }
  	  dlts_viewer_add_json_documents_to_queue($clean_files);
  	  db_insert('dlts_viewer_drush_git_commits')->fields(array('hash' => $commit, 'label'=> $label, 'created' => $timestamp, 'proceed' => 0))->execute();
  	}
  }
  chdir($pwd);
}
