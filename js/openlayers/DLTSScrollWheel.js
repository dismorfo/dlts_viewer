/**
 * @requires OpenLayers/Control.js
 * @requires OpenLayers/Handler/DLTSMouseWheel.js
 */

/**
 * Class: OpenLayers.Control.DLTSScrollWheel
 * The scrollWheel control pans the map upon scroll wheel use
 * 
 * Inherits from:
 *  - <OpenLayers.Control>
 */

OpenLayers.Control.DLTSScrollWheel = OpenLayers.Class( OpenLayers.Control, {

  /** 
   * Property: type
   * {OpenLayers.Control.TYPES}
   */
	
  type: OpenLayers.Control.TYPE_TOOL,
	
  /**
   * APIProperty: autoActivate
   * {Boolean} Activate the control when it is added to a map.
   */

  autoActivate: true,
  
  /**
   * Property: interval
   * {Integer} The number of milliseconds that should ellapse before
   *     panning the map again. Set this to increase dragging performance.
   *     Defaults to 25 milliseconds.
   */
  
  interval: 25,
  
  /**
   * APIProperty: slideFactor
   * Pixels to slide by.
   */
  slideFactor: 0.75,  
  
  initialize: function ( options ) {
    
	OpenLayers.Control.prototype.initialize.apply( this, arguments );

    this.handler = new OpenLayers.Handler.DLTSMouseWheel( this, {
      'up': this.onWheelUp,
      'down': this.onWheelDown
    });
  },
 
  onWheelUp : function() {
    var size = this.map.getSize();
    this.map.pan( 0, -this.slideFactor*size.h );
  },
  
  onWheelDown : function() {
	var size = this.map.getSize();
    this.map.pan(0, this.slideFactor*size.h);
  },
  
  CLASS_NAME: "OpenLayers.Control.DLTSScrollWheel"
	  
});