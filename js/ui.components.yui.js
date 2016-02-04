/* jshint laxcomma: true, laxbreak: true, unused: false */
YUI().use(
    'node'
  , 'event'
  , 'event-custom'
  , 'transition'
  , 'slider'
  , 'pjax'
  , 'gallery-soon'
  , 'widget-anim'
  , function(Y) {
    'use strict';
    /** set a X-PJAX HTTP header for all IO requests */
    Y.io.header('X-PJAX', 'true');
    var PJAX_INVALID = -1;
    var PJAX_UNKNOWN_ERROR = -2;    
    var html = Y.one('html');
    var top = Y.one('#top');
    var pagemeta = Y.one('.pane.pagemeta');
    var display = Y.one('#display');
    var pager = Y.one('#pager');
    var displayData = display.getData();
    var land_dir = pager.get('dir');
    var bookUrl = displayData['url'];
    var sequenceCount = parseInt(displayData['sequence-count'] , 10);
    var sequence = parseInt(displayData['sequence'] , 10);
    var slider_datasource = Y.one('#slider_value');
    /** slider object */
    var slider = new Y.Slider({
      axis: 'x', 
      min: 1,
      dir: land_dir, 
      clickableRail: false, 
      max: sequenceCount, 
      value: sequence, 
      length:(Y.one('#pager').get('offsetWidth') - 120) + 'px' 
    });
    /** nodes */
    function resizePageMeta() {
      slider.set('length' ,(Y.one('#pager').get('offsetWidth') - 120 ));
       var viewportHeight = this.get('winHeight'),
        adminBarHeight = 0,
        topHeight = Y.one('#top').get('offsetHeight'),
        navbarHeight = Y.one('#navbar').get('offsetHeight'),
        pageHeight = Y.one('#pager').get('offsetHeight'),
        nodeAdminMenu = Y.one('#admin-menu'),
        sidebarHeight
      ; /** definition list end */
      if (nodeAdminMenu) {
        adminBarHeight =  nodeAdminMenu.get('offsetHeight') ;
      }
      sidebarHeight = viewportHeight - (adminBarHeight + topHeight + navbarHeight + pageHeight);
      Y.one('#pagemeta').setStyles({
        'height': sidebarHeight
      });
    }
    function on_toggle_language(e) {
      Y.log('on_toggle_language');
      return;
      var current_target = e.currentTarget;
      var data_target = current_target.get('value');
      e.preventDefault();
      Y.io(data_target, {
        on: {
          complete: function(id, e) {
            var node = Y.one('#pagemeta');
            var dir;
            node.set('innerHTML', e.response);
            dir = node.one('.node-dlts-book').getAttribute('data-dir');
            Y.one('.pane.main').set('dir', dir);
            Y.one('.titlebar').set('dir', dir);
            Y.one('#page-title').set('innerHTML', node.one('.field-name-field-title .field-item').get('text'));
          }
        }
      });
    }

    function on_button_click(e) {
      e.preventDefault();
      var self = this;
      var current_target = e.currentTarget;
      var event_prefix; 
      var event_id; 
      var node_target;
      var data_target;
      /** don't waste time if the button is inactive */
      if (current_target.hasClass('inactive')) return;
      /** if current target has target, get target from data-target */
      if (current_target.hasClass('target')) {
        data_target = self.getAttribute('data-target');
        event_prefix = 'button:' + data_target;
        /** look-up for the main target */
        node_target = Y.all('#' + data_target);
      }
      /** current target is the main target */
      else {
        event_id = self.get('id');
        event_prefix = 'button:' + event_id;
        /** find possible reference targets to this target */
        node_target = Y.all('a[data-target=' + event_id + ']');
      }
      if (self.hasClass('on')) {
        self.removeClass('on');
        if (Y.Lang.isObject(node_target)) {
          node_target.each(function(node) {
            node.removeClass('on');
          });
        }
        Y.fire(event_prefix + ':off', e);
      } 
      else {
        self.addClass('on');
        if (Y.Lang.isObject(node_target)) {
          node_target.each(function(node) {
            node.addClass('on');
          });
        }
        Y.fire(event_prefix + ':on', e);
      }
      Y.fire(event_prefix + ':toggle', e);
    }

    /** TODO: I don't like this, find a more elegant solution */
    function pager_form(e) {
      e.preventDefault();
      var value = this.get('value');
      var current = parseInt(book.sequence_number, 10);
      var css_class;
      if (value.match(/\D/)) {
        css_class = 'error';
      } 
      else {
        value = parseInt(value, 10);
        if (value !== current &&(value > 0 && value <= sequenceCount )) {
          css_class = 'ok';
          pjax.navigate(bookUrl + '/' +  value);
        } 
        else {
          if (value !== current) {
            css_class = 'error';
          } 
          else {
            css_class = 'warning';
          }
        }
      }
      this.addClass(css_class).transition({
        duration: 1,
        easing: 'ease-in',
        opacity: 0.9
      }, function() {
        this.removeClass(css_class);
      });
    }

    /** callback for changes in the value of the slider */
    function slide_value_change(e) {
      Y.log('Callback for changes in the value of the slider');
      /** slider event */
      if (!Y.Lang.isValue(slider.triggerBy)) {
        slider_datasource.set('value', e.newVal);
      }
      /** event was triggered by reference */
      else {
        slider.triggerBy = undefined;
      }
    }

    /** callback for the slide end event */
    function slide_end(e) {
      e.preventDefault();
      var target = e.target;
      var map = Y.one('.dlts_viewer_map');
      var data = map.getData();
      var request = bookUrl + '/' + e.target.getValue() + '?page_view=' + data.pageview;
      if (!Y.Lang.isValue(slider.triggerBy)) {
        pjax.navigate(request);
        /** slider set focus to the slider rail, blur as soon as possible so that user can use the keyboard to read the book */
        Y.soon(function() { 
          slider.thumb.blur();
        });
      }
      /** event was triggered by reference */
      else {
        slider.triggerBy = undefined;
      }
    }

    function pjax_navigate(e) {
      var msg = e.url.replace(bookUrl, '' ).replace('/' , '');
      if (/(^[\d]+$){1}/.test(msg ) || /(^[\d]+-[\d]+$){1}/.test(msg)) {
        this.one('.current_page').set('text', msg);
      } 
      this.addClass('loading').show();
    }

    /** 
     * pjax callback can be call by clicking a pjax 
     * enable link or by reference with data-url 
     */
    function pjax_callback(e) {
      Y.log('pjax callback can be call by clicking a pjax enable link or by reference with data-url');
      var url;
      e.preventDefault();
      /** test if the target is not active */
      if (e.currentTarget.hasClass('inactive')) return false;
      /** if event has referenceTarget, then event was trigger by reference */
      if (Y.Lang.isObject(e.referenceTarget, true)) {
        url = e.referenceTarget.getAttribute('data-url');
      }
      /** trigger by a pjax enable link */
      else {
        url = this.get('href');
      }
      /** request URL */
      pjax.navigate(url);
    }
    
    function PjaxException(value) {
      this.value = value;
      this.message = "does not conform to the expected format for a PJAX request";
      this.toString = function() {
        return this.value + ' ' + this.message;
      };
    }    

    function pjax_load(e) {
      var config = {};    
      var node = e.content.node;
      var toggle = Y.one('.navbar-item .toggle');
      var next = Y.one('.navbar-item .next');
      var previous = Y.one('.navbar-item .previous');      
      try {
        /** check if request include a map object */
        var map = node.one('.dlts_viewer_map');
        if (map) {
          /** if "toggle" navbar item is available, replace it with this request link */
          if (toggle) {
            toggle.replace(node.one('.toggle').cloneNode(true));
          }
          /** if "next" navbar item is available, replace it with this request link */
          if (next) {
            next.replace(node.one('.next').cloneNode(true));
          }
          /** if "previous" navbar item is available, replace it with this request link */
          if (previous) {
            previous.replace(node.one('.previous').cloneNode(true));
          }
          /** Configuration for the new book page */
          config = {
            id: map.get('id'),
            title: map.getAttribute('data-title'),
            node: map,
            sequence: map.getAttribute('data-sequence'),
            sequenceCount: map.getAttribute('data-sequenceCount'),
            uri: map.getAttribute('data-uri'),
            metadata: {
              width: map.getAttribute('data-width'),
              height: map.getAttribute('data-height'),
              levels: map.getAttribute('data-levels'),
              dwtLevels: map.getAttribute('data-dwtlevels'),
              compositingLayerCount: map.getAttribute('data-compositingLayerCount')
            }
          };
          Y.on('available', change_page, '#' + config.id, OpenLayers, config);
          Y.fire('pjax:load:available', config);
        }
        else {
          throw new PjaxException(e.url);
        }
      }
      catch(e) {
        if (e instanceof PjaxException) {
          return PJAX_INVALID;
        } 
        else {
          return PJAX_UNKNOWN_ERROR;
        }        
      }
    }
    
    function fullscreenOn(e) {
      var docElm = document.documentElement;
      var metadata = Y.one('.metadata');
      var top = Y.one('.top');
      if (docElm.requestFullscreen) {
        docElm.requestFullscreen();
      }
      else if (docElm.msRequestFullscreen) {
        docElm.msRequestFullscreen();
      }
      else if (docElm.mozRequestFullScreen) {
        docElm.mozRequestFullScreen();
      }
      else if (docElm.webkitRequestFullScreen) {
        docElm.webkitRequestFullScreen();
      }      
      if (top) {
        top.addClass('hidden');      
      }
      if (metadata) {
        metadata.removeClass('on');      
      }
    }

    function fullscreenOff(e) {
      var fullscreenButton = Y.one('a.fullscreen');
      var top = Y.one('.top');
      if (document.exitFullscreen) {
        document.exitFullscreen();
      }      
      else if (document.msExitFullscreen) {
        document.msExitFullscreen();
      }
      else if (document.mozCancelFullScreen) {
        document.mozCancelFullScreen();
      }
      else if (document.webkitCancelFullScreen) {
        document.webkitCancelFullScreen();
      }
      if (fullscreenButton) {
        fullscreenButton.blur();
      }
      if (top) {
        top.removeClass('hidden');      
      }
    }    

    function change_page(config) {
      var map;
      var service;
      var zoom;
      var open_layers_dlts = OpenLayers.DLTS;
      if (Y.Lang.isObject(open_layers_dlts.pages[0], true)) {
        map = open_layers_dlts.pages[0];
        service = map.baseLayer.url; // get this value from a data attribute
        zoom = map.getZoom(); // get this value from a data attribute
      }
      open_layers_dlts.pages = [];
      open_layers_dlts.Page(config.id, config.uri, {
        zoom: zoom,
        boxes: config.boxes,
        service: service,
        imgMetadata: config.metadata
      });
      Y.on('contentready', function() {
        Y.later(1000, Y.one('.pane.load'), function() {
          this.hide();
        });
      }, '#' + config.id);
    }
    
    function onButtonMetadataOn(e) {
      this.removeClass('hidden');
      this.ancestor('.pane-body').removeClass('pagemeta-hidden');
    }
    
    function onButtonMetadataOff(e) {
      this.addClass('hidden');
      this.ancestor('.pane-body').addClass('pagemeta-hidden');    
    }    
    
    function openLayersTilesLoading() {
      if (Y.one('body').hasClass('openlayers-loading' )) {
        Y.later(500, Y.one('.pane.load'), openLayersTilesLoading);
      }
      else {
        Y.one('.pane.load').hide();
      }
    }

    function onPjaxLoadAvailable(conf) {
      var page_title = Y.one('#page-title') ;
      var sequence = conf.sequence;
      if (page_title) {
        page_title.set('text', conf.title);
      }
      slider.triggerBy = 'pjax:load:available';
      slider.set('value', parseInt(sequence, 10));
      Y.one('#slider_value').set('value', sequence);      
    }
    
    function onButtonThumbnailsOnIOStart(e) {
      var thumbnails = Y.one('#thumbnails');
      if (thumbnails) {
        thumbnails.removeClass('hidden');
      }
    }

    function onButtonThumbnailsOn(e) {
      var thumbnails = Y.one('#thumbnails');
      var thumbnailsParams = Y.one('#thumbnails-params');
      var data = {};
      if (thumbnailsParams) {
        data = thumbnailsParams.getData();
        Y.io(data.url + '?page=' + data.page + '&rows=' + data.rows,
          { on: { start: onButtonThumbnailsOnIOStart ,
                  complete: onThumbnailsOnSuccess } }
        );
      }
    }
    
    function onButtonThumbnailsOff(e) {
      var thumbnails = Y.one('#thumbnails');
      if (thumbnails) {
      thumbnails.addClass('hidden');       
      }
    }

    function onThumbnailsContainerPagerClick(e) {
      e.preventDefault();
      pjax.navigate(e.currentTarget.get('href'));
    }
    
    function onThumbnailsPagePagerClick(e) {
      var url;
      e.preventDefault();
      /** test if the target is not active */
      if (e.currentTarget.hasClass('inactive')) { 
        return false;
      }
      /** if event has referenceTarget, then event was trigger by reference */
      if (Y.Lang.isObject(e.referenceTarget, true)) {
        url = e.referenceTarget.getAttribute('data-url');
      }
      /** trigger by a pjax enable link */
      else {
        url = this.get('href');
      }
      /** request new page */
      Y.io(url, { on : { 
    	  start: onThumbnailsPageStart, 
    	  end: onThumbnailsPageEnd, 
    	  complete: onThumbnailsPageComplete, 
    	  success: onThumbnailsPageSuccess, 
    	  failure: onThumbnailsPageFailure 
    	} 
      });
    }
    
    // remove content
    function onThumbnailsPageComplete(id, response, args) {
      Y.one('.thumbnails-container').empty();
    }

    // add loading effect
    function onThumbnailsPageStart() {
      Y.one('.thumbnails-container').addClass('io-loading');
    }

    // remove loading effect        
    function onThumbnailsPageEnd() {
      Y.one('.thumbnails-container').removeClass('io-loading');
    }

    function onThumbnailsPageSuccess(id, response) {
      Y.one('.thumbnails-container').set('innerHTML', response.response);
    }

    function onThumbnailsPageFailure(id, request)  {
      Y.log('failure');
    }    

    function onThumbnailsOnSuccess(id, request) {
      var node = Y.one('#thumbnails');
      if (node) {
        node.set('innerHTML', request.response);
        node.addClass('active');
      }
    }
    
    /** render the slider and plug-ins */

    /** events listeners */
    
    slider.render('#slider');

    slider.after('valueChange', slide_value_change);

    slider.after('slideEnd', slide_end, slider);

    Y.on('pjax:load:available', onPjaxLoadAvailable);

    Y.one('.pane.pager').delegate('submit', pager_form, 'form', slider_datasource);

    /** 
     * Pjax object to request new book pages; the content from 
     * successful requests will be appended to "display" pane 
     */
    var pjax = new Y.Pjax({ container: '.pane.display' });

    pjax.on('load', pjax_load);

    pjax.on('navigate', pjax_navigate, Y.one('.pane.load'));
    
    html.delegate('click', on_button_click, 'a.button');
    
    html.delegate('click', pjax_callback, 'a.paging');
    
    Y.on('pjax:change|openlayers:next|openlayers:previous', pjax_callback);

    Y.on('button:button-metadata:on', onButtonMetadataOn , pagemeta);

    Y.on('button:button-metadata:off', onButtonMetadataOff, pagemeta);

    Y.on('button:button-fullscreen:on', fullscreenOn);

    Y.on('button:button-fullscreen:off', fullscreenOff);

    Y.once('contentready', openLayersTilesLoading, '.dlts_image_map');

    Y.on('contentready', resizePageMeta, '#pagemeta');

    Y.on('windowresize', resizePageMeta);

    /** Thumbnails related events */
    Y.on('button:button-thumbnails:on', onButtonThumbnailsOn);

    Y.on('button:button-thumbnails:off', onButtonThumbnailsOff);

    Y.delegate('click', onThumbnailsContainerPagerClick, 'body', '.thumbnails .views-row a');

    Y.one('body').delegate('click', onThumbnailsPagePagerClick, '#thumbnails .pager a');

    Y.delegate('change', on_toggle_language, 'body', '.language');

});
