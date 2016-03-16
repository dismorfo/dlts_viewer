 1. Openlayers

 Read release notes @ http://trac.osgeo.org/openlayers/wiki/Release/2.11/Notes

 Download or checkout Openlayers >= 2.11
  
 cd ~/tools/libraries/
 svn co http://svn.openlayers.org/branches/openlayers/2.11 openlayers
  
 2. Google Closure
 
 cd ~/tools/libraries/
 wget http://closure-compiler.googlecode.com/files/compiler-latest.zip
 mv compiler.jar ~/tools/libraries/openlayers/tools/closure-compiler.jar

 3. DLTS Image module

 3.1 Prepare
 
 In order to build a usable OpenLayers version for to use with DLTS Image, move the
 following classes and build profile to their corresponding paths.

  ---------------------------------------------------------------------------------------------------------
 | File                                          |  Path(~/tools/libraries/)                              |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/books.cfg            | ./openlayers/build/books.cfg                            |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTS.js              | ./openlayers/lib/OpenLayers/DLTS.js                     |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSZoomIn.js        | ./openlayers/lib/OpenLayers/Control/DLTSZoomIn.js       |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSZoomOut.js       | ./openlayers/lib/OpenLayers/Control/DLTSZoomOut.js      | 
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSZoomPanel.js     | ./openlayers/lib/OpenLayers/Control/DLTSZoomPanel.js    | 
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSZoomOutPanel.js  | ./openlayers/lib/OpenLayers/Control/DLTSZoomOutPanel.js |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSZoomInPanel.js   | ./openlayers/lib/OpenLayers/Control/DLTSZoomInPanel.js  |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSScrollWheel.js   | ./openlayers/lib/OpenLayers/Control/DLTSScrollWheel.js  |
 |-----------------------------------------------|---------------------------------------------------------|
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/DLTSMouseWheel.js   | ./openlayers/lib/OpenLayers/Handler/DLTSMouseWheel.js    |
 |-----------------------------------------------|---------------------------------------------------------|
 | dlts_image/js/openlayers/OpenURL.js           | ./openlayers/lib/OpenLayers/Layer/OpenURL.js            | 
  ---------------------------------------------------------------------------------------------------------

 3.3 Build
 
 When building a file, you can choose to build with several different compression options 
 with the python-based build.py script. We use Google Closure(see step 2). Tested with
 Python 2.7.2

 Build OpenLayers.js with build.py using books.cfg file.
 
  cd ~/tools/libraries/openlayers/build
 ./build.py -c closure books.cfg

 The resulting ~/tools/libraries/openlayers/build/OpenLayers.js file is optimize for dlts_image module.
 If you need more clases you can create a new .cfg file.

 4. Libraries API

 DLTS Image add Libraries API dependency. See: http://drupal.org/project/libraries 

  Copy the resulting file(from step 3.3) to sites/all/libraries/openlayers
  folder.

  e.g., sites/all/libraries/openlayers/OpenLayers.js
  
 5. Have fun!
 
 Notes:
 
  - ~/tools/libraries is the folder I used in my local machine to organize different kind of libraries, not to be confuse with
   Drupal's all/libraries folder.