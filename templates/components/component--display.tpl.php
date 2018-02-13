<div id="pagemeta" class="pane pagemeta" dir="<?php print $lang_dir ?>">
  <div class="<?php print $classes ?> " dir="<?php print $lang_dir ?>" data-dir="<?php print $lang_dir ?>" data-lang="<?php print $lang_language ?>>
  <?php if ($lang_options) : ?>
    <div class="metapane-dropdowns">
      <div class="lang-options"><?php print locale('Available languages', NULL, $lang_language) ?>: <?php print render($lang_options) ?></div>
    </div>
  <?php endif ?>
  <?php print $ds_content ?>
</div>