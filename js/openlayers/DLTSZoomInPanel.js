/* Copyright (c) 2006-2011 by OpenLayers Contributors (see authors.txt for 
 * full list of contributors). Published under the Clear BSD license.  
 * See http://svn.openlayers.org/trunk/openlayers/license.txt for the
 * full text of the license. */

/**
 * @requires OpenLayers/Control/Panel.js
 * @requires OpenLayers/Control/DLTSZoomIn.js
 */

/**
 * Class: OpenLayers.Control.DLTSZoomInPanel
 * The DLTSZoomInPanel control contain <OpenLayers.Control.ZoomOut>. 
 * By default it is drawn in the upper left corner of the map.
 *
 * Note: 
 * If you wish to use this class with the default images and you want 
 * it to look nice in ie6, you should add the following, conditionally
 * added css stylesheet to your HTML file:
 * 
 * (code)
 * <!--[if lte IE 6]>
 *   <link rel="stylesheet" href="../theme/default/ie6-style.css" type="text/css" />
 * <![endif]-->
 * (end)
 * 
 * Inherits from:
 *  - <OpenLayers.Control.Panel>
 */
OpenLayers.Control.DLTSZoomInPanel = OpenLayers.Class(OpenLayers.Control.Panel, {

    /**
     * Constructor: OpenLayers.Control.DLTSZoomPanel
     * Add the three zooming controls.
     *
     * Parameters:
     * options - {Object} An optional object whose properties will be used
     *     to extend the control.
     */
    initialize: function(options) {
        OpenLayers.Control.Panel.prototype.initialize.apply(this, [options]);
        this.addControls([
            new OpenLayers.Control.DLTSZoomIn()
        ]);
    },

    CLASS_NAME: "OpenLayers.Control.DLTSZoomInPanel"
});