/**
 * @requires OpenLayers/Layer/OpenURL.js
 * @requires OpenLayers/Control/KeyboardDefaults.js
 * @requires OpenLayers/Control/Attribution.js
 * @requires OpenLayers/Control/DLTSZoomOutPanel.js
 * @requires OpenLayers/Control/DLTSZoomInPanel.js
 * @requires OpenLayers/Control/Navigation.js
 * @requires OpenLayers/Control/DLTSScrollWheel.js
 * @requires OpenLayers/Control/TouchNavigation.js
 */

/**
 * Namespace: DLTS
 */
OpenLayers.DLTS = OpenLayers.DLTS || {};

/**
 * DLTS space to cache maps
 */

OpenLayers.DLTS.pages = OpenLayers.DLTS.pages || [];

/**
 * Page Init function
 */

OpenLayers.DLTS.Page = function ( container, url, config, callback ) {

  var params = {
    url_ver: "Z39.88-2004",
    svc_id: "info:lanl-repo/svc/getMetadata",
    layername: "basic",
    format:  "image/jpeg",
    rft_id: url || null,
    imgMetadata: config.imgMetadata,
    zoom: config.zoom || 0,
    service: config.service || null
  },
	
  OUlayer = new OpenLayers.Layer.OpenURL('Page', params.service, { 
    layername: params.layername, 
    format: params.format, 
    rft_id: params.rft_id, 
    imgMetadata: params.imgMetadata,
    resolver: config.resolver || null
  }),
  
  metadata = OUlayer.getImageMetadata(),

  options = {
    center: new OpenLayers.LonLat( metadata.width / 2, metadata.height / 2),
    zoom: config.zoom,
    layers: [ OUlayer ],
    theme: null,
    resolutions: OUlayer.getResolutions(),
    maxExtent: new OpenLayers.Bounds(0, 0, metadata.width, metadata.height),
    tileSize: OUlayer.getTileSize(), 
    controls: []
  },
	        
  controls = [
    new OpenLayers.Control.DLTSZoomInPanel({div: OpenLayers.Util.getElement("control-zoom-in") }),
    new OpenLayers.Control.DLTSZoomOutPanel({div: OpenLayers.Util.getElement("control-zoom-out") }),            
    new OpenLayers.Control.Navigation({
      dragPanOptions: {
        enableKinetic: true
      },
      zoomWheelEnabled: false
    }),
    new OpenLayers.Control.DLTSScrollWheel(),            
    new OpenLayers.Control.TouchNavigation({
      dragPanOptions: {
        enableKinetic: true
      }
    })
  ];
  
  // we can also register "tileloaded" event
  // view-source:http://dev.openlayers.org/releases/OpenLayers-2.12/examples/layerLoadMonitoring.html  
  OUlayer.events.register("loadstart", OUlayer, function() {
    OpenLayers.Element.addClass(document.getElementsByTagName('body')[0], 'openlayers-loading');
  });

  OUlayer.events.register("loadend", OUlayer, function() {
    OpenLayers.Element.removeClass(document.getElementsByTagName('body')[0], 'openlayers-loading');    
  });
  
  var map = new OpenLayers.Map(container, options);

  map.addControls(controls);
  
  map.pan(0, (((map.getSize().h - (map.getTileSize().h * map.resolutions[(map.resolutions.length - (map.getZoom() + 1))])) / 2) - 5));
  
  OpenLayers.DLTS.pages.push(map);
  
  return map;
};
