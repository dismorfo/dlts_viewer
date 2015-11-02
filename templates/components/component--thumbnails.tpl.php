<style>.thumbHolder { background-image: url(<?php print $placeholder ?>); }</style>
<div class="thumbnails-container views-u-1">
  <div id="book-navbar">
    <?php if ( isset ( $pager ) ) : ?>
      <?php print $pager ?>
    <?php endif; ?>    
  </div>
  <div class="view-book-thumbnails">
    <?php if ( isset ( $items ) ) : ?>
      <?php foreach ( $items as $thumbnail ) : ?>
        <div class="views-row" data-uri="<?php print $thumbnail['cm']['uri'] ?>" data-sequenceCount="we need this" data-sequence="<?php print $thumbnail['sequence'] ?>" data-title="we need this" data-width="<?php print $thumbnail['cm']['width'] ?>" data-height="<?php print $thumbnail['cm']['heigh'] ?>" data-levels="<?php print $thumbnail['cm']['levels'] ?>" data-dwtLevels="<?php print $thumbnail['cm']['dwtLevels'] ?>" data-compositingLayerCount="<?php print $thumbnail['cm']['compositingLayerCount'] ?>">
          <div class="thumbHolder">
            <a href="<?php print $thumbnail['url'] ?>"><img src="<?php print $thumbnail['ri']['imageServeURI'] ?>" /></a>
          </div>
          <span class="page-number"><?php print $thumbnail['sequence'] ?></span>
        </div>
      <?php endforeach ?>
    <?php endif; ?>    
  </div>
</div>
