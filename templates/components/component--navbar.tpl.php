<ul class="navbar navbar-left">
  <?php if (isset($nav_buttons_left)) : ?>
    <?php foreach ($nav_buttons_left as $button) print $button; ?>
  <?php endif; ?>
</ul>
<div class="navbar navbar-middle">
  <?php if (isset($control_panel)) print $control_panel; ?>
</div>
<ul class="navbar  navbar-arrows">
  <?php
  if (isset($nav_buttons_pager_left_arrow) || isset($nav_buttons_pager_right_arrow)) {
    print '<li class="navbar-item">' . $nav_buttons_pager_left_arrow . '</li>';
    print '<li class="navbar-item">' . $nav_buttons_pager_right_arrow . '</li>';
  }
  ?>
</ul>

<ul class="navbar-fullscreen">
  <?php foreach ($nav_buttons_right as $button) print $button; ?>
</ul>
