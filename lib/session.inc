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
		$GLOBALS["page"]->language="cz";
		$GLOBALS["page"]->lang_block = sprintf('cz | <a href="language.php?lang=en&von=%s">en</a>', $url_von);
		require('languages/czech.php');
	break;

	case 'en':
		$GLOBALS["page"]->language="en-us";
		$GLOBALS["page"]->lang_block = sprintf('<a href="language.php?lang=cz&von=%s">cz</a> | en', $url_von);
		require('languages/english.php');
	break;
	
	default:
		// = cz, set cz
		$GLOBALS["page"]->language="cz";
		$GLOBALS["page"]->lang_block = sprintf('cz | <a href="language.php?lang=en&von=%s">en</a>', $url_von);
		require('languages/czech.php');
	break;
}

$GLOBALS["lang"] = & $lang;

if ($_SESSION["is_logged_in"] == true) {
	$GLOBALS["page"]->login_block = $_SESSION['user_name'] . ' (<a href="./?page=logout">'.$lang['log out'].'</a>)';

	$GLOBALS["cps_lidi"] = $prava_lidi = $_SESSION['user_prava_lidi'];
	$GLOBALS["cps_kluby"] = $prava_kluby = $_SESSION['user_prava_kluby'];
	$GLOBALS["cps_debaty"] = $prava_debaty = $_SESSION['user_prava_debaty'];
	$GLOBALS["cps_souteze"] = $prava_souteze = $_SESSION['user_prava_souteze'];
	$GLOBALS["cps_teze"] = $prava_teze = $_SESSION['user_prava_teze'];
} else {
	$GLOBALS["page"]->login_block = $lang['anonymous'] . ' <a href="./?page=login&amp;von=' . $url_von .'">'.$lang['log in'].'</a>';

	$GLOBALS["cps_lidi"] = $prava_lidi = -1;
	$GLOBALS["cps_kluby"] = $prava_kluby = -1;
	$GLOBALS["cps_debaty"] = $prava_debaty = -1;
	$GLOBALS["cps_souteze"] = $prava_souteze = -1;
	$GLOBALS["cps_teze"] = $prava_teze = -1;
}

?>
