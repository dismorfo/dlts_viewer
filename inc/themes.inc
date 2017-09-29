<?php

/**
 * @file
 * Themes
 */

/**
 * Themes functions
 */
/**
 * Implements hook_custom_theme().
 * http://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_custom_theme/7
 */



function dlts_viewer_theme() {
    return array(
        'dlts_viewer_pager_button' => array(
            'variables' => array(),
        ),
        'thumbnails_pager' => array(
            'variables' => array(
                'tags' => array(),
                'limit' => 10,
                'element' => 0,
                'parameters' => array(),
                'quantity' => 9
            ),
        ),
        'thumbnails_photo_pager' => array(
            'variables' => array(
                'tags' => array(),
                'limit' => 10,
                'element' => 0,
                'parameters' => array(),
                'quantity' => 9
            ),
        ),
        'dlts_viewer_openlayers_image' => array(
            'variables' => array(),
        ),
        'dlts_viewer_navbar_item' => array(
            'variables' => array(),
        ),
        'dlts_viewer_loading' => array(
            'template' => 'templates/components/component--loading',
            'variables' => array(
                'sequence_number' => 0,
            ),
        ),
        'dlts_viewer_slider' => array(
            'template' => 'templates/components/component--slider',
            'variables' => array(
                'slider_id' => 'slider',
                'uri' => base_path(),
                'lang_dir' => 'rtl',
                'sequence_number' => 0,
                'sequence_count' => 0,
            ),
        ),
        'dlts_viewer_navbar' => array(
            'template' => 'templates/components/component--navbar',
            'variables' => array(),
        ),
        'dlts_viewer_thumbnails' => array(
            'template' => 'templates/components/component--thumbnails',
            'variables' => array(),
        ),
        'dlts_viewer_set' => array(
            'template' => 'templates/set',
            'variables' => array(),
        ),
        'dlts_viewer_controlpanel' => array(
            'template' => 'templates/components/component--control_panel',
            'variables' => NULL,
        ),
        'viewer' => array(
            'template' => 'templates/components/component--viewer',
            'variables' => array(),
        ),
        'photo' => array(
            'template' => 'templates/components/component--photo',
            'variables' => array(),
        ),
        'viewer_page' => array(
            'template' => 'templates/components/component--bookpage',
            'variables' => array(),
        ),
        'viewer_map' => array(
            'template' => 'templates/components/component--map',
            'variables' => array(),
        ),
        'viewer_display' => array(
            'template' => 'templates/components/component--display',
            'variables' => array(),
        ),
        'dlts_viewer_oembed' => array(
            'template' => 'templates/components/component--oembed',
            'variables' => array(),
        ),
        'dlts_viewer_top' => array(
            'template' => 'templates/components/component--top',
            'variables' => array(),
        ),
        'html__viewer' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/html',
        ),
        'html__viewer_pjax' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/html.pjax',
        ),
        'page__viewer' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/page',
        ),
        'page__viewer_pjax' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/page.pjax',
        ),
        'ds_1col__node_metadata' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/ds-1col--node-metadata',
        ),
        'node__display' => array(
            'render element' => 'content',
            'base hook' => 'node',
            'template' => 'templates/display',
        ),
        'block__content' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/block',
        ),
        'region__content' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/region',
        ),
        'node__dlts_book' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/node',
        ),
        'node__dlts_book_pjax' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/node.pjax',
        ),
        'node__dlts_map' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/node',
        ),
        'node__dlts_map_pjax' => array(
            'render element' => 'content',
            'base hook' => 'page',
            'template' => 'templates/node.pjax',
        ),
    );
}
function dlts_viewer_theme_registry_alter(&$theme_registry) {
    if (!empty($theme_registry['field'])) {
        $theme_registry['field']['function'] = 'dlts_viewer_theme_field';
    }
}

