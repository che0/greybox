<?php
//	this will handle sessions
// session variables:
// is_logged_in ... true if we're logged in
// user_name
// user_clovek_ID
// user_longname ... according to name fields
// user_klubID ... from database
// user_prava_lidi
// user_prava_kluby
// user_prava_souteze
// user_prava_debaty
// user_prava_teze ... according to other rights

session_start();

if (isset($_SESSION["is_logged_in"])) {
	$template->editvar('login_info',$_SESSION['user_longname'] . ' (<a href="logout.php">'.$lang['log out'].'</a>)');
} else {
	$template->editvar('login_info','anonymous user <a href="login.php">'.$lang['log in'].'</a>');
}


?>
