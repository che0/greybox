<?php
require('inc/head.php');
set_title("logout");

session_destroy();

$template->editvar('login_info','odhlašování...');
$templace->editvar('page_header','<meta http-equiv="refresh" content="1;url=\'index.php\'">');
echo $template->make('head');


body_message($lang['logout ok']);

echo $template->make('tail');

require('inc/tail.php');
?>
