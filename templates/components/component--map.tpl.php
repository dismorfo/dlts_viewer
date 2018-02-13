<div class="dlts_viewer node pjax" data-identifier="<?php print $identifier ?>">
  <?php if (isset($button_previous)) : ?>
    <?php print $button_togglepage ?>
  <?php endif; ?>
  <div id="<?php print $fid ?>"class="dlts_viewer_map" <?php foreach($data_attributes as $key => $value) : ?><?php print 'data-' . $key . '="' . $value . '" ' ?><?php endforeach; ?></div>
<?php if (isset($button_next)) : ?>
  <?php print $button_next ?>
<?php endif; ?>
<?php if (isset($button_togglepage)) : ?>
  <?php print $button_togglepage ?>
<?php endif; ?>
</div>