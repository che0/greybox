<?php
require('inc/head.php');
$template->editvar('page_title','pokus');

echo $template->make('head');
require('inc/session.php');
echo $template->make('linkblock');

echo '<p>blabla</p>';

echo $template->make('tail');

require('inc/tail.php');
