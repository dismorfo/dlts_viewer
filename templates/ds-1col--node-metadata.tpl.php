<div id="navbar" class="pane navbar">
  <?php print $navbar ?>
</div>
<div id="main" class="pane main" dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>">
<div id="pagemeta" class="pane pagemeta">
  <div class="container">
    <<?php print $ds_content_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> " dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>" data-dir="<?php print isset($lang_dir) ? $lang_dir : "ltr" ?>" data-lang="<?php print isset($lang_language) ? $lang_language : "und" ?>" >
    <?php if (isset($lang_options) || isset($select_multivolbook) ) : ?>
	  <div class="metapane-dropdowns">
      <?php if (isset($lang_options)) : ?>
  	  <div class="lang-options"><?php print locale('Available languages', NULL, $lang_language) ?>: <?php print render($lang_options) ; ?>
		</div>
		<?php endif; ?>
 	</div>
	<?php endif; ?>
  <?php print $ds_content; ?>
</<?php print $ds_content_wrapper ?>>
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
</div> <!-- end container -->
</div> <!-- end pane -->
 <div id="display" class="pane display" data-url="<?php print $url ?>" data-identifier="<?php print $identifier ?>" data-sequence-count="<?php print $sequence_count ?>" data-sequence="<?php print $book_page_sequence_number ?>" data-title="<?php print $title ?>" data-pageView="<?php print $pageView ?>">
    <?php print $book_page ?>
    <?php print $button_previous ?>
    <?php print $button_next ?>
    <span id="thumbnails-params" data-url="<?php print $thumbnailsURL ?>" data-rows="<?php print $thumbnailsRows ?>" data-page="<?php print $thumbnailsPage ?>"></span>
 </div>
  <div class="pane load loading">
    <?php if (isset($loading ) ) print $loading ; ?>
  </div>
</div>
<?php if (isset($read_order )) : ?>
  <div dir="<?php if (isset($read_order ) ) print $read_order ; else print 'ltr' ?>" id="pager" class="pane pager">
    <?php if (isset($slider ) ) : print $slider ; endif ; ?>
  </div>
<?php endif; ?>
<div id="thumbnails" class="views-g pane thumbnails hidden">
<?php if (isset($thumbnails ) ) : ?>
  <?php print $thumbnails; ?>
<?php endif; ?>
</div>
