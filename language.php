<?php
require('inc/head.php');

$_SESSION['language'] = $_GET['lang'];
header_redirect(urldecode($_GET['von']));

echo $template->make('head');

print_one_message($lang['language changed']);

echo $template->make('tail');

require('inc/tail.php');
?>
