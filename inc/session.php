<?php
//	this will handle sessions
// session variables:
// is_logged_in ... true if we're logged in
// user_name
// user_clovek_ID
// user_klub_ID ... from database
// user_prava_lidi
// user_prava_kluby
// user_prava_souteze
// user_prava_debaty
// user_prava_teze ... according to other rights

session_start();

if ($_SESSION["is_logged_in"] == true) {
	$template->editvar('login_info', $_SESSION['user_name'] . ' (<a href="logout.php">'.$lang['log out'].'</a>)');
} else {
	$template->editvar('login_info', $lang['anonymous'] . ' <a href="login.php?von=' . urlencode($_SERVER["PHP_SELF"] . (($_SERVER["QUERY_STRING"] != "") ? ("?". $_SERVER["QUERY_STRING"]) : "")) .'">'.$lang['log in'].'</a>');
}

?>
