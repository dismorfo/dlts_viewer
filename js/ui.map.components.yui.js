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
    , 'crossframe'
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
        var displayData = display.getData();
        var bookUrl = displayData['url'];


        function on_toggle_language(e) {
            var current_target = e.currentTarget;
            var data_target = current_target.get('value');
            e.preventDefault();
            Y.io(data_target, {
                on: {
                    complete: function(id, e) {
                        var node = Y.one('#pagemeta');
                        var dir;
                        var titlebar = Y.one('#titlebar');
                        var pagetitle = Y.one('#page-title');
                        node.set('innerHTML', e.response);
                        dir = node.one('.node-dlts-book').getAttribute('data-dir');
                        Y.one('.pane.main').set('dir', dir);
                        if (titlebar) {
                            titlebar.set('dir', dir);
                        }
                        if (pagetitle) {
                            pagetitle.set('innerHTML', node.one('.field-name-title .field-item').get('text'));
                        }
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
            var url;
            var currentTarget = e.currentTarget;
            e.preventDefault();
            /** test if the target is not active */
            if (currentTarget.hasClass('inactive')) return false;
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
            Y.fire('button:button-thumbnails:off');
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
            var metadata = Y.one('.pagemeta');
            var top = Y.one('.top');
            var button = Y.one('#button-metadata');
            if (button) {
                button.removeClass('on');
            }
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
            Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-fullscreen:on'}));
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
            Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-fullscreen:off'}));
        }



        function onButtonMetadataOn(e) {
            this.removeClass('hidden');
            this.ancestor('.pane-body').removeClass('pagemeta-hidden');
            Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-metadata:on'}));
        }

        function onButtonMetadataOff(e) {
            this.addClass('hidden');
            this.ancestor('.pane-body').addClass('pagemeta-hidden');
            Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-metadata:off'}));
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
            var thumbnails = false;
            var currentPage = false;
            var node = false;
            if (page_title) {
                page_title.set('text', conf.title);
            }
            slider.triggerBy = 'pjax:load:available';
            slider.set('value', parseInt(sequence, 10));
            Y.one('#slider_value').set('value', sequence);
            var thumbnails = Y.one('.view-book-thumbnails');
            if (thumbnails) {
                currentPage = thumbnails.one('.current-page');
                if (currentPage) {
                    currentPage.removeClass('current-page');
                }
                node = thumbnails.one('[data-sequence="'+ sequence +'"]');
                if (node) {
                    node.addClass('current-page');
                }
            }
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

        /** events listeners */


        Y.on('pjax:load:available', onPjaxLoadAvailable);

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

        Y.once('contentready', openLayersTilesLoading, '.dlts_viewer_map');


        // https://github.com/josephj/yui3-crossframe
        function onSelectMVChange(e) {
            e.halt();
            var currentTarget = e.currentTarget;
            var value = currentTarget.one(':checked').get('value');
            var url = value.substring(value.indexOf('::') + 2, value.length);
            var data = { url : url };
            if (window.self === window.top) {
                window.location.replace(url);
            }
            else {
                Y.CrossFrame.postMessage('parent', JSON.stringify({ fire: 'change:option:multivolume', data: $data }));
            }
        }

        // we need to remove all jQuery events for this node (DOM)
        jQuery('.field-name-mv-2016 *').unbind();

        Y.delegate('change', onSelectMVChange, 'body', '.field-name-mv-2016 form');

    });
