<?php
require('inc/head.php');
set_title("logout");

session_destroy();

$template->editvar('login_info',$lang['logging out']);
$template->editvar('page_headers','<meta http-equiv="refresh" content="1;url=\'index.php\'">');
echo $template->make('head');


body_message($lang['logout ok']);

print_body_messages();
echo $template->make('tail');

require('inc/tail.php');
?>
