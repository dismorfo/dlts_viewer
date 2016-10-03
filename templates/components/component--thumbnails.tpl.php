<div class="thumbnails-container views-u-1">
  <div id="book-navbar">
    <?php if (isset($pager)) : ?>
      <?php print $pager ?>
    <?php endif; ?>
  </div>
  <div class="view-book-thumbnails">
    <?php if (isset($items)) : ?>
      <?php foreach ($items as $thumbnail) : ?>
        <div class="<?php if ($thumbnail['sequence'] == $sequence) { print 'current-page '; } ?>views-row" data-uri="<?php print $thumbnail['cm']['uri'] ?>" data-sequence="<?php print $thumbnail['sequence'] ?>" data-width="<?php print $thumbnail['cm']['width'] ?>" data-height="<?php print $thumbnail['cm']['height'] ?>" data-levels="<?php print $thumbnail['cm']['levels'] ?>" data-dwtLevels="<?php print $thumbnail['cm']['dwtLevels'] ?>" data-compositingLayerCount="<?php print $thumbnail['cm']['compositingLayerCount'] ?>">
          <div class="thumbHolder">
            <a href="<?php print $thumbnail['url'] ?>">
              <img class="thumbItem"  src="<?php print $thumbnail['imageServeURI'] ?>"  />
              <img class="thumbItemloader" src="<?php print $placeholder ?>" height="230" width="166">
              <img class="thumbItembg"  src="<?php print $placeholder ?>" height="230" width="166">
              <span class="page-number"><?php print $thumbnail['sequence'] ?></span>
            </a>
          </div>
        </div>
      <?php endforeach ?>
    <?php endif ?>
  </div>
</div>
