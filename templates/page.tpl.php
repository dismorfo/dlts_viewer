<?php if ( $page ) : ?>
  <div id="page" class="page">
    <?php if ( ! $is_embed ) : ?>
      <?php print $top ?>
    <?php endif; ?>
    <?php print render ( $page['content'] ) ; ?>
  </div>
<?php endif; ?>
