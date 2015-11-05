/* Copyright (c) UNC Chapel Hill University Library, created by Hugh A. Cayless
 * and revised by J. Clifford Dyer.  Published under the Clear BSD licence.  
 * See http://svn.openlayers.org/trunk/openlayers/license.txt for the full 
 * text of the license. 
 */

/**
 * @requires OpenLayers/Layer/Grid.js
 * @requires OpenLayers/Tile/Image.js
 */

/**
 * Class: OpenLayers.Layer.OpenURL
 * 
 * Inherits from:
 *  - <OpenLayers.Layer.Grid>
 */
OpenLayers.Layer.OpenURL = OpenLayers.Class(OpenLayers.Layer.Grid, {

    /**
     * APIProperty: isBaseLayer
     * {Boolean}
     */
    isBaseLayer: true,   

    /**
     * APIProperty: tileOrigin
     * {<OpenLayers.Pixel>}
     */
    tileOrigin: null,    
    url_ver: 'Z39.88-2004',
    rft_id: null,
    svc_id: "info:lanl-repo/svc/getRegion",
    svc_val_fmt: "info:ofi/fmt:kev:mtx:jpeg2000",
    format: null,
    tileHeight: null,
    viewerWidth: 512,
    viewerHeight: 512,
    minDjatokaLevelDimension: 48,
    djatokaURL: '/resolver',
    transitionEffect: 'resize',
	
    /**
     * Constructor: OpenLayers.Layer.OpenURL
     * 
     * Parameters:
     * name - {String}
     * url - {String}
     * options - {Object} Hashtable of extra options to tag onto the layer
     */
    initialize: function(name, url, options) {
      var minLevel, viewerLevel, width, height, i;
       
       if (options.viewerWidth) {
          this.viewerWidth = options.viewerWidth;
       }

        if (options.viewerHeight) {
          this.viewerHeight = options.viewerHeight;
        }
        
        if (options.resolver) {
          this.djatokaURL = options.resolver;
        }
        
        OpenLayers.Layer.Grid.prototype.initialize.apply(this, [name, url, {}, options]);
        
        this.rft_id = options.rft_id;
        this.format = options.format;
        
        // Get image metadata if it hasn't been set
        if (!options.imgMetadata) {
          this.imgMetadata = this.requestToJSON( OpenLayers.Request.issue({url: options.metadataUrl, async: false }) );
        } else {
          this.imgMetadata = options.imgMetadata;
        }
        
        minLevel = this.getMinLevel();

        // viewerLevel is the smallest useful zoom level: i.e., it is the largest level that fits entirely 
        // within the bounds of the viewer div.
        
        viewerLevel = Math.ceil(Math.min(minLevel, Math.max((Math.log(this.imgMetadata.width) - Math.log(this.viewerWidth)), (Math.log(this.imgMetadata.height) - Math.log(this.viewerHeight))) / Math.log(2)));
        this.zoomOffset = minLevel - viewerLevel;

        // width at level viewerLevel
        width = this.imgMetadata.width / Math.pow(2, viewerLevel);

        // height at level viewerLevel
        height = this.imgMetadata.height / Math.pow(2, viewerLevel);

        this.resolutions = [];

        for (i = viewerLevel; i >= 0; i--) {
          this.resolutions.push(Math.pow(2, i));
        }
        this.tileSize = new OpenLayers.Size(Math.ceil(width), Math.ceil(height));

    },

    /**
     * APIMethod:destroy
     */
    destroy: function() {
      // for now, nothing special to do here. 
      OpenLayers.Layer.Grid.prototype.destroy.apply(this, arguments);
    },

    
    /**
     * APIMethod: clone
     * 
     * Parameters:
     * obj - {Object}
     * 
     * Returns:
     * {<OpenLayers.Layer.OpenURL>} An exact clone of this <OpenLayers.Layer.OpenURL>
     */
    clone: function (obj) {
      if (obj == null) {
          obj = new OpenLayers.Layer.OpenURL(this.name, this.url, this.options);
      }

      //get all additions from superclasses
      obj = OpenLayers.Layer.Grid.prototype.clone.apply(this, [obj]);

      // copy/set any non-init, non-simple values here

      return obj;
    },    
    
    /**
     * Method: getURL
     * 
     * Parameters:
     * bounds - {<OpenLayers.Bounds>}
     * 
     * Returns:
     * {String} A string with the layer's url and parameters and also the
     *          passed-in bounds and appropriate tile size specified as 
     *          parameters
     */
    getURL: function (bounds) {
      var url, z, path;
      bounds = this.adjustBounds(bounds);
      this.calculatePositionAndSize(bounds);
      z = this.map.getZoom() + this.zoomOffset;
      path = this.djatokaURL + "?url_ver=" + this.url_ver + "&rft_id=" + this.rft_id + "&svc_id=" + this.svc_id + "&svc_val_fmt=" + this.svc_val_fmt + "&svc.format=" + this.format + "&svc.level=" + z + "&svc.rotate=0&svc.region=" + this.tilePos.lat + "," + this.tilePos.lon + "," + this.imageSize.h + "," + this.imageSize.w;
      url = this.url;
      if (url instanceof Array) {
        url = this.selectUrl(path, url);
      }
      return url + path;
    },

    /**
     * Method: addTile
     * addTile creates a tile, initializes it, and adds it to the layer div. 
     * 
     * Parameters:
     * bounds - {<OpenLayers.Bounds>}
     * position - {<OpenLayers.Pixel>}
     * 
     * Returns:
     * {<OpenLayers.Tile.Image>} The added OpenLayers.Tile.Image
     */
    addTile:function(bounds,position) {
      this.calculatePositionAndSize(bounds);
      return new OpenLayers.Tile.Image(this, position, bounds, null, this.imageSize);
    },

    /** 
     * APIMethod: setMap
     * When the layer is added to a map, then we can fetch our origin 
     *    (if we don't have one.) 
     * 
     * Parameters:
     * map - {<OpenLayers.Map>}
     */
    setMap: function(map) {
      OpenLayers.Layer.Grid.prototype.setMap.apply(this, arguments);
      if (!this.tileOrigin) { 
        this.tileOrigin = new OpenLayers.LonLat(this.map.maxExtent.left, this.map.maxExtent.bottom);
      }                                       
      
    },
    
    calculatePositionAndSize: function(bounds) {
      // Have to recalculate x and y (instead of using bounds and resolution), because resolution will be off.
      // Get number of tiles in image
      
      var max = this.map.getMaxExtent(),
          xtiles = Math.round( 1 / (this.tileSize.w / max.getWidth())),
          xpos = Math.round((bounds.left / max.getWidth()) * xtiles), // Find out which tile we're on      
          x = xpos * (this.tileSize.w + 1), // Set x
          xExtent = max.getWidth() / this.map.getResolution(),
          ytiles = Math.round( 1 / (this.tileSize.h / max.getHeight())),     
          y = max.getHeight() - bounds.top, // Djatoka's coordinate system is top-down, not bottom-up, so invert for y
          y = y < 0? 0 : y,
          ypos = Math.round((y / max.getHeight()) * ytiles),
          y = ypos * (this.tileSize.h + 1),
          yExtent = max.getHeight() / this.map.getResolution(),          
          w = this.tileSize.w,
          h = this.tileSize.h,
          minustile =  xtiles - 1;

      if (xpos === minustile) {
        w = xExtent % (this.tileSize.w + 1);
      }
      if (ypos === minustile) {
        h = yExtent % (this.tileSize.h + 1);
      }
      this.tilePos = new OpenLayers.LonLat(x,y);
      this.imageSize = new OpenLayers.Size(w,h);
    },
    
    getImageMetadata: function() {
      return this.imgMetadata;
    },
    
    getResolutions: function() {
      return this.resolutions;
    },
    
    getTileSize: function() {
      return this.tileSize;
    },

    getMinLevel: function() {
        // Versions of djatoka from before 4/17/09 have levels set to the 
        // number of levels encoded in the image.  After this date, that 
        // number is assigned to the new dwtLevels, and levels contains the
        // number of levels between the full image size and the minimum 
        // size djatoka could return.  We want the lesser of these two numbers.

        var levelsInImg,
            levelsToDjatokaMin,
            maxImgDimension;
        
        if (this.imgMetadata.dwtLevels === undefined) {
          maxImgDimension = Math.max(this.imgMetadata.width, this.imgMetadata.height);
          levelsInImg = this.imgMetadata.levels;
          levelsToDjatokaMin = Math.floor((Math.log(maxImgDimension) - Math.log(this.minDjatokaLevelDimension)) / Math.log(2));
        } 
        else {
          levelsInImg = this.imgMetadata.dwtLevels;
          levelsToDjatokaMin = this.imgMetadata.levels;
        }
        return Math.min(levelsInImg, levelsToDjatokaMin);
    },
    
    requestToJSON: function( request ) {
      if ( request.status === 200 ) {    	  
        return new OpenLayers.Format.JSON().read( request.responseText );
      }
      else {
        return null;
      }
    },

    CLASS_NAME: "OpenLayers.Layer.OpenURL"
});