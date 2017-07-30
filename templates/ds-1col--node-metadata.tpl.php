<div id="navbar" class="pane navbar">
    <?php print $navbar ?>
</div>
<div id="main" class="pane main" dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>">
    <div id="pagemeta" class="pane pagemeta">
        <div class="container">
            <<?php print $ds_content_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> " dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>" data-dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>" data-lang="<?php print isset($lang_language) ? $lang_language : "und" ?>" >
            <?php if (isset($lang_options)) : ?>
                <div class="metapane-dropdowns">
                    <div class="lang-options"><?php print locale('Available languages', NULL, $lang_language) ?>: <?php print render($lang_options) ; ?>
                    </div>
                </div>
            <?php endif; ?>
            <?php print $ds_content; ?>
        </<?php print $ds_content_wrapper ?>>
        <?php if (!empty($drupal_render_children)) : print $drupal_render_children; endif; ?>
    </div>
</div>
<div id="display" class="pane display"
     data-url="<?php print $url ?>"
     data-identifier="<?php print $identifier ?>"
     data-sequence-count="<?php print $sequence_count ?>"
     data-sequence="<?php print $book_page_sequence_number ?>"
     data-title="<?php print $title ?>"
     data-pageView="<?php print $pageView ?>">
    <?php print $book_page ?>
    <?php print $button_previous ?>
    <?php print $button_next ?>
</div>
<div class="pane load loading">
    <?php if (isset($loading)) print $loading ; ?>
</div>
</div>
<?php if (isset($read_order)) : ?>
    <div id="pager" class="pane pager" dir="<?php print $read_order; ?>">
        <?php if (isset($slider)) : print $slider ; endif ; ?>
    </div>
<?php endif; ?>
<div id="thumbnails" class="views-g pane thumbnails hidden">
    <?php if (isset($thumbnails)) : print $thumbnails; endif; ?>
</div>
