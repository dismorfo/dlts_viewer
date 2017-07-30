
<div id="book-navbar">
    <?php if (isset($pager)) : ?>
        <?php print $pager ?>
    <?php endif; ?>
</div>
<div  class="views-g pane " >
    <?php if (isset($rows)) : ?>
        <?php foreach ($rows as $row) : ?>
    <div>
            <?php foreach ($row as $thumbnail) : ?>
              <div class="<?php if ($thumbnail['sequence'] == $sequence) { print 'current-page '; } ?>views-row" data-uri="<?php print $thumbnail['cm']['uri'] ?>" data-sequence="<?php print $thumbnail['sequence'] ?>" data-width="<?php print $thumbnail['cm']['width'] ?>" data-height="<?php print $thumbnail['cm']['height'] ?>" data-levels="<?php print $thumbnail['cm']['levels'] ?>" data-dwtLevels="<?php print $thumbnail['cm']['dwtLevels'] ?>" data-compositingLayerCount="<?php print $thumbnail['cm']['compositingLayerCount'] ?>">
                <div class="thumbHolder">
                    <a href="<?php print $thumbnail['url'] ?>">
                        <img class="thumbItem"  src="<?php print $thumbnail['imageServeURI'] ?>"  />
                    </a>
                </div>
              </div>
            <?php endforeach ?>
    </div>
        <?php endforeach ?>
    <?php endif ?>
</div>
