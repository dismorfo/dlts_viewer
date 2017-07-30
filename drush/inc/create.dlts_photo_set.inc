<?php
/**
 * Created by PhpStorm.
 * User: katepechekhonova
 * Date: 7/22/17
 * Time: 9:11 PM
 *//**
 * @file
 * DLTS Viewer drush
 */
function save_dlts_photo_set($node, $language = 'en') {

    // Get the current user
    // --user=uid OR --user=1 for super user
	global $user;

  $node_language = (isset($node['entity_language']) ? $node['entity_language'] : $language);

  /** force EN language */
  if ($node_language == LANGUAGE_NONE) {
      $node_language = 'en';
  }

  $photo_set_exist = book_nid($node['identifier'], $node_language);

  if ($photo_set_exist) {
      // Load the node by NID
      $entity = node_load($photo_set_exist);
      // Wrap it with Entity API
      $ewrapper = entity_metadata_wrapper('node', $entity);
  }
  else {
      // entity_create replaces the procedural steps in the first example of
      // creating a new object $node and setting its 'type' and uid property
      $values = array(
          'type' => 'dlts_photo_set',
          'uid' => $user->uid,
          'status' => 1,
          'comment' => 0,
          'promote' => 0,
      );
      if ($node_language == LANGUAGE_NONE) {
          $node_language = 'en';
      }

      $values['language'] = $node_language;
      $entity = entity_create('node', $values);
      // The entity is now created, but we have not yet simplified use of it.
      // Now create an entity_metadata_wrapper around the new node entity
      // to make getting and setting values easier
      $ewrapper = entity_metadata_wrapper('node', $entity);
  }
  $collections_nids = $subjects = array();
  if (!empty($node['entity_title'])) {
      $node['entity_title'] = trim($node['entity_title']);
      $substr_title = $node['entity_title'];
      if (strlen($substr_title) > 255) {
          $substr_title = substr($node['entity_title'], 0, 200);
      }
      $ewrapper->title->set($substr_title);
  }
  else {
      $ewrapper->title->set('Untitled');
  }
  if (!empty($node['identifier'])) {
      $ewrapper->field_identifier->set($node['identifier']);
  }
  else {
      drush_log('Document without identifier', 'error');
      return;
  }

    if (!empty($node['partners']) && is_array($node['partners']) && count($node['partners']) > 0) {
        foreach ($node['partners'] as $partner) {
            $partner_entity = NULL;
            $nid = partner_nid($partner['identifier']);
            if (!$nid) {
                $partner_entity = create_dlts_partner($partner);
                if ($partner_entity) {
                    $nid = $partner_entity->nid;
                }
            }
            if ($nid) {
                $partners_nids[] = $nid;
            }
        }
    }
    if (!empty($node['collections']) && is_array($node['collections']) && count($node['collections']) > 0) {
        foreach ($node['collections'] as $collection) {
            $collection_entity = NULL;
            $nid = collection_nid($collection['identifier']);
            // create collection
            if (!$nid) {
                $collection_entity = create_dlts_collection($collection);
                if ($collection_entity) {
                    $nid = $collection_entity->nid;
                }
            }
            if ($nid) {
                $collections_nids[] = $nid;
            }
        }
    }

      if (!empty($partners_nids)) {
          $ewrapper->field_partner->set($partners_nids);
      }
      if (!empty($collections_nids)) {
          $ewrapper->field_collection->set($collections_nids);
      }
      if (!empty($node['handle'])) {
          $ewrapper->field_handle->set(array('url' => $node['handle']));
      }

      if (!empty($node['page_count'])) {
          $ewrapper->field_page_count->set((int)$node['page_count']);
      }
      if (!empty($node['sequence_count'])) {
          $ewrapper->field_sequence_count->set((int)$node['sequence_count']);
      }

  if (!empty($node['title_long'])) {
      $ewrapper->field_title->set($node['title_long']);
  }
  elseif(!empty($node['entity_title'])) {
      $ewrapper->field_title->set($node['entity_title']);
  }

  $ewrapper->save();

  return $entity;

}

function create_dlts_photo_set_from_json($photo_sets, $cretePhotos = FALSE) {
    foreach ($photo_sets as $photo_set) {
        if (!file_exists($photo_set->uri)) continue;
        $json_string = file_get_contents($photo_set->uri);
        $json_string = str_replace(array('\r', '\n'), '', $json_string);
        $data = json_decode($json_string);
        drush_log(dt('Importing photo from JSON document @document',array('@document'=>basename ($photo_set->uri))), 'ok');

        /** Create photo_set Drupal node */

        $entity_title = $data->entity_title;

        $entity_identifier = $data->identifier;

        /** Define metadata fields */

        $partners=
        $collections=array();
        $title=
        $page_count=
        $sequence_count=null;

        drush_log(dt('hyi'));

        if (!isset($data->metadata)) {
            $metadata = (object) array();
        }
        else {
            $metadata = $data->metadata;
        }

        /** partners the book belong */
        if (isset($metadata->partner)) {
            foreach ($metadata->partner->value as $value) {
                $partners[] =  array(
                    'title' => $value->title,
                    'language' => $value->language,
                    'identifier' => $value->identifier,
                    'code' => $value->code,
                    'name' => $value->name,
                );
            }
        }
        /** collections the book belong */
        if (isset($metadata->collection)) {
            foreach ($metadata->collection->value as $value) {
                $collections[] =  array(
                    'title' => $value->title,
                    'language' => $value->language,
                    'identifier' => $value->identifier,
                    'code' => $value->code,
                    'name' => $value->name,
                    'partner' => $value->partner,
                );
            }
        }
        /** change title to string not array */
        if (isset($metadata->title)) {
            $title = $metadata->title->value[0];
        }

        if (isset($metadata->page_count) && !empty($metadata->page_count->value)) {
            $page_count = $metadata->page_count->value[0];
        }
        if (isset($metadata->sequence_count) && !empty($metadata->sequence_count->value)) {
            $sequence_count = $metadata->sequence_count->value[0];
        }
        if (isset($metadata->handle) && !empty($metadata->handle->value)) {
            $handle = $metadata->handle->value[0];
        }
        $node = array(
            'entity_title' => $entity_title,
            'identifier' => $entity_identifier,
            'partners' => $partners,
            'collections' => $collections,
            'title' => $title,
            'page_count' => $page_count,
            'sequence_count' => $sequence_count,
        );
         save_dlts_photo_set($node);

        /** Load MongoDB util */
        module_load_include('inc', 'dlts_viewer', 'inc/mongodb');
        dlts_viewer_mongodb_new_sequence($data, 'dlts_photo');
    }
}