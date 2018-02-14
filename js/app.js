'use strict';

// https://github.com/rafaelw/mutation-summary
// https://github.com/rafaelw/mutation-summary/blob/master/APIReference.md
// https://github.com/rafaelw/mutation-summary/blob/master/Tutorial.md

$(document).ready(() => {

  let App = {}

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
    'viewer-thumbnails'
  ])

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
      OpenSeadragon.makeAjaxRequest({
        url: $map.attr('data-thumbnails-url'),
        success: (request) => {
          if ($thumbnails.length) {
            $thumbnails.removeClass('hidden')
              .addClass('active')
              .html(request.response)
          }   
        },
        error: onAjaxRequestError,
        headers: {
          'X-PJAX': true
        }
      })
    }
    else {
      $thumbnails.removeClass('active')
        .addClass('hidden')
        .html('')
    }
  })

  ee.addListener('viewer-toggle', () => {
    const $elem = $('#navbar .button.toggle')
    OpenSeadragon.makeAjaxRequest({
      url: $elem.attr('href'), 
      success: onAjaxRequestSuccess,
      error: onAjaxRequestError,
      headers: {
        'X-PJAX': true
      }
    })
  })

  ee.addListener('viewer-zoomin', () => {
    // see: doSingleZoomIn()
    App.viewer.viewport.zoomBy(
      App.viewer.zoomPerClick / 1.0
    )
    App.viewer.viewport.applyConstraints()
  })

  ee.addListener('viewer-zoomout', () => {
    // doSingleZoomOut()
    App.viewer.viewport.zoomBy(
      1.0 / App.viewer.zoomPerClick
    )
    App.viewer.viewport.applyConstraints()
  })  

  ee.addListener('viewer-fullscreen', () => {
    const elem = document.getElementById("page")
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

  function onAjaxRequestError () { 
    console.log('error') 
  }

  function onAjaxRequestSuccess (response) {
    if (response.status === 200) {
      const responseText = $.parseHTML(response.responseText)
      $('#navbar .previous').replaceWith($(responseText).find('.previous'))
      $('#navbar .next').replaceWith($(responseText).find('.next'))
      $('#navbar .toggle').replaceWith($(responseText).find('.toggle'))
      $('.dlts_viewer_map').replaceWith($(responseText).find('.dlts_viewer_map'))
      newPage()
    }
  }

  // https://www.w3schools.com/howto/howto_js_rangeslider.asp

  ee.addListener('viewer-next', () => {
    const $elem = $('#navbar .button.next')
    if ($elem.hasClass('active')) {
      OpenSeadragon.makeAjaxRequest({
        url: $elem.attr('href'), 
        success: onAjaxRequestSuccess,
        error: onAjaxRequestError,
        headers: {
          'X-PJAX': true
        }
      })
    }
  })

  ee.addListener('viewer-previous', () => {
    const $elem = $('#navbar .button.previous')
    if ($elem.hasClass('active')) {
      OpenSeadragon.makeAjaxRequest({
        url: $elem.attr('href'), 
        success: onAjaxRequestSuccess,
        error: onAjaxRequestError,
        headers: {
          'X-PJAX': true
        }
      })
    }
  })

  // http://openseadragon.github.io/docs/OpenSeadragon.html#.Options
  function newPage () {
    const $dragon = $('.dlts_viewer_map')
    const context = _.assign(settings, $dragon.data())
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
      tileSources: context.tiles,
      // Prepends the prefixUrl to navImages paths, which is very 
      // useful since the default paths are rarely useful for production
      // environments.
      prefixUrl: context.path + "/js/openseadragon-bin-2.3.1/images/",      
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

    App.viewer = OpenSeadragon(_.assign(localConfiguration, defaultConfiguration))

    App.viewer.addHandler('open', (data) => {
      const slider_value = $('#slider_value').attr('value')
      const sequence = context.sequence
      if (slider_value !== sequence) {
        $('#slider_value').attr('value', sequence)
      }
    })

    App.viewer.addHandler('close', (data) => {
      console.log('close')
    })

    return App.viewer

  }

  $("body").delegate('.button', 'click', function (event) {
    event.preventDefault()
    const $this = $(this)
    // if (!$this.hasClass('inactive')) {
      $this.toggleClass('on')
      ee.emitEvent($this.attr('data-emmit'), this)
    // }
  })

  newPage()

})

// slider
// length:(Y.one('#pager').get('offsetWidth') - 120) + 'px'
