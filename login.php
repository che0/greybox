<?php
require('inc/head.php');
set_title($lang['login']);
$template->editvar('login_info',$lang['on login page']);


$template->load('login');
$template->editvar('login_username',$lang['username']);
$template->editvar('login_password',$lang['password']);
$template->editvar('login_default_name','');
if ($_GET['von'] != '') {
	$template->editvar('login_von',htmlspecialchars(urldecode($_GET['von'])));
} else {
	$template->editvar('login_von','index.php');
}

if ($_POST['action'] == 'login') {
	$query = 'select clovek.* from login left join clovek using (clovek_ID) where login.username = \'' . $_POST['username'] . '\' and login.password = md5(\'' . $_POST['password'] . '\')';
	$result = mysql_query($query);
	
	if ($result2 = mysql_fetch_array($result)) {
		body_message($lang['login successful']);
		$template->editvar('page_headers','<meta http-equiv="refresh" content="1;url=\'' . htmlspecialchars(urldecode($_POST['von'])) . '\'">');

		// now set all the session variables
		$_SESSION['is_logged_in'] = true;
		$_SESSION['user_name'] = join_name($result2['jmeno'],$result2['nick'],$result2['prijmeni']);
		$_SESSION['user_clovek_ID'] = $result2['clovek_ID'];
		$_SESSION['user_klub_ID'] = $result2['klub_ID'];
		$_SESSION['user_prava_lidi'] = $result2['prava_lidi'];
		$_SESSION['user_prava_kluby'] = $result2['prava_kluby'];
		$_SESSION['user_prava_souteze'] = $result2['prava_souteze'];
		$_SESSION['user_prava_debaty'] = $result2['prava_debaty'];
		$_SESSION['user_prava_teze'] = $result2['prava_souteze'] || $result2['prava_debaty'];
	} else {
		body_message('access denied');
		$template->editvar('login_default_name',stripslashes($_POST['username']));
	}
}

echo $template->make('head');
print_body_messages();

if (! $_SESSION['is_logged_in']) echo $template->make('login');
echo $template->make('tail');

require('inc/tail.php');
?>
