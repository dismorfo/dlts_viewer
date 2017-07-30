<div id="top" class="pane top views-g">
    <div class="views-u-1">
        <?php if ($is_admin && $messages) : ?>
            <?php print $messages; ?>
        <?php endif; ?>
        <?php if ($title) : ?>
            <?php print $title_prefix; ?>
            <div id="titlebar">
                <h1 id="page-title"><?php print $title; ?></h1>
            </div>
            <?php print $title_suffix; ?>
        <?php endif; ?>
        <?php if ($tabs) : ?>
            <div class="tabs"><?php print $tabs; ?></div>
        <?php endif; ?>
        <?php print $help; ?>
        <?php if ($action_links) : ?>
            <ul class="action-links"><?php  print $action_links; ?></ul>
        <?php endif; ?>
    </div>
</div>
