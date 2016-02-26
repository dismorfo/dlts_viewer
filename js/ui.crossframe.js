/* jshint laxcomma: true, laxbreak: true, unused: false */
YUI().use(
    'node'
  , 'event'
  , 'event-custom'
  , 'crossframe'
  , function(Y) {
  'use strict';
  Y.on('button:button-metadata:on', function(e) {
    Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-metadata:on'}));
  });
  Y.on('button:button-metadata:off', function(e) {
    Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-metadata:off'}));
  });
  Y.on('button:button-fullscreen:on', function(e) {
    Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-fullscreen:on'}));
  });
  Y.on('button:button-fullscreen:off', function(e) {
    Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'button:button-fullscreen:off'}));
  });
  Y.on('openlayers:change', function(e) {
    Y.CrossFrame.postMessage("parent", JSON.stringify({fire: 'openlayers:change', data: {sequence: e.sequence,title: e.title}}));
  });
});
