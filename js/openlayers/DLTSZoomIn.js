/* ======================================================================
    OpenLayers/Control/DLTSZoomIn.js
   ====================================================================== */
/**
 * @requires OpenLayers/Control.js
 */

/**
 * Class: OpenLayers.Control.DLTSZoomIn
 * The ZoomIn control is a button to increase the zoom level of a map.
 *
 * Inherits from:
 *  - <OpenLayers.Control>
 */

OpenLayers.Control.DLTSZoomIn = OpenLayers.Class(OpenLayers.Control, {

    /**
     * Property: type
     * {String} The type of <OpenLayers.Control> -- When added to a 
     *     <Control.Panel>, 'type' is used by the panel to determine how to 
     *     handle our events.
     */
    type: OpenLayers.Control.TYPE_BUTTON,
    
    /**
     * Method: trigger
     */
    trigger: function() {

        var zoomInDiv
          , zoomOutDiv
          , int_before_zoom_in
          , max_zoom;

        max_zoom = this.map.resolutions.length - 1;
        
        int_before_zoom_in = this.map.zoom;
        
        if (int_before_zoom_in === max_zoom) { return }    
    
        int_before_zoom_in = int_before_zoom_in + 1;

        zoomInDiv = this.map.getControlsByClass('OpenLayers.Control.DLTSZoomIn')[0].panel_div;
        
        zoomOutDiv = this.map.getControlsByClass('OpenLayers.Control.DLTSZoomOut')[0].panel_div;
        
        if (int_before_zoom_in === max_zoom) {
          OpenLayers.Element.addClass(zoomInDiv, 'zoom_in_max');
        }

        if (OpenLayers.Element.hasClass(zoomOutDiv, 'zoom_out_max')) {
          OpenLayers.Element.removeClass(zoomOutDiv, 'zoom_out_max');
        }

        this.map.zoomIn();

    },

    CLASS_NAME: "OpenLayers.Control.DLTSZoomIn"
});