function theme_dlts_viewer_pager_button($variables) {

	$classes = array('paging');

  if (isset($variables['classes'])) {
    $classes = array_merge($classes, $variables['classes']);
  }

  if (dlts_viewer_is_pjax()) {
    $classes[] = 'pjax';
  }

  $render_array = array(
    'markup' => array(
      '#theme' => 'html_tag',
      '#tag' => 'a',
      '#value' => '<span>' . $variables['text'] . '</span>',
      '#attributes' => array(
        'title' => $variables['text'],
        'href' => $variables['url'],
        'class' =>  $classes,
      ),
    ),
  );
  return drupal_render($render_array);
}

function theme_dlts_viewer_navbar_item($variables) {
  $parts = array('html' => TRUE);
  if (isset($variables['fragment'])) {
    $parts = array_merge($parts, array('fragment' => $variables['fragment']));
  }
  if (isset($variables['attributes'])) {
    $parts = array_merge($parts, array('attributes' => $variables['attributes']));
  }
  if (isset($variables['query'])) {
    $parts = array_merge($parts, array('query' => $variables['query']));
  }
  if (empty($variables['url'])) {
    $url = $variables['path'];
  }
  else {
    $url = $variables['url'];
  }
  return '<li class="navbar-item">' . l(t('<span>@title</span>', array('@title' => $variables['title'])), $url, $parts) . '</li>';
}

/**
 * Returns HTML for a high-resolution JPEG 2000 image.
 *
 * This function returns the necessary HTML elements, and sets certain
 * JavaScript settings for the file being displayed. The given elements are
 * then replaced by OpenLayers with a zoomable version of the image.
 *
 * @ingroup themable
 */
