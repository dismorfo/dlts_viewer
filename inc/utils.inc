<?php

function dlts_viewer_content_by_field($field_name) {
  $out = array();
  $field_name = filter_xss($field_name);
  $info = field_info_field($field_name);
  $query_parameters = drupal_get_query_parameters();
  if ($info) {
    $m = array_keys($info['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
    $table = $m[0];
    $c = array_keys( $info['columns'] );
    $columns = $c[0];
    if ($columns == 'fid') {
      return array();
    }
    $seek = $field_name . '_' . $columns;
    $table = 'field_data_' . $field_name;
    $query = 'SELECT DISTINCT ' . $seek . ' AS value FROM {' . $table . '}';
    $result = db_query($query);
    foreach ($result as $record) {
      if ( $columns == 'nid' ) {
        $node = node_load( $record->value );
        $raw_value = $record->value;
        $value = $node->title;
      }
      elseif ( $columns == 'tid' ) {
        $term = taxonomy_term_load( $record->value );
        $raw_value = $record->value;
        $value = $term->name;
      }
      else {
        $raw_value = $value = $record->value;
      }
      $out[] = array(
        'value' => $value,
        'raw_value' => $raw_value,
      );
    }
  }
  return $out;
}
/**
 * Package and send page callback as JSON/JSONP.
 * See: http://api.drupal.org/api/drupal/includes--common.inc/function/drupal_deliver_html_page/7
 *
 * @param $page_callback_result
 * A keyed array to be converted into JSON
 */
function dlts_viewer_deliver_json($page_callback_result) {
    /** Send appropriate HTTP-Header for browsers and search engines. */
    global $language;
    drupal_add_http_header('Content-Language', $language->language);
    /** Menu status constants are integers; page content is a string or array. */
    if (is_int($page_callback_result)) {
        switch ($page_callback_result) {
            // 404 response
            case MENU_NOT_FOUND:
                drupal_add_http_header('Status', 'HTTP/1.1 404 Not Found');
                watchdog('Request not found', check_plain($_GET['q']), NULL, WATCHDOG_WARNING);
                $page_callback_result = array('msg' => t('404 Not Found'), 'code' => 404);
                break;
            // 403 response
            case MENU_ACCESS_DENIED:
                drupal_add_http_header('Status', 'HTTP/1.1 403 Forbidden');
                watchdog('dlts_utilities', check_plain($_GET['q']), NULL, WATCHDOG_WARNING);
                $page_callback_result = array('msg' => t('403 Forbidden'), 'code' => 403);
                break;
            // 503 response.
            case MENU_SITE_OFFLINE:
                drupal_add_http_header('Status', 'HTTP/1.1 503 Service unavailable');
                $page_callback_result = array('msg' => t('503 Service unavailable.'), 'code' => 503);
                break;
        }
    }
    // Only add Content-Length if we actually have any content. Some non-standard
    // servers get confused by Content-Length in at least HEAD/GET requests
    $content_length = strlen($data);
    if ($content_length > 0) {
        $defaults['Content-Length'] = 'Content-Length: ' . $content_length;
    }
    if (isset($page_callback_result)) {
        if (array_key_exists('callback', $_GET)) {
            drupal_add_http_header('Content-Type', 'text/javascript; charset=utf-8');
            echo sprintf('%s(%s);', filter_xss($_GET['callback'], array()), drupal_json_encode($page_callback_result));
        }
        else {
            drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
            echo drupal_json_encode($page_callback_result);
        }
    }
}
function dlts_viewer_is_pjax() {
  $is_pjax = &drupal_static('is_pjax');
  if (!isset($is_pjax) && function_exists('getallheaders')) {
        $headers = getallheaders();
        if (isset($headers['X-Pjax']) || isset($headers['X-PJAX'])) {
            drupal_add_http_header('uri', request_uri());
            $is_pjax = TRUE;
        }
        else {
            $params = drupal_get_query_parameters();
            if (isset($params['pjax'])) {
                drupal_add_http_header('uri', base_path() . request_uri());
                $is_pjax = TRUE;
            }
            else {
                $is_pjax = FALSE;
            }
        }
    }
    return $is_pjax;
}
function dlts_viewer_is_embed() {
    $is_embed = &drupal_static('is_embed');
    if (!isset($is_embed)) {
        $params = drupal_get_query_parameters();
        if (isset($params['embed'])) {
            if ($params['embed'] === '1' || $params['embed'] === 'true') {
                $is_embed = TRUE;
            }
            else {
                $is_embed = FALSE;
            }
        }
    }
    return $is_embed;
}
function dlts_viewer_viewer_language($vars = NULL) {
    $viewer_language = &drupal_static('viewer_language');
    if (!isset($viewer_language)) {
        if (!empty($vars) && isset($vars['type'])) {
            $obj_language = $vars['language'];
            if ($obj_language === LANGUAGE_NONE) {
                $viewer_language = 'ltr';
            }
            else {
                $language_list = language_list('enabled');
                $enabled_languages = array_pop($language_list);
                if (isset($enabled_languages[$obj_language])) {
                    if ($enabled_languages[$obj_language]->direction) {
                        $viewer_language = 'rtl';
                    }
                    else {
                        $viewer_language = 'ltr';
                    }
                }
                else {
                    $viewer_language = 'rtl';
                }
            }
        }
        else {
            $viewer_language = 'ltr';
        }
    }
    return $viewer_language;
}

function return_nodeid_json($dlts_id, $lang_code = NULL) {
  $langs = $lang_code ? array($lang_code) : array('und', 'en');
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node', '=')
    ->propertyCondition('language', $langs, 'IN')
    ->fieldCondition('field_identifier', 'value', $dlts_id, '=');
  $result = $query->execute();
  $nodeids = array();
  if (isset($result['node'])) {
    foreach ($result['node'] as $node) {
      $nodeids[] = $node->nid;
    }
  }
  echo drupal_json_encode($nodeids);
  die;
}
