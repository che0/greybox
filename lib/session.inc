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

$url_von = urlencode($_SERVER["PHP_SELF"] . (($_SERVER["QUERY_STRING"] != "") ? ("?". $_SERVER["QUERY_STRING"]) : ""));

array ($lang);

switch ($_SESSION['language']) {
	case 'cz':
		$_GLOBALS["page"]->language="cz";
		$_GLOBALS["page"]->lang_block = sprintf('cz | <a href="language.php?lang=en&von=%s">en</a>', $url_von);
		require('languages/czech.php');
	break;

	case 'en':
		$_GLOBALS["page"]->language="en-us";
		$_GLOBALS["page"]->lang_block = sprintf('<a href="language.php?lang=cz&von=%s">cz</a> | en', $url_von);
		require('languages/english.php');
	break;
	
	default:
		// = cz, set cz
		$_GLOBALS["page"]->language="cz";
		$template->editvar('page_header_language','cs');
		$_GLOBALS["page"]->lang_block = sprintf('cz | <a href="language.php?lang=en&von=%s">en</a>', $url_von);
		require('languages/czech.php');
	break;
}

if ($_SESSION["is_logged_in"] == true) {
	$_GLOBALS["page"]->login_block = $_SESSION['user_name'] . ' (<a href="logout.php">'.$lang['log out'].'</a>)';

	$prava_lidi = $_SESSION['user_prava_lidi'];
	$prava_kluby = $_SESSION['user_prava_kluby'];
	$prava_debaty = $_SESSION['user_prava_debaty'];
	$prava_souteze = $_SESSION['user_prava_souteze'];
	$prava_teze = $_SESSION['user_prava_teze'];
} else {
	$_GLOBALS["page"]->login_block =  $lang['anonymous'] . ' <a href="login.php?von=' . $url_von .'">'.$lang['log in'].'</a>';

	$prava_lidi = -1;
	$prava_kluby = -1;
	$prava_debaty = -1;
	$prava_souteze = -1;
	$prava_teze = -1;
}

?>
