
<div id="book-navbar" class="book-navbar-photos">
    <?php if (isset($pager)) : ?>
        <?php print $pager ?>
    <?php endif; ?>
</div>
<div class="views-g thumbnails-container photoset-thumbs" id="display" >
<?php if (isset($rows)) : ?>

        <?php foreach ($rows as $thumbnail) : ?>
            <div class="thumbHolder" data-uri="<?php print $thumbnail['cm']['uri'] ?>" data-sequence="<?php print $thumbnail['sequence'] ?>" data-width="<?php print $thumbnail['cm']['width'] ?>" data-height="<?php print $thumbnail['cm']['height'] ?>" data-levels="<?php print $thumbnail['cm']['levels'] ?>" data-dwtLevels="<?php print $thumbnail['cm']['dwtLevels'] ?>" data-compositingLayerCount="<?php print $thumbnail['cm']['compositingLayerCount'] ?>">
                    <a href="<?php print $thumbnail['url'] ?>">
                        <img class="thumbItem"  src="<?php print $thumbnail['imageServeURI'] ?>"  />
                    </a>
              
            </div>
        <?php endforeach ?>
<?php endif ?>
</div>

