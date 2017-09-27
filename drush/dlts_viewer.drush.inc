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
    $items['import-photo'] = array(
        'callback' => 'dlts_viewer_import_photo',
        'description' => dt('Import photo from JSON document.'),
        'options' => array(
            'source' => dt('JSON document filepath.'),
            'file' => dt('list of JSON files.'),
        ),
    );
    $items['import-map'] = array(
        'callback' => 'dlts_viewer_import_map',
        'description' => dt('Import map from a JSON document.'),
        'options' => array(
            'source' => dt('JSON document filepath.'),
        ),
    );
  $items['import-book'] = array(
        'callback' => 'dlts_viewer_import_book1',
        'description' => dt('Import book from a JSON document.'),
        'options' => array(
            'source' => dt('JSON document filepath.'),
            'pages' => dt('Create pages.'),
            'legacy' => dt('Create book using legacy JSON documents.'),
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
    $items['import-add-book-queue'] = array(
        'callback' => 'dlts_viewer_add_json_documents_queue',
        'description' => dt('Something nice will be here'),
        'options' => array(
            'source' => dt('Book source'),
            'legacy' => dt('Create book using legacy JSON documents.'),
        ),
    );
    $items['import-free-queue'] = array(
        'callback' => 'dlts_viewer_free_json_documents_queue',
        'description' => dt('Import book from a git repository'),
        'options' => array(
            'pages' => dt('Create pages.'),
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
  }
}

function dlts_viewer_import_photo($source = NULL) {
    include_once(__DIR__ . '/inc/common.inc');
    include_once(__DIR__ . '/inc/datasource.inc');
    include_once(__DIR__ . '/inc/create.inc');
    if (!$source) {
        $source = drush_get_option('source', NULL);
        if (!$source) {
            drush_log('No source to import ', 'error');
            return;
        }
    }
    $file = drush_get_option('file');
    if (!empty($file) && (!file_exists($file))) {
        drush_log("file option: $file is specified and must exist","error");
        return;
    }
    drush_print('we are in photo import');
    if (is_dir($source) && is_null($file)) {
        create_dlts_photo_set_from_json(file_scan_directory($source, '/(.*)\.json$/', array('recurse' => FALSE)));
    }
    elseif (is_dir($source) && (!empty($file))) {
       $handle = @fopen($file, "r") or exit("unable to open file ($file)"); 
       if ($handle) {
          while (($line = fgets($handle, 4096)) !== false) { 
            $fn = trim($line);
            $full_file = "$source/$fn";
            create_dlts_photo_set_from_json(array((object) array('uri' => $full_file)));
          }
      }
    }
    else {
        return create_dlts_photo_set_from_json(array((object) array('uri' => $source)));
    }
}


function dlts_viewer_import_map($source = NULL) {
    include_once(__DIR__ . '/inc/common.inc');
    include_once(__DIR__ . '/inc/datasource.inc');
    include_once(__DIR__ . '/inc/create.inc');
    if (!$source) {
        $source = drush_get_option('source', NULL);
        if (!$source) {
            drush_log('No source to import ', 'error');
            return;
        }
    }
    drush_log('we are here');
    if (is_dir($source)) {
        create_dlts_map_from_json(file_scan_directory($source, '/(.*)\.json$/', array('recurse' => FALSE)));
    }
    else {
        return create_dlts_map_from_json(array((object) array('uri' => $source)));
    }
}

function dlts_viewer_import_book($source = NULL, $create_pages = FALSE) {
  include_once(__DIR__ . '/inc/common.inc');
  include_once(__DIR__ . '/inc/datasource.inc');
  include_once(__DIR__ . '/inc/create.inc');
  if (drush_get_option('pages', FALSE)) $create_pages = TRUE;
  if (!$source) {
    $source = drush_get_option('source', NULL);
    if (!$source) {
    	drush_log('No source to import', 'error');
    	return;
    }
  }
  if (is_dir($source)) {
  	create_dlts_book_from_json(file_scan_directory($source, '/(.*)\.en.json$/', array('recurse' => FALSE)), $create_pages);
    // re-think
    create_dlts_book_from_json(file_scan_directory($source, '/(.*)\.ar.json$/', array('recurse' => FALSE)), $create_pages);
  }
  else {
  	return create_dlts_book_from_json(array((object) array('uri' => $source)), $create_pages);
  }
}

function dlts_viewer_json_documents($language = 'en', $limit = 100) {
  $query = db_select('dlts_viewer_books_json', 'b')
	  ->fields('b', array(
	    'document',
	    'legacy',
	    'identifier'
	  ))
	  ->condition('proceed', '0')
	  ->condition('language', $language)
    ->orderBy('created', 'DESC')
    ->range(0,$limit);
  return $query->execute()->fetchAll();
}

function dlts_viewer_free_json_documents_queue($limit = 100) {
  include_once(__DIR__ . '/inc/common.inc');
  include_once(__DIR__ . '/inc/datasource.inc');
  include_once(__DIR__ . '/inc/create.inc');
  $language = 'en';
  $results = dlts_viewer_json_documents($language, $limit);
  if (count($results) < 1) {
    $language = 'ar';
    $results = dlts_viewer_json_documents($language, $limit);
    if (count($results) < 1) return;
  }
	foreach ($results as $key => $result) {
  	if (file_exists($result->document)) {
      $created = create_dlts_book_from_json(array((object) array('uri' => $result->document)), FALSE, $result->legacy);
	    if (isset($created[0])) {
	  	  $node = $created[0];
	  	  if ($node->nid) {
  	      $fields = array(
  	        'nid' => $node->nid,
  		      'proceed' => 1,
  		      'created' =>  $node->created,
  		      'changed' => $node->changed
  	      );
	  		  db_update('dlts_viewer_books_json')->fields($fields)->condition('identifier', $result->identifier, '=')->condition('language', $language, '=')->execute();
	  	  }
	  	  else {
  	  	  db_update('dlts_viewer_books_json')->fields(array('proceed' => 2))->condition('identifier', $result->identifier, '=')->condition('language', $language, '=')->execute();
	  	  }
	    }
	    else {
        db_update('dlts_viewer_books_json')->fields(array('proceed' => 3))->condition('identifier', $result->identifier, '=')->condition('language', $language, '=')->execute();
	    }
	  }
	  else {
	    db_delete('dlts_viewer_books_json')->condition('identifier', $result->identifier)->condition('language', $language)->execute();
	  }
	}
}

function dlts_viewer_add_json_documents_queue() {
  $source = drush_get_option('source', NULL);
  $legacy = (drush_get_option('legacy', FALSE)) ? 1 : 0;
  if (is_dir($source)) {
    $uniqid = uniqid();
    $files = file_scan_directory($source, '/\.json/', array('recurse' => FALSE));
    foreach ($files as $file) {
      $json_string = file_get_contents($file->uri);
      if ($json_string) {
        $json_string = str_replace(array('\r', '\n'), '', $json_string);
        $data = json_decode($json_string);
        $language = 'en';
        $entity_language = (isset($data->entity_language) ? $data->entity_language : $language);
        /** force EN language */
        if ($entity_language == LANGUAGE_NONE) {
  	      $entity_language = 'en';
        }
  	    $fields = array(
  	      'nid' => 0,
  		    'hash' => $uniqid,
  		    'identifier' => $data->identifier,
  		    'document'=> $file->uri,
  		    'language' => $entity_language,
  		    'proceed' => 0,
  		    'created' => 0,
  		    'changed' => 0,
  		    'legacy' => $legacy,
  	    );
  		  db_insert('dlts_viewer_books_json')->fields($fields)->execute();
        unset($json_string);
        unset($data);
      }
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
