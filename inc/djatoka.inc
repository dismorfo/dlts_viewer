<?php

/**
 * @file
 * Djatoka
 */

/**
 * Djatoka request
 */
function dlts_viewer_djatoka_request($file) {
	$file = (object) $file;
  $service_url = variable_get('dlts_viewer_djatoka_service', 'http://dl-img.home.nyu.edu/adore-djatoka');
  $rft_id = file_create_url($file->uri);
  // aof; here for now.
  //if (isset($file->timestamp)) {
    //$rft_id = $rft_id . '?v=' . $file->timestamp;
  //}
  if (!empty($service_url)) {
    $request = url($service_url . '/resolver', array('absolute' => TRUE, 'query' => array('url_ver' => 'Z39.88-2004', 'svc_id' => 'info:lanl-repo/svc/getMetadata', 'rft_id' => $rft_id)));
    $response = drupal_http_request($request);
    if ($response->code == '200' && isset($response->data)) {
      return drupal_json_decode($response->data);
    }
    else {
    	drupal_set_message(t('<em>Djatoka Image Server</em>: Error requesting image metadata from Djakota service. @url.', array('@url' => $rft_id)), 'error');
      watchdog('dlts_viewer', 'Error requesting image metadata from Djakota service(url @url). @message(error code @code). Please verify that the ' . l(t('DLTS Image'), 'admin/config/media/dlts-image') . ' module is properly configured. ', array('@message' => $response->error, '@code' => $response->code, '@url' => $request ), WATCHDOG_ERROR);
      return array('message' => isset($response->status_message) ? $response->status_message : $response->error, 'error' => $response->error, 'code' => $response->code);
    }
  }
  else {
  	drupal_set_message(t('<em>Djatoka Image Server</em>: Error requesting image metadata from Djakota service. @url.', array('@url' => $rft_id)), 'error');
    watchdog('dlts_viewer', 'Error requesting image metadata from Djakota service. Please configure Djatoka Jpeg 2000 Image Server.', array(), WATCHDOG_ERROR);
    return array('message' => $msg, 'error' => 404, 'code' => 404);
  }
}

function dlts_viewer_djatoka_url($file, $embeded_tumb=false) {
	$service = variable_get('dlts_viewer_djatoka_service', 'http://dl-img.home.nyu.edu/adore-djatoka');
	$arguments = array(
	  'url_ver' => 'Z39.88-2004',
		'svc_id' => 'info:lanl-repo/svc/getRegion',
		'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
		'svc.format' => 'image/jpeg',
		'rft_id' => file_create_url($file['uri']),
	);
    if($embeded_tumb) {
        $arguments['svc.scale'] = '100';
    } else {
        if (isset($file['image_style'])) {
            $dimmensions = explode("x", $file['image_style']);
            if (count($dimmensions) == 2 && is_numeric($dimmensions[1])) {
                $arguments['svc.scale'] = $dimmensions[1];
            }
        } else {
            $arguments['svc.scale'] = '230';
        }
    }

	return url($service . '/resolver', array('external' => true, 'query' => $arguments));
}
