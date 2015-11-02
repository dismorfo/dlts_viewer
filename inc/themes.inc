<?php

/**
 * Themes functions
 */
function theme_dlts_book_api_pager_button  ( $arguments ) {

  $isPJAX = dlts_book_api_is_pjax() ;

  $pjax = $isPJAX ? 'pjax' : '' ;

  $status = $arguments['active'] ? 'active' : 'inactive' ;

  $classes = array ( $pjax, 'paging' , $arguments['operation'], $status ) ;

  $render_array = array (
    'markup' => array (
        '#theme' => 'html_tag',
        '#tag' => 'a',
        '#value' => '<span>' . $arguments['text'] . '</span>',
        '#attributes' => array (
            'title' => $arguments['text'] ,
            'class' =>  $classes,
            'href' => $arguments['url'] ,
        ),
    ),
  );

  return drupal_render ( $render_array ) ;

}

function theme_dlts_book_api_navbar_item ( $variables = array ( ) ) {

  $parts = array( 'html' => TRUE ) ;

  if ( isset ( $variables['fragment'] ) ) {
    $parts = array_merge($parts, array( 'fragment' => $variables['fragment']));
  }

  if (isset($variables['attributes'])) {
    $parts = array_merge($parts, array('attributes' => $variables['attributes']));
  }

  if (isset($variables['query'])) {
    $parts = array_merge($parts, array('query' => $variables['query']));
  }
  
  $url = ( empty ( $variables['url'] ) ) ? $variables['path'] : $variables['url']  ;
  
  return '<li class="navbar-item">'. l('<span>' . $variables['title'] . '</span>', $url, $parts) . '</li>';

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
function theme_dlts_book_api_openlayers_image ( $variables ) {

	$file = $variables['file'];

	$fid = 'id-'. $file['fid'];

	$fileUri = file_create_url($file['uri']);

	if ( isset ( $file['timestamp'] ) ) $fileUri = $fileUri . '?v=' . $file['timestamp'];

	/** Add Openlayers to the page */
	drupal_add_js( variable_get('dlts_image_openlayers_source', 'sites/all/libraries/openlayers/lib/OpenLayers.js'), array('group' => JS_LIBRARY ) ) ;

	if (!isset($file['djakota_width'])) {
		$djatoka = dlts_image_djatoka_request($file);
		if (!isset($file['djakota_width'])) {
			$file['djakota_width'] = $djatoka['width'];
			$file['djakota_height'] = $djatoka['height'];
			$file['djakota_levels'] = $djatoka['levels'];
			$file['djakota_dwtLevels'] = $djatoka['dwtLevels'];
			$file['djakota_compositingLayerCount'] = $djatoka['compositingLayerCount'];
		}
	}

	$openlayers_options = array(
			'service' => variable_get('dlts_image_djatoka_service', 'http://localhost:8080/adore-djatoka'),
			'imgMetadata' => array(
					'width' => $file['djakota_width'],
					'height' => $file['djakota_height'],
					'levels' => $file['djakota_levels'],
					'dwtLevels' => $file['djakota_dwtLevels'],
					'compositingLayerCount' => $file['djakota_compositingLayerCount'],

			),
			'zoom' => isset($file['zoom']) ? $file['zoom'] : 1,
	);

	$js_inline = '(function(O){O.DLTS.Page("'. $fid .'","'.  $fileUri .'",'. json_encode($openlayers_options) .')})(OpenLayers);';

	$js_options = array(
			'group' => JS_DEFAULT,
			'type' => 'inline',
			'every_page' => FALSE,
			'weight' => 5,
			'scope' => 'footer',
			'cache' => TRUE,
			'defer' => TRUE,
	);

	drupal_add_js($js_inline, $js_options);

	$options = array(
			'@id' => $fid,
			'@uri' => $fileUri,
			'@width' => $file['djakota_width'],
			'@height' => $file['djakota_height'],
			'@levels' => $file['djakota_levels'],
			'@dwtLevels' => $file['djakota_dwtLevels'],
			'@layer' => $file['djakota_compositingLayerCount'],
	);

	return t('<div id="@id" class="dlts_image_map" data-uri="@uri" data-width="@width" data-height="@height" data-levels="@levels" data-dwtLevels="@dwtLevels" data-compositing-layer="@layer"></div>', $options);

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
function theme_dlts_book_api_seadragon_image ( $variables ) {

	$file = $variables['file'];

	$fid = 'id-'. $file['fid'];

	$fileUri = file_create_url($file['uri']);

	if ( isset ( $file['timestamp'] ) ) $fileUri = $fileUri . '?v=' . $file['timestamp'];

	/** Add Openlayers to the page */
	// drupal_add_js( variable_get('dlts_image_openlayers_source', 'sites/all/libraries/openlayers/lib/OpenLayers.js'), array('group' => JS_LIBRARY ) ) ;
	// drupal_add_css('sites/all/libraries/openlayers/theme/default/style.css') ;

	drupal_add_js( drupal_get_path('module', 'dlts_book_api') . '/js/DjatokaSeadragon/OpenSeadragon.debug.js' , array('group' => JS_LIBRARY ) ) ;
	drupal_add_js( drupal_get_path('module', 'dlts_book_api') . '/js/DjatokaSeadragon/hack.js' , array('group' => JS_LIBRARY ) ) ;

	$a = 'http://dev-dl-pa.home.nyu.edu/viewer/' . drupal_get_path('module', 'dlts_book_api') . '/js/DjatokaSeadragon' ;

	if (!isset($file['djakota_width'])) {
		$djatoka = dlts_image_djatoka_request($file);
		if (!isset($file['djakota_width'])) {
			$file['djakota_width'] = $djatoka['width'];
			$file['djakota_height'] = $djatoka['height'];
			$file['djakota_levels'] = $djatoka['levels'];
			$file['djakota_dwtLevels'] = $djatoka['dwtLevels'];
			$file['djakota_compositingLayerCount'] = $djatoka['compositingLayerCount'];
		}
	}

	$djatoka_service = 'http://dl-imgdev.home.nyu.edu/adore-djatoka/resolver' ;

	$openlayers_options = array(
			'service' => variable_get('dlts_image_djatoka_service', 'http://localhost:8080/adore-djatoka'),
			'imgMetadata' => array(
					'width' => $file['djakota_width'],
					'height' => $file['djakota_height'],
					'levels' => $file['djakota_levels'],
					'dwtLevels' => $file['djakota_dwtLevels'],
					'compositingLayerCount' => $file['djakota_compositingLayerCount'],

			),
			'zoom' => isset($file['zoom']) ? $file['zoom'] : 1,
	);

	$xmlString = "<?xml version=1.0 encoding=UTF-8?><Image xmlns=http://schemas.microsoft.com/deepzoom/2008 Format=jpg Overlap=1 TileSize=256><Size Width=" . $file['djakota_width'] ." Height=" . $file['djakota_height'] . "/></Image>" ;

	$b = 'Seadragon.Config = function () {

    this.debugMode = true;

    this.animationTime = 1.5;

    this.blendTime = 0.5;

    this.alwaysBlend = false;

    this.autoHideControls = true;

    this.immediateRender = false;

    this.wrapHorizontal = false;

    this.wrapVertical = false;

    this.minZoomImageRatio = 0.8;

    this.maxZoomPixelRatio = 2;

    this.visibilityRatio = 0.5;

    this.springStiffness = 5.0;

    this.imageLoaderLimit = 2;

    this.clickTimeThreshold = 200;

    this.clickDistThreshold = 5;

    this.zoomPerClick = 2.0;

    this.zoomPerScroll = 1.2;

    this.zoomPerSecond = 2.0;

    this.showNavigationControl = true;

    this.maxImageCacheCount = 100;

    this.minPixelRatio = 0.5;

    this.mouseNavEnabled = true;

    this.navImages = {
		zoomIn: {
			REST: "'.$a.'/images/zoomin_rest.png",
			GROUP: "'.$a.'/images/zoomin_grouphover.png",
			HOVER: "'.$a.'/images/zoomin_hover.png",
			DOWN: "'.$a.'/images/zoomin_pressed.png"
		},
		zoomOut: {
			REST: "'.$a.'/images/zoomout_rest.png",
			GROUP: "'.$a.'/images/zoomout_grouphover.png",
			HOVER: "'.$a.'/images/zoomout_hover.png",
			DOWN: "'.$a.'/images/zoomout_pressed.png"
		},
		home: {
			REST: "'.$a.'/images/home_rest.png",
			GROUP: "'.$a.'/images/home_grouphover.png",
			HOVER: "'.$a.'/images/home_hover.png",
			DOWN: "'.$a.'/images/home_pressed.png"
		},
		fullpage: {
			REST: "'.$a.'/images/fullpage_rest.png",
			GROUP: "'.$a.'/images/fullpage_grouphover.png",
			HOVER: "'.$a.'/images/fullpage_hover.png",
			DOWN: "'.$a.'/images/fullpage_pressed.png"
		}
    }
}';

	$js_inline = $b . ';function init () { var djbase = "' . $djatoka_service . '?url_ver=Z39.88-2004&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/jpeg&svc.rotate=0&rft_id=' . $fileUri . '" ; var viewer = new Seadragon.Viewer("' . $fid .'", djbase) ; viewer.openDzifromXMLString( "' . $xmlString . '") ; } Seadragon.Utils.addEvent(window, "load", init);' ;

	// $js_inline = '(function(O){O.DLTS.Page("'. $fid .'","'.  $fileUri .'",'. json_encode($openlayers_options) .')})(OpenLayers);';

	$js_options = array(
			'group' => JS_DEFAULT,
			'type' => 'inline',
			'every_page' => FALSE,
			'weight' => 5,
			'scope' => 'footer',
			'cache' => TRUE,
			'defer' => TRUE,
	);

	drupal_add_js($js_inline, $js_options);

	$options = array(
			'@id' => $fid,
			'@uri' => $fileUri,
			'@width' => $file['djakota_width'],
			'@height' => $file['djakota_height'],
			'@levels' => $file['djakota_levels'],
			'@dwtLevels' => $file['djakota_dwtLevels'],
			'@layer' => $file['djakota_compositingLayerCount'],
	);

	return t('<div id="@id" class="dlts_image_map" data-uri="@uri" data-width="@width" data-height="@height" data-levels="@levels" data-dwtLevels="@dwtLevels" data-compositing-layer="@layer"></div>', $options);

}
