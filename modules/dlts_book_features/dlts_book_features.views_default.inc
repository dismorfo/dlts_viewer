<?php
/**
 * @file
 * dlts_book_features.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function dlts_book_features_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'book';
  $view->description = 'All Pages for a book';
  $view->tag = 'dlts_books';
  $view->base_table = 'node';
  $view->human_name = 'Book';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Book';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '8';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_sequence_number']['id'] = 'field_sequence_number';
  $handler->display->display_options['fields']['field_sequence_number']['table'] = 'field_data_field_sequence_number';
  $handler->display->display_options['fields']['field_sequence_number']['field'] = 'field_sequence_number';
  $handler->display->display_options['fields']['field_sequence_number']['label'] = 'Page Number';
  $handler->display->display_options['fields']['field_sequence_number']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_sequence_number']['hide_alter_empty'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_page_type']['id'] = 'field_page_type';
  $handler->display->display_options['fields']['field_page_type']['table'] = 'field_data_field_page_type';
  $handler->display->display_options['fields']['field_page_type']['field'] = 'field_page_type';
  $handler->display->display_options['fields']['field_page_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_page_type']['hide_alter_empty'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_cropped_master']['id'] = 'field_cropped_master';
  $handler->display->display_options['fields']['field_cropped_master']['table'] = 'field_data_field_cropped_master';
  $handler->display->display_options['fields']['field_cropped_master']['field'] = 'field_cropped_master';
  $handler->display->display_options['fields']['field_cropped_master']['label'] = '';
  $handler->display->display_options['fields']['field_cropped_master']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_cropped_master']['hide_alter_empty'] = FALSE;
  /* Sort criterion: Broken/missing handler */
  $handler->display->display_options['sorts']['field_sequence_number_value']['id'] = 'field_sequence_number_value';
  $handler->display->display_options['sorts']['field_sequence_number_value']['table'] = 'field_data_field_sequence_number';
  $handler->display->display_options['sorts']['field_sequence_number_value']['field'] = 'field_sequence_number_value';
  /* Contextual filter: Content: Book (field_book) */
  $handler->display->display_options['arguments']['field_book_nid']['id'] = 'field_book_nid';
  $handler->display->display_options['arguments']['field_book_nid']['table'] = 'field_data_field_book';
  $handler->display->display_options['arguments']['field_book_nid']['field'] = 'field_book_nid';
  $handler->display->display_options['arguments']['field_book_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_book_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_book_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_book_nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_visible_value']['id'] = 'field_visible_value';
  $handler->display->display_options['filters']['field_visible_value']['table'] = 'field_data_field_visible';
  $handler->display->display_options['filters']['field_visible_value']['field'] = 'field_visible_value';
  $handler->display->display_options['filters']['field_visible_value']['value'] = array(
    1 => '1',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'book/%/pages/no';

  /* Display: Page - Not Normal */
  $handler = $view->new_display('page', 'Page - Not Normal', 'page_1');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_page_type_value']['id'] = 'field_page_type_value';
  $handler->display->display_options['filters']['field_page_type_value']['table'] = 'field_data_field_page_type';
  $handler->display->display_options['filters']['field_page_type_value']['field'] = 'field_page_type_value';
  $handler->display->display_options['filters']['field_page_type_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_page_type_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['path'] = 'books/%/non-normal';
  $translatables['book'] = array(
    t('Master'),
    t('Book'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Page Number'),
    t('Broken handler field_data_field_page_type.field_page_type'),
    t('All'),
    t('Page'),
    t('Page - Not Normal'),
  );
  $export['book'] = $view;

  $view = new view();
  $view->name = 'book_description';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Book Description';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Book Description';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'metadata';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Page Count */
  $handler->display->display_options['fields']['field_page_count']['id'] = 'field_page_count';
  $handler->display->display_options['fields']['field_page_count']['table'] = 'field_data_field_page_count';
  $handler->display->display_options['fields']['field_page_count']['field'] = 'field_page_count';
  $handler->display->display_options['fields']['field_page_count']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_page_count']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['field_language'] = 'ar';
  $translatables['book_description'] = array(
    t('Master'),
    t('Book Description'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Page Count'),
    t('All'),
    t('Block'),
  );
  $export['book_description'] = $view;

  $view = new view();
  $view->name = 'book_title_with_volnum';
  $view->description = 'For Metadata pane -- book title with volume number, if any';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Book title with volnum';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Book title with volnum';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'no title';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'no title found';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Content: Volume */
  $handler->display->display_options['fields']['field_volume']['id'] = 'field_volume';
  $handler->display->display_options['fields']['field_volume']['table'] = 'field_data_field_volume';
  $handler->display->display_options['fields']['field_volume']['field'] = 'field_volume';
  $handler->display->display_options['fields']['field_volume']['label'] = '';
  $handler->display->display_options['fields']['field_volume']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_volume']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_volume']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_volume']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_volume']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_volume']['type'] = 'text_plain';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[title] Vol. [field_volume]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Identifier (field_identifier) */
  $handler->display->display_options['arguments']['field_identifier_value']['id'] = 'field_identifier_value';
  $handler->display->display_options['arguments']['field_identifier_value']['table'] = 'field_data_field_identifier';
  $handler->display->display_options['arguments']['field_identifier_value']['field'] = 'field_identifier_value';
  $handler->display->display_options['arguments']['field_identifier_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_identifier_value']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_identifier_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_identifier_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_identifier_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_identifier_value']['limit'] = '0';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dlts_book' => 'dlts_book',
  );
  /* Filter criterion: Content: Language (field_language) */
  $handler->display->display_options['filters']['field_language_value']['id'] = 'field_language_value';
  $handler->display->display_options['filters']['field_language_value']['table'] = 'field_data_field_language';
  $handler->display->display_options['filters']['field_language_value']['field'] = 'field_language_value';
  $handler->display->display_options['filters']['field_language_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_language_value']['value'] = 'Eng';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['book_title_with_volnum'] = array(
    t('Master'),
    t('Book title with volnum'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('no title'),
    t('no title found'),
    t('[title] Vol. [field_volume]'),
    t('All'),
    t('Block'),
  );
  $export['book_title_with_volnum'] = $view;

  $view = new view();
  $view->name = 'books';
  $view->description = 'Book listing with optional argument(IA identifier)';
  $view->tag = 'dlts_books';
  $view->base_table = 'node';
  $view->human_name = 'Books';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All Books';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    2 => '2',
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '16';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '8';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Book (field_book) */
  $handler->display->display_options['relationships']['field_book_nid']['id'] = 'field_book_nid';
  $handler->display->display_options['relationships']['field_book_nid']['table'] = 'field_data_field_book';
  $handler->display->display_options['relationships']['field_book_nid']['field'] = 'field_book_nid';
  $handler->display->display_options['relationships']['field_book_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_book_nid']['delta'] = '-1';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['path']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_service_copy']['id'] = 'field_service_copy';
  $handler->display->display_options['fields']['field_service_copy']['table'] = 'field_data_field_service_copy';
  $handler->display->display_options['fields']['field_service_copy']['field'] = 'field_service_copy';
  $handler->display->display_options['fields']['field_service_copy']['label'] = '';
  $handler->display->display_options['fields']['field_service_copy']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_service_copy']['alter']['path'] = '[path]';
  $handler->display->display_options['fields']['field_service_copy']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['field_service_copy']['element_type'] = '0';
  $handler->display->display_options['fields']['field_service_copy']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_service_copy']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_service_copy']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_service_copy']['element_default_classes'] = FALSE;
  /* Sort criterion: Content: Date (field_publication_date) */
  $handler->display->display_options['sorts']['field_publication_date_value']['id'] = 'field_publication_date_value';
  $handler->display->display_options['sorts']['field_publication_date_value']['table'] = 'field_data_field_publication_date';
  $handler->display->display_options['sorts']['field_publication_date_value']['field'] = 'field_publication_date_value';
  $handler->display->display_options['sorts']['field_publication_date_value']['relationship'] = 'field_book_nid';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_sequence_number_value']['id'] = 'field_sequence_number_value';
  $handler->display->display_options['filters']['field_sequence_number_value']['table'] = 'field_data_field_sequence_number';
  $handler->display->display_options['filters']['field_sequence_number_value']['field'] = 'field_sequence_number_value';
  $handler->display->display_options['filters']['field_sequence_number_value']['value'] = array(
    'value' => '1',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'collection-items';
  $translatables['books'] = array(
    t('Master'),
    t('All Books'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('field_book'),
    t('Page'),
  );
  $export['books'] = $view;

  $view = new view();
  $view->name = 'books_by_collection';
  $view->description = 'Book listing with optional argument(IA identifier)';
  $view->tag = 'dlts_books';
  $view->base_table = 'node';
  $view->human_name = 'Books by collection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All Books by collection';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    2 => '2',
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'path' => 'path',
    'field_title' => 'field_title',
    'field_collection' => 'field_collection',
    'status' => 'status',
    'changed' => 'changed',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'path' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_collection' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['path']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Content: Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'node';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'custom';
  $handler->display->display_options['fields']['changed']['custom_date_format'] = 'm/d/Y';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  /* Sort criterion: Content: Date (field_publication_date) */
  $handler->display->display_options['sorts']['field_publication_date_value']['id'] = 'field_publication_date_value';
  $handler->display->display_options['sorts']['field_publication_date_value']['table'] = 'field_data_field_publication_date';
  $handler->display->display_options['sorts']['field_publication_date_value']['field'] = 'field_publication_date_value';
  $handler->display->display_options['sorts']['field_publication_date_value']['relationship'] = 'field_book_nid';
  /* Sort criterion: Content: Collection (field_collection) */
  $handler->display->display_options['sorts']['field_collection_nid']['id'] = 'field_collection_nid';
  $handler->display->display_options['sorts']['field_collection_nid']['table'] = 'field_data_field_collection';
  $handler->display->display_options['sorts']['field_collection_nid']['field'] = 'field_collection_nid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Post date';
  /* Contextual filter: Content: Collection (field_collection) */
  $handler->display->display_options['arguments']['field_collection_nid']['id'] = 'field_collection_nid';
  $handler->display->display_options['arguments']['field_collection_nid']['table'] = 'field_data_field_collection';
  $handler->display->display_options['arguments']['field_collection_nid']['field'] = 'field_collection_nid';
  $handler->display->display_options['arguments']['field_collection_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_collection_nid']['default_argument_options']['argument'] = 'All';
  $handler->display->display_options['arguments']['field_collection_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_collection_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_collection_nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dlts_book' => 'dlts_book',
  );
  /* Filter criterion: Content: Collection (field_collection) */
  $handler->display->display_options['filters']['field_collection_nid']['id'] = 'field_collection_nid';
  $handler->display->display_options['filters']['field_collection_nid']['table'] = 'field_data_field_collection';
  $handler->display->display_options['filters']['field_collection_nid']['field'] = 'field_collection_nid';
  $handler->display->display_options['filters']['field_collection_nid']['value'] = array(
    'all' => 'all',
    2 => '2',
  );
  $handler->display->display_options['filters']['field_collection_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_collection_nid']['expose']['operator_id'] = 'field_collection_nid_op';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['label'] = 'Collection';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['operator'] = 'field_collection_nid_op';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['identifier'] = 'field_collection_nid';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_collection_nid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    1 => 0,
  );
  /* Filter criterion: Content: Title (field_title) */
  $handler->display->display_options['filters']['field_title_value']['id'] = 'field_title_value';
  $handler->display->display_options['filters']['field_title_value']['table'] = 'field_data_field_title';
  $handler->display->display_options['filters']['field_title_value']['field'] = 'field_title_value';
  $handler->display->display_options['filters']['field_title_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_title_value']['expose']['operator_id'] = 'field_title_value_op';
  $handler->display->display_options['filters']['field_title_value']['expose']['label'] = 'Title(field_title)';
  $handler->display->display_options['filters']['field_title_value']['expose']['operator'] = 'field_title_value_op';
  $handler->display->display_options['filters']['field_title_value']['expose']['identifier'] = 'field_title_value';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Date (field_publication_date) */
  $handler->display->display_options['sorts']['field_publication_date_value']['id'] = 'field_publication_date_value';
  $handler->display->display_options['sorts']['field_publication_date_value']['table'] = 'field_data_field_publication_date';
  $handler->display->display_options['sorts']['field_publication_date_value']['field'] = 'field_publication_date_value';
  $handler->display->display_options['sorts']['field_publication_date_value']['relationship'] = 'field_book_nid';
  /* Sort criterion: Content: Collection (field_collection) */
  $handler->display->display_options['sorts']['field_collection_nid']['id'] = 'field_collection_nid';
  $handler->display->display_options['sorts']['field_collection_nid']['table'] = 'field_data_field_collection';
  $handler->display->display_options['sorts']['field_collection_nid']['field'] = 'field_collection_nid';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dlts_book' => 'dlts_book',
  );
  /* Filter criterion: Content: Collection (field_collection) */
  $handler->display->display_options['filters']['field_collection_nid']['id'] = 'field_collection_nid';
  $handler->display->display_options['filters']['field_collection_nid']['table'] = 'field_data_field_collection';
  $handler->display->display_options['filters']['field_collection_nid']['field'] = 'field_collection_nid';
  $handler->display->display_options['filters']['field_collection_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_collection_nid']['expose']['operator_id'] = 'field_collection_nid_op';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['label'] = 'Collection';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['operator'] = 'field_collection_nid_op';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['identifier'] = 'field_collection_nid';
  $handler->display->display_options['filters']['field_collection_nid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_collection_nid']['expose']['remember_roles'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    1 => 0,
  );
  /* Filter criterion: Content: Title (field_title) */
  $handler->display->display_options['filters']['field_title_value']['id'] = 'field_title_value';
  $handler->display->display_options['filters']['field_title_value']['table'] = 'field_data_field_title';
  $handler->display->display_options['filters']['field_title_value']['field'] = 'field_title_value';
  $handler->display->display_options['filters']['field_title_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_title_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_title_value']['expose']['operator_id'] = 'field_title_value_op';
  $handler->display->display_options['filters']['field_title_value']['expose']['label'] = 'Title contains word or phrase:';
  $handler->display->display_options['filters']['field_title_value']['expose']['operator'] = 'field_title_value_op';
  $handler->display->display_options['filters']['field_title_value']['expose']['identifier'] = 'field_title_value';
  $handler->display->display_options['filters']['field_title_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'admin/collection';
  $translatables['books_by_collection'] = array(
    t('Master'),
    t('All Books by collection'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Displaying @start - @end of @total'),
    t('Title'),
    t('Language'),
    t('Status'),
    t('Updated date'),
    t('Edit link'),
    t('Post date'),
    t('All'),
    t('Collection'),
    t('Title(field_title)'),
    t('Page'),
    t('Title contains word or phrase:'),
  );
  $export['books_by_collection'] = $view;

  $view = new view();
  $view->name = 'mv';
  $view->description = 'Renders a jump menu with links to the other volumes in this set';
  $view->tag = '2016';
  $view->base_table = 'node';
  $view->human_name = 'Multivolume select';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'MV';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'jump_menu';
  $handler->display->display_options['style_options']['hide'] = TRUE;
  $handler->display->display_options['style_options']['path'] = 'field_book';
  $handler->display->display_options['style_options']['choose'] = '- View Related Titles -';
  $handler->display->display_options['style_options']['inline'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Book (field_book) - reverse */
  $handler->display->display_options['relationships']['reverse_field_book_node']['id'] = 'reverse_field_book_node';
  $handler->display->display_options['relationships']['reverse_field_book_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_book_node']['field'] = 'reverse_field_book_node';
  $handler->display->display_options['relationships']['reverse_field_book_node']['required'] = TRUE;
  /* Relationship: Content: Multi-Volume (field_multivol) */
  $handler->display->display_options['relationships']['field_multivol_nid']['id'] = 'field_multivol_nid';
  $handler->display->display_options['relationships']['field_multivol_nid']['table'] = 'field_data_field_multivol';
  $handler->display->display_options['relationships']['field_multivol_nid']['field'] = 'field_multivol_nid';
  $handler->display->display_options['relationships']['field_multivol_nid']['relationship'] = 'reverse_field_book_node';
  $handler->display->display_options['relationships']['field_multivol_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_multivol_nid']['delta'] = '-1';
  /* Relationship: Content: Multi-Volume (field_multivol) - reverse */
  $handler->display->display_options['relationships']['reverse_field_multivol_node']['id'] = 'reverse_field_multivol_node';
  $handler->display->display_options['relationships']['reverse_field_multivol_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_multivol_node']['field'] = 'reverse_field_multivol_node';
  $handler->display->display_options['relationships']['reverse_field_multivol_node']['relationship'] = 'field_multivol_nid';
  $handler->display->display_options['relationships']['reverse_field_multivol_node']['required'] = TRUE;
  /* Field: Content: Volume Number */
  $handler->display->display_options['fields']['field_volume_number_1']['id'] = 'field_volume_number_1';
  $handler->display->display_options['fields']['field_volume_number_1']['table'] = 'field_data_field_volume_number';
  $handler->display->display_options['fields']['field_volume_number_1']['field'] = 'field_volume_number';
  $handler->display->display_options['fields']['field_volume_number_1']['relationship'] = 'reverse_field_multivol_node';
  $handler->display->display_options['fields']['field_volume_number_1']['label'] = '';
  $handler->display->display_options['fields']['field_volume_number_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_volume_number_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_volume_number_1']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Link only - for token */
  $handler->display->display_options['fields']['field_book']['id'] = 'field_book';
  $handler->display->display_options['fields']['field_book']['table'] = 'field_data_field_book';
  $handler->display->display_options['fields']['field_book']['field'] = 'field_book';
  $handler->display->display_options['fields']['field_book']['relationship'] = 'reverse_field_multivol_node';
  $handler->display->display_options['fields']['field_book']['ui_name'] = 'Link only - for token';
  $handler->display->display_options['fields']['field_book']['label'] = '';
  $handler->display->display_options['fields']['field_book']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_book']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_book']['type'] = 'node_reference_path';
  $handler->display->display_options['fields']['field_book']['settings'] = array(
    'alias' => 1,
    'absolute' => 0,
  );
  /* Field: Content: Book */
  $handler->display->display_options['fields']['field_book_1']['id'] = 'field_book_1';
  $handler->display->display_options['fields']['field_book_1']['table'] = 'field_data_field_book';
  $handler->display->display_options['fields']['field_book_1']['field'] = 'field_book';
  $handler->display->display_options['fields']['field_book_1']['relationship'] = 'reverse_field_multivol_node';
  $handler->display->display_options['fields']['field_book_1']['label'] = '';
  $handler->display->display_options['fields']['field_book_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_book_1']['alter']['text'] = '[field_book_1]  Vol. [field_volume_number_1]';
  $handler->display->display_options['fields']['field_book_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_book_1']['type'] = 'node_reference_plain';
  /* Sort criterion: Content: Volume Number (field_volume_number) */
  $handler->display->display_options['sorts']['field_volume_number_value']['id'] = 'field_volume_number_value';
  $handler->display->display_options['sorts']['field_volume_number_value']['table'] = 'field_data_field_volume_number';
  $handler->display->display_options['sorts']['field_volume_number_value']['field'] = 'field_volume_number_value';
  $handler->display->display_options['sorts']['field_volume_number_value']['relationship'] = 'reverse_field_multivol_node';
  /* Contextual filter: Content: Identifier (field_identifier) */
  $handler->display->display_options['arguments']['field_identifier_value']['id'] = 'field_identifier_value';
  $handler->display->display_options['arguments']['field_identifier_value']['table'] = 'field_data_field_identifier';
  $handler->display->display_options['arguments']['field_identifier_value']['field'] = 'field_identifier_value';
  $handler->display->display_options['arguments']['field_identifier_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_identifier_value']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_identifier_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_identifier_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_identifier_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_identifier_value']['limit'] = '0';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_description'] = 'Multivolume Select';
  $translatables['mv'] = array(
    t('Master'),
    t('MV'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('field_book'),
    t('field_multivol'),
    t('[field_book_1]  Vol. [field_volume_number_1]'),
    t('All'),
    t('Block'),
    t('Multivolume Select'),
  );
  $export['mv'] = $view;

  $view = new view();
  $view->name = 'series';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Series';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Series';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Identifier */
  $handler->display->display_options['fields']['field_identifier']['id'] = 'field_identifier';
  $handler->display->display_options['fields']['field_identifier']['table'] = 'field_data_field_identifier';
  $handler->display->display_options['fields']['field_identifier']['field'] = 'field_identifier';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Sort criterion: Content: Identifier (field_identifier) */
  $handler->display->display_options['sorts']['field_identifier_value']['id'] = 'field_identifier_value';
  $handler->display->display_options['sorts']['field_identifier_value']['table'] = 'field_data_field_identifier';
  $handler->display->display_options['sorts']['field_identifier_value']['field'] = 'field_identifier_value';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dlts_series' => 'dlts_series',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/series';
  $translatables['series'] = array(
    t('Master'),
    t('Series'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Identifier'),
    t('Nid'),
    t('Page'),
  );
  $export['series'] = $view;

  $view = new view();
  $view->name = 'series_books';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Series books';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Series';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '500';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_series' => 'field_series',
  );
  $handler->display->display_options['style_options']['default'] = 'field_series';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_series' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Series */
  $handler->display->display_options['fields']['field_series']['id'] = 'field_series';
  $handler->display->display_options['fields']['field_series']['table'] = 'field_data_field_series';
  $handler->display->display_options['fields']['field_series']['field'] = 'field_series';
  $handler->display->display_options['fields']['field_series']['type'] = 'node_reference_nid';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dlts_series_book' => 'dlts_series_book',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/series_pages';
  $translatables['series_books'] = array(
    t('Master'),
    t('Series'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Page'),
  );
  $export['series_books'] = $view;

  $view = new view();
  $view->name = 'series_for_a_book';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Series for a book';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Series for a book';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_series' => 'field_series',
    'field_volume_number_str' => 'field_volume_number_str',
  );
  $handler->display->display_options['row_options']['separator'] = ', ';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Relationship: Content: Book (field_book) - reverse */
  $handler->display->display_options['relationships']['reverse_field_book_node']['id'] = 'reverse_field_book_node';
  $handler->display->display_options['relationships']['reverse_field_book_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_book_node']['field'] = 'reverse_field_book_node';
  $handler->display->display_options['relationships']['reverse_field_book_node']['label'] = 'field_book_rev';
  $handler->display->display_options['relationships']['reverse_field_book_node']['required'] = TRUE;
  /* Field: Content: Series */
  $handler->display->display_options['fields']['field_series']['id'] = 'field_series';
  $handler->display->display_options['fields']['field_series']['table'] = 'field_data_field_series';
  $handler->display->display_options['fields']['field_series']['field'] = 'field_series';
  $handler->display->display_options['fields']['field_series']['relationship'] = 'reverse_field_book_node';
  $handler->display->display_options['fields']['field_series']['label'] = '';
  $handler->display->display_options['fields']['field_series']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_series']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_series']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_series']['empty'] = 'No series listed';
  $handler->display->display_options['fields']['field_series']['type'] = 'node_reference_plain';
  /* Field: Content: Volume Number String */
  $handler->display->display_options['fields']['field_volume_number_str']['id'] = 'field_volume_number_str';
  $handler->display->display_options['fields']['field_volume_number_str']['table'] = 'field_data_field_volume_number_str';
  $handler->display->display_options['fields']['field_volume_number_str']['field'] = 'field_volume_number_str';
  $handler->display->display_options['fields']['field_volume_number_str']['relationship'] = 'reverse_field_book_node';
  $handler->display->display_options['fields']['field_volume_number_str']['label'] = '';
  $handler->display->display_options['fields']['field_volume_number_str']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_volume_number_str']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_volume_number_str']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_volume_number_str']['hide_empty'] = TRUE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Identifier (field_identifier) */
  $handler->display->display_options['arguments']['field_identifier_value']['id'] = 'field_identifier_value';
  $handler->display->display_options['arguments']['field_identifier_value']['table'] = 'field_data_field_identifier';
  $handler->display->display_options['arguments']['field_identifier_value']['field'] = 'field_identifier_value';
  $handler->display->display_options['arguments']['field_identifier_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_identifier_value']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_identifier_value']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['field_identifier_value']['default_argument_options']['use_alias'] = TRUE;
  $handler->display->display_options['arguments']['field_identifier_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_identifier_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_identifier_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_identifier_value']['limit'] = '0';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'reverse_field_book_node';
  $handler->display->display_options['filters']['type']['value'] = array(
    'dlts_series_book' => 'dlts_series_book',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['series_for_a_book'] = array(
    t('Master'),
    t('Series for a book'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('field_book_rev'),
    t('No series listed'),
    t('All'),
    t('Block'),
  );
  $export['series_for_a_book'] = $view;

  return $export;
}
