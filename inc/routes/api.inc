<?php

function _dlts_viewer_menu_api() {

  $items = array();

  $items['sources/field/%'] = array(
  	'title' => 'Cotent types',
  	'access arguments' => array('access content'),
  	'page callback' => 'dlts_viewer_content_by_field',
  	'delivery callback' => 'dlts_viewer_deliver_json',
  	'page arguments' => array(2),
  );

  $items['content/%/nodeid'] = array(
    'title' => 'Return nid of node',
    'page callback' => 'return_nodeid_json',
    'page arguments' => array(1),
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
  );

  $items['content/%/nodeid/%'] = array(
    'title' => 'Return nid of node',
    'page callback' => 'return_nodeid_json',
    'page arguments' => array(1, 3),
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
  );

  $items['content/%/page_nodeids'] = array(
    'title' => 'Return page nodeids for book',
    'page callback' => 'return_page_nodeids_json',
    'page arguments' => array(1),
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
  );

  return $items;

}
