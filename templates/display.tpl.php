<div id="pagemeta" class="pane pagemeta" dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>">
    <div class="container">
        <<?php print $ds_content_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> " dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>" data-dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>" data-lang="<?php print isset($lang_language) ? $lang_language : "en" ?>" >
        <?php if (isset($lang_options)) : ?>
            <div class="metapane-dropdowns">
                <div class="lang-options"><?php print locale('Available languages', NULL, $lang_language) ?>: <?php print render($lang_options) ; ?></div>
            </div>
        <?php endif; ?>
        <?php print $ds_content; ?>
    </<?php print $ds_content_wrapper ?>>
    <?php if (!empty($drupal_render_children)): ?>
        <?php print $drupal_render_children ?>
    <?php endif; ?>
</div>