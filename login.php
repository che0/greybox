<?php
require('inc/head.php');
set_title($lang['login']);
$template->editvar('login_info',$lang['on login page']);


$template->load('login');
$template->editvar('login_username',$lang['username']);
$template->editvar('login_password',$lang['password']);

if ($_POST['action'] == 'login') {
	body_message('evaluating login');
	
	$template->editvar('login_default_name',$_POST['username']);
} else {
	$template->editvar('login_default_name','');
}

echo $template->make('head');
print_body_messages();

if (! $_SESSION['is_logged_in']) echo $template->make('login');
echo $template->make('tail');

require('inc/tail.php');
?>
