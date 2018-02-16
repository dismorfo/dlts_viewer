'use strict';

jQuery(document).ready(($) => {

  let App = {}

  function onAjaxRequestError () { 
    console.log('error') 
  }

  function onAjaxRequestSuccess (response) {
    const responseText = $.parseHTML(response.responseText)
    $('#navbar .previous').replaceWith($(responseText).find('.previous'))
    $('#navbar .next').replaceWith($(responseText).find('.next'))
    $('#navbar .toggle').replaceWith($(responseText).find('.toggle'))
    $('.dlts_viewer_map').replaceWith($(responseText).find('.dlts_viewer_map'))
    ee.emitEvent('viewer-newpage')
  }

  function requestUrl(options) {
    // remove this default
    const success = (OpenSeadragon.isFunction(options.success)) ? options.success : onAjaxRequestSuccess
    OpenSeadragon.makeAjaxRequest({
      url: options.url,
      success: success,
      error: onAjaxRequestError,
      headers: {
        'X-PJAX': true
      }
    })
  }

  function requestBySelector(selector) {
    const $elem = $(selector)
    if ($elem.hasClass('active')) {
      requestUrl({
        url: $elem.attr('href'),
        success: onAjaxRequestSuccess
      })
    }
  }

  const settings = Drupal.settings.dlts_viewer

  const ee = new EventEmitter()  

  ee.defineEvents([
    'viewer-metadata',
    'viewer-next', 
    'viewer-previous', 
    'viewer-fullscreen',
    'viewer-toggle',
    'viewer-zoomin',
    'viewer-zoomout',
    'viewer-thumbnails',
    'viewer-rotate',
    'viewer-newpage',
  ])

  ee.addListener('viewer-rotate', () => {
    App.viewer.viewport.setRotation((App.viewer.viewport ? App.viewer.viewport.getRotation() : App.viewer.degrees || 0) + 90)
  })

  ee.addListener('viewer-metadata', () => {
    $('body').toggleClass('pagemeta-hidden')
  })

  ee.addListener('viewer-thumbnails', () => {
    const $elem = $('#navbar .button.thumbnails')
    const $map = $('.dlts_viewer_map')
    const $thumbnails = $('#thumbnails')
    const state = $elem.attr('data-state') ==  'on' ? 'off' : 'on'
    $elem.attr('data-state', state)
    if (state === 'on') {
      requestUrl({
        url: $map.attr('data-thumbnails-url'),
        success: (request) => {
          if ($thumbnails.length) {
            $thumbnails.removeClass('hidden')
              .addClass('active')
              .html(request.response)
          }   
        }
      })
    }
    else {
      $thumbnails.removeClass('active')
        .addClass('hidden')
        .html('')
    }
  })

  ee.addListener('viewer-zoomin', () => {
    App.viewer.viewport.zoomBy(
      App.viewer.zoomPerClick / 1.0
    )
    App.viewer.viewport.applyConstraints()
  })

  ee.addListener('viewer-zoomout', () => {
    App.viewer.viewport.zoomBy(
      1.0 / App.viewer.zoomPerClick
    )
    App.viewer.viewport.applyConstraints()
  })  

  ee.addListener('viewer-fullscreen', () => {
    const elem = document.getElementById('page')
    // get out of fullscreen
    if (OpenSeadragon.isFullScreen()) {
      OpenSeadragon.exitFullScreen()
      // Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-fullscreen:off'}));
    }
    // go fullscreen
    else {
      OpenSeadragon.requestFullScreen(elem)
      // Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-fullscreen:on'}));
    }    
  })

  ee.addListener('viewer-next', () => {
    requestBySelector('#navbar .button.next')
  })

  ee.addListener('viewer-previous', () => {
    requestBySelector('#navbar .button.previous')
  })

  ee.addListener('viewer-toggle', () => {
    requestBySelector('#navbar .button.toggle')
  })

  ee.addListener('viewer-newpage', () => {
    const $dragon = $('.dlts_viewer_map')
    const context = OpenSeadragon.extend(true, settings, $dragon.data())
    const defaultConfiguration = {
      // Render the best closest level first, ignoring the lowering levels
      // which provide the effect of very blurry to sharp. It is 
      // recommended to change setting to true for mobile devices.
      immediateRender: false,
      preserveViewport: true,
      showRotationControl: true,
      showNavigationControl: false,
      // Enable touch rotation on tactile devices
      gestureSettingsTouch: {
        pinchRotate: true
      }      
    }
    let localConfiguration = {
      id: $dragon.attr('id'),
      degrees: context.degrees,
      // Tile source(s) to open initially. This is a complex parameter; 
      // see {@link OpenSeadragon.Viewer#open} for details.
      tileSources: context.tiles
    }
    if (context.pagevView === 'double') {
      localConfiguration = _.assign({
        sequenceMode: false,
        collectionMode: true,
        collectionRows: 1,
        collectionTileMargin: -278, // why?    
      }, localConfiguration)
    }    
    if (App.viewer) {
      // localConfiguration.defaultZoomLevel = App.viewer.viewport.getZoom()
      localConfiguration.degrees = App.viewer.viewport.getRotation()
      App.viewer.destroy()
      window.history.pushState(
        null, 
        context.title, 
        '/viewer/?q=books/princeton_aco000001/' + context.sequence
      )
    }
    // see: http://openseadragon.github.io/docs/OpenSeadragon.html#.Options
    App.viewer = OpenSeadragon(OpenSeadragon.extend(true, localConfiguration, defaultConfiguration))
    App.viewer.addHandler('open', (data) => {
      const slider_value = $('#slider_value').attr('value')
      const sequence = context.sequence
      if (slider_value !== sequence) {
        $('#slider_value').attr('value', sequence)
      }
    })
    return App.viewer
  })

  $("body").delegate('.button', 'click', function (event) {
    event.preventDefault()
    ee.emitEvent($(this).attr('data-emmit'), this)
  })

  $("#thumbnails").delegate('a', 'click', function (event) {
    event.preventDefault()
    requestUrl({
      url: $(this).attr('href'),
      success: onAjaxRequestSuccess
    })
  })

  $("body").delegate('select.language', 'change', function (event) {
    const $option = $(this).find(':selected')
    requestUrl({
      url: $option.attr('value'),
      success: (response) => {
        const $responseText = $($.parseHTML(response.responseText))
        const dir = $responseText.find('.node-dlts-book').attr('dir')
        $('#main').attr('dir', dir)
        $('#pagemeta').replaceWith($responseText.find('#pagemeta'))
      }
    })
  })

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

  // we need to remove all jQuery events for this node
  jQuery('.field-name-mv-2016 *').unbind()

  // Y.delegate('change', onSelectMVChange, 'body', '.field-name-mv-2016 form')

  ee.emitEvent('viewer-newpage')

})
