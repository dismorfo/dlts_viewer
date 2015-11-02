
Seadragon._DziTileSourceHelper.createFromXmlString = function(xmlString, callback) {

  var async = typeof (callback) == "function";
  
  var error = null;

  function finish(func, obj) {
    try {
      return func(obj, tilesUrl);
    } 
    catch (e) {
      if (async) {
        error = this.getError(e).message;
        return null;
      } 
      else {
        throw this.getError(e);
      }
    }
  }
  
  if (async) {
    if (xmlString) {
      var handler = Function.createDelegate(this, this.processXml);
      
      window.setTimeout(function() {
        var source = finish(handler, Seadragon.Utils.parseXml(xmlString));
        callback(source, error);    // call after finish sets error
      }, 1);
      
    }
    return null;
  }
}
