<div class="dlts_viewer node pjax" data-identifier="<?php print $identifier ?>">
  <?php print $button_previous ?>
  <div id="<?php print $fid ?>"
     class="dlts_viewer_map"
     data-uri="<?php print $uri ?>"
     data-pageView="<?php print $pageView ?>"
     data-sequenceCount="<?php print $sequence_count ?>"
     data-sequence="<?php print $sequence ?>"
     data-title="<?php print $title ?>"
     data-width="<?php print $width ?>"
     data-height="<?php print $height ?>"
     data-levels="<?php print $levels ?>"
     data-dwtLevels="<?php print $dwtLevels ?>"
     data-compositingLayerCount="<?php print $compositingLayerCount ?>">
  </div>
  <?php print $button_next ?>
  <?php print $button_togglepage ?>
  <span id="thumbnails-params"
        data-url="<?php print $thumbnailsURL ?>"
        data-rows="<?php print $thumbnailsRows ?>"
        data-page="<?php print $thumbnailsPage ?>">
  </span>
</div>