function theme_dlts_viewer_openlayers_image($variables) {

  $file = $variables['file'];

  $fid = 'id-' . $file['fid'];

  $file_uri = file_create_url($file['uri']);

  if (isset($file['timestamp'])) {
  	$file_uri = $file_uri . '?v=' . $file['timestamp'];
  }

  $openlayers_options = array(
    'service' => variable_get('dlts_viewer_djatoka_service', 'http://dl-img.home.nyu.edu/adore-djatoka'),
    'imgMetadata' => array(
      'width' => $file['width'],
      'height' => $file['height'],
      'levels' => $file['levels'],
      'dwtLevels' => $file['dwtLevels'],
      'compositingLayerCount' => $file['compositingLayerCount'],
    ),
    'zoom' => isset($file['zoom']) ? $file['zoom'] : 1,
  );
  
  $json_data = json_encode($openlayers_options);

  $js_inline = '(function(O){O.DLTS.Page("' . $fid . '","' .  $file_uri . '",' . $json_data . ')})(OpenLayers);';

  $js_options = array('group' => JS_DEFAULT, 'type' => 'inline', 'every_page' => FALSE, 'weight' => 5, 'scope' => 'footer', 'cache' => TRUE, 'defer' => TRUE);

  drupal_add_js($js_inline, $js_options);

  if(!isset($file['thumbnailsUrl'])) {
      $file['thumbnailsUrl'] = 'undefined';
  }
  if(!isset($file['thumbnailsRows'])) {
        $file['thumbnailsRows'] = 0;
  }
  if(!isset($file['thumbnailsPage'])) {
        $file['thumbnailsPage'] = 0;
  }

  $options = array(
    '@id' => $fid,
    '@uri' => $file_uri,
    '@width' => $file['width'],
    '@height' => $file['height'],
    '@levels' => $file['levels'],
    '@dwtLevels' => $file['dwtLevels'],
    '@layer' => $file['compositingLayerCount'],
  	'@sequence' => array_shift($file['sequence']),
  	'@thumbnailsUrl' => $file['thumbnailsUrl'],
    '@thumbnailsRows' => $file['thumbnailsRows'],
    '@thumbnailsPage' => $file['thumbnailsPage'],
  	'@pageView' => 'single',
  );

  return t('<div 
    id="@id" 
    class="dlts_image_map dlts_viewer_map" 
    data-sequence="@sequence" 
    data-pageView="@pageView" 
    data-uri="@uri" 
    data-width="@width" 
    data-height="@height" 
    data-levels="@levels" 
    data-dwtLevels="@dwtLevels" 
    data-compositing-layer="@layer"
    data-thumbnails-url="@thumbnailsUrl"
    data-thumbnails-rows="@thumbnailsRows"
    data-thumbnails-page="@thumbnailsPage"
  ></div>', $options);
}

/**
 * Override of theme_pager().
 * Easily one of the most obnoxious theming jobs in Drupal core.
 * Goals: consolidate functionality into less than 5 functions and
 * ensure the markup will not conflict with major other styles
 * (theme_item_list() in particular).
 */
function theme_thumbnails_pager($vars) {
	$tags = $vars['tags'];
	$element = $vars['element'];
	$parameters = $vars['parameters'];
	$quantity = $vars['quantity'];
	$pager_list = theme('pager_list', $vars);
	$links = array();
	$links['pager-first'] = theme('pager_first', array(
			'text' => (isset($tags[0]) ? $tags[0] : t('First')),
			'element' => $element,
			'parameters' => $parameters
	));
	$links['pager-previous'] = theme('pager_previous', array(
			'text' => (isset($tags[1]) ? $tags[1] : t('Prev')),
			'element' => $element,
			'interval' => 1,
			'parameters' => $parameters
	));
	$links['pager-next'] = theme('pager_next', array(
			'text' => (isset($tags[3]) ? $tags[3] : t('Next')),
			'element' => $element,
			'interval' => 1,
			'parameters' => $parameters
	));
	$links['pager-last'] = theme('pager_last', array(
			'text' => (isset($tags[4]) ? $tags[4] : t('Last')),
			'element' => $element,
			'parameters' => $parameters
	));
	$links['close'] = array('title' => t('Close [x]'), 'href'=> '#', 'attributes' => array('class' => array('close')));
	$links = array_filter($links);
	$pager_links = theme('links', array(
			'links' => $links,
			'attributes' => array('class' => 'links pager pager-links')
	));
	if ($pager_list) {
		return '<div class="pager clearfix">' . $pager_list . ' ' . $pager_links . '</div>';
	}
}


function theme_thumbnails_photo_pager($vars) {
    $tags = $vars['tags'];
    $element = $vars['element'];
    $parameters = $vars['parameters'];
    $quantity = $vars['quantity'];
    $pager_list = theme('pager_list', $vars);
    $current_page = $parameters['page'];
    $links = array();
    for($i=0; $i < $quantity; $i++) {
        $links[''.$i] = theme('pager_link', array(
            'text' => (isset($tags[0]) ? $tags[0] : t(''.($i))),
            'element' => $element,
            'parameters' => array('page'=>$i-1),
            'attributes' => array('target'=> '_self'),
            'page_new' => $vars['url']
        ));
    }
    if($current_page>0) {
        $links['pager-first'] = theme('pager_link', array(
            'text' => (isset($tags[0]) ? $tags[0] : t('First')),
            'element' => $element,
            'parameters' => array('page' => 0),
            'attributes' => array('target' => '_self'),
            'page_new' => $vars['url']
        ));
        $links['pager-previous'] = theme('pager_link', array(
            'text' => (isset($tags[1]) ? $tags[1] : t('Prev')),
            'element' => $element,
            'parameters' => array('page' => $current_page - 1),
            'attributes' => array('target' => '_self'),
            'page_new' => $vars['url']
        ));
    }
    if($current_page<$i-2) {
        $links['pager-next'] = theme('pager_link', array(
            'text' => (isset($tags[3]) ? $tags[3] : t('Next')),
            'element' => $element,
            'parameters' => array('page' => $current_page + 1),
            'attributes' => array('target' => '_self'),
            'page_new' => $vars['url']
        ));
        $links['pager-last'] = theme('pager_link', array(
            'text' => (isset($tags[4]) ? $tags[4] : t('Last')),
            'element' => $element,
            'parameters' => array('page' => $i - 2),
            'attributes' => array('target' => '_self'),
            'page_new' => $vars['url']
        ));
    }
    $links = array_filter($links);
    $pager_links = theme('links', array(
        'links' => $links,
        'attributes' => array('class' => 'links pager pager-links')
    ));
    if ($pager_links) {
        return '<div class="pager clearfix">' . $pager_links . '</div>';
    }
}

