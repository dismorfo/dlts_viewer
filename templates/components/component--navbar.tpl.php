<ul class="navbar navbar-left">
  <?php if (isset($nav_buttons_left)) : ?>
    <?php foreach ($nav_buttons_left as $button) print $button; ?>
  <?php endif; ?>
</ul>
<div class="navbar navbar-middle">
  <?php if (isset($control_panel)) print $control_panel; ?>
</div>
<ul class="navbar  navbar-arrows">
  <?php if (isset($nav_buttons_arrows)) : ?>
    <?php foreach ($nav_buttons_arrows as $button) print '<li class="navbar-item">' . $button . '</li>'; ?>
  <?php endif; ?>
</ul>

<ul class="navbar-fullscreen">
  <?php foreach ($nav_buttons_right as $button) print $button; ?>
</ul>
