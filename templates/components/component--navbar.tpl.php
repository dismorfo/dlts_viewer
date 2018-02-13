<ul class="navbar navbar-left">
  <?php if (isset($nav_buttons_left)) : ?>
    <?php foreach ($nav_buttons_left as $button) : ?>
      <?php print $button ?>
    <?php endforeach ?>
  <?php endif ?>
</ul>
<ul class="navbar navbar-middle">
  <?php if (isset($control_panel)) : ?>
    <?php foreach ($control_panel as $button) : ?>
      <?php print $button ?>
    <?php endforeach ?>    
  <?php endif ?>
</ul>
<ul class="navbar navbar-arrows">
  <?php if (isset($nav_buttons_pager_left_arrow) || isset($nav_buttons_pager_right_arrow)) : ?>
    <?php print $nav_buttons_pager_left_arrow ?>
    <?php print $nav_buttons_pager_right_arrow ?>
  <?php endif ?>
</ul>
<ul class="navbar-fullscreen">
  <?php foreach ($nav_buttons_right as $button) : ?>
    <?php print $button ?>
  <?php endforeach ?>
</ul>
