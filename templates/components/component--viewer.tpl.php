<?php if (isset($navbar)) : ?>
  <div id="navbar" class="pane navbar">
    <?php print $navbar ?>
  </div>
<?php endif; ?>
<div id="main" class="pane main" dir="<?php print $language_dir ?>">
  <?php if ($pagemeta) : ?>
    <div id="pagemeta" class="pane pagemeta">
      <div class="container">
        <?php print $pagemeta ?>
      </div>
    </div>
  <?php endif; ?>
  <div id="display"
       class="pane display"
       data-title="<?php print $title ?>"
       data-url="<?php print $url ?>"
       data-identifier="<?php print $identifier ?>"
       data-sequence-count="<?php print $sequence_count ?>"
       data-sequence="<?php print $book_page_sequence_number ?>"
  >
    <?php if (isset($book_page)) : ?>
      <?php print $book_page ?>
    <?php endif; ?>
    <?php if (isset($button_previous)) : ?>
      <?php print $button_previous ?>
    <?php endif; ?>
    <?php if (isset($button_next)) : ?>
      <?php print $button_next ?>
    <?php endif; ?>
  </div>
  <?php if (isset($loading)) : ?>
    <div class="pane load loading">
      <?php print $loading ?>
  </div>
  <?php endif; ?>
</div>
<?php if (isset($slider)) : ?>
  <div id="pager" dir="<?php print $language_dir ?>" class="pane pager">
    <?php print $slider ?>
  </div>
<?php endif; ?>
<?php if (isset($thumbnails)) : ?>
  <div id="thumbnails" class="views-g pane thumbnails hidden">
    <?php print $thumbnails ?>
  </div>
<?php endif; ?>
