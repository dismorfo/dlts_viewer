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
        'current_page'=> 0,
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
    'dlts_viewer_dragon' => array(
      'template' => 'templates/components/component--dragon',
      'variables' => array(),
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
  $parts = array('html' => TRUE);
  $variables['attributes']['class'][] = 'paging';
  $variables['attributes']['class'][] = 'button';
  if (isset($variables['fragment'])) {
    $parts = array_merge($parts, array('fragment' => $variables['fragment']));
  }
  if (isset($variables['attributes'])) {
    if (isset($variables['events'])) {
      foreach ($variables['events'] as $type => $events) {
        $variables['attributes']['data-' . $type] = array();
        foreach ($events as $event) {
          $variables['attributes']['data-' . $type][] = $event;
        }
      }
    }
    $parts = array_merge($parts, array('attributes' => $variables['attributes']));
  }
  if (isset($variables['query'])) {
    $parts = array_merge($parts, array('query' => $variables['query']));
  }
  return l(t('<span>@title</span>', array('@title' => $variables['attributes']['title'])), $variables['attributes']['href'], $parts);
}

function theme_dlts_viewer_navbar_item($variables) {
  $parts = array('html' => TRUE);
  if (isset($variables['fragment'])) {
    $parts = array_merge($parts, array('fragment' => $variables['fragment']));
  }
  if (isset($variables['attributes'])) {
    if (isset($variables['events'])) {
      foreach ($variables['events'] as $type => $events) {
        $variables['attributes']['data-' . $type] = array();
        foreach ($events as $event) {
          $variables['attributes']['data-' . $type][] = $event;
        }
      }
    }
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
  return '<li class="navbar-item">' . l(t('<span>@title</span>', array('@title' => $variables['title'])), $url, $parts);
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
  $pager_count=ceil($quantity/10);
  $current_page = $vars['current_page'];
  $links = array();
  for ($i=0; $i < $pager_count; $i++) {
    $attributes = array('target'=> '_self');
    if ($i== $current_page) {
      $links[''.$i] =  array('title' => ''.$i+1);
    } 
    else {
      $links['' . $i] = theme('pager_link', array(
              'text' => (isset($tags[0]) ? $tags[0] : t('' . ($i + 1))),
              'element' => $element,
              'parameters' => array('page' => $i),
              'attributes' => $attributes,
              'page_new' => '/'
          ));
      }
    }
    if($current_page>0) {
        $links['pager-first'] = theme('pager_link', array(
            'text' => (isset($tags[0]) ? $tags[0] : t('First')),
            'element' => $element,
            'parameters' => array('page' => 0),
            'attributes' => array('target' => '_self'),
            'page_new' => '/'
        ));
        $links['pager-previous'] = theme('pager_link', array(
            'text' => (isset($tags[1]) ? $tags[1] : t('Prev')),
            'element' => $element,
            'parameters' => array('page' => $current_page - 1),
            'attributes' => array('target' => '_self'),
            'page_new' => '/'
        ));
    }
    if($current_page<$i-1) {
        $links['pager-next'] = theme('pager_link', array(
            'text' => (isset($tags[3]) ? $tags[3] : t('Next')),
            'element' => $element,
            'parameters' => array('page' => $current_page + 1),
            'attributes' => array('target' => '_self'),
            'page_new' => '/'
        ));
        $links['pager-last'] = theme('pager_link', array(
            'text' => (isset($tags[4]) ? $tags[4] : t('Last')),
            'element' => $element,
            'parameters' => array('page' => $i - 1),
            'attributes' => array('target' => '_self'),
            'page_new' => '/'
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

function dlts_viewer_theme_field($variables) {
	$dir = dlts_viewer_viewer_language((array)$variables['element']['#object']);
	$output = '';
	// Render the label, if it's not hidden.
	if (!$variables['label_hidden']) {
		$lang_language = $variables['element']['#object']->language;
		$label = locale($variables['label'], NULL, $lang_language);
		$output .= '<div dir="'. $dir .'" class="field-label"' . $variables['title_attributes'] . '>' . $label . ':&nbsp;</div>';
	}
	// Render the items.
	$output .= '<div dir="'. $dir .'" class="field-items"' . $variables['content_attributes'] . '>';
	foreach ($variables['items'] as $delta => $item) {
		$classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
		$output .= '<div dir="'. $dir .'" class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
	}
	$output .= '</div>';
	// Render the top-level DIV.
	$output = '<div dir="'. $dir .'" class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';
	return $output;
}

function theme_dlts_viewer_openlayers_image($variables) {
  $defaults = $variables += array(
    'thumbnailsUrl' => '',
    'thumbnailsRows' => 0,
    'thumbnailsPage' => 0,
    'rotation' => 0,
    'preserveViewport' => 0,
    'degrees' => 0,
    'sequenceMode' => 'false',
  );
  return theme('dlts_viewer_dragon', 
    array(
      'dragon' => 
        array(
          'id' => 'id-' . $variables['_id'],
          'class' => 'dlts_viewer_map',
          'data-title' => $defaults['title'],
          'data-identifier' => $defaults['isPartOf'],
          'data-tiles' => drupal_json_encode($defaults['tiles']),
          'data-pagev-view' => $defaults['pageView'],
          'data-sequence-count' => $defaults['sequenceCount'],
          'data-sequence' => $defaults['sequence'],
          'data-sequence-mode' => $defaults['sequenceMode'],
          'data-preserve-viewport' => $defaults['preserveViewport'],
          'data-rotation' => $defaults['rotation'],
          'data-degrees' => $defaults['degrees'],
          'data-thumbnails-url' => $defaults['thumbnailsUrl'],
          'data-thumbnails-rows' => $defaults['thumbnailsRows'],
          'data-thumbnails-page' => $defaults['thumbnailsPage'],
      )
    )
  );
}