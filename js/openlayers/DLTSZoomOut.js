/* ======================================================================
    OpenLayers/Control/DLTSZoomOut.js
   ====================================================================== */

/**
 * @requires OpenLayers/Control.js
 */

/**
 * Class: OpenLayers.Control.DLTSZoomOut
 * The ZoomOut control is a button to decrease the zoom level of a map.
 *
 * Inherits from:
 *  - <OpenLayers.Control>
 */
OpenLayers.Control.DLTSZoomOut = OpenLayers.Class(OpenLayers.Control, {

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
          , int_before_zoom_out
          , int_after_zoom_out;

        int_before_zoom_out = this.map.zoom;

        if (int_before_zoom_out < 1) { return }
        
        int_after_zoom_out = int_before_zoom_out - 1;

        zoomInDiv = this.map.getControlsByClass('OpenLayers.Control.DLTSZoomIn')[0].panel_div;
        
        zoomOutDiv = this.map.getControlsByClass('OpenLayers.Control.DLTSZoomOut')[0].panel_div;

        if (int_after_zoom_out === 0) {
          OpenLayers.Element.addClass(zoomOutDiv, 'zoom_out_max');
        }

        if (OpenLayers.Element.hasClass(zoomInDiv, 'zoom_in_max')) {
          OpenLayers.Element.removeClass(zoomInDiv, 'zoom_in_max');
        }        

        this.map.zoomOut();

    },

    CLASS_NAME: "OpenLayers.Control.DLTSZoomOut"
});