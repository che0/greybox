<?php
/*
 * greybox
 * $Id: session.inc,v 1.10 2005/03/28 17:56:34 che0 Exp $
 */

include_once("misc.inc");
include_once("cpdb.inc");

session_start();

$url_von = urlencode($_SERVER["PHP_SELF"] . (($_SERVER["QUERY_STRING"] != "") ? ("?". $_SERVER["QUERY_STRING"]) : ""));

// languages

array ($lang);

switch ($_SESSION['language']) {
	case 'cz':
		$GLOBALS["page"]->language="cz";
		$GLOBALS["page"]->lang_block = sprintf('cz | <a href="./?page=language&amp;jazyk=en&von=%s">en</a>', $url_von);
		require('languages/czech.php');
	break;

	case 'en':
		$GLOBALS["page"]->language="en";
		$GLOBALS["page"]->lang_block = sprintf('<a href="./?page=language&amp;jazyk=cz&amp;von=%s">cz</a> | en', $url_von);
		require('languages/english.php');
	break;
	
	default:
		// = cz, set cz
		$GLOBALS["page"]->language="cs";
		$GLOBALS["page"]->lang_block = sprintf('cz | <a href="./?page=language&amp;jazyk=en&amp;von=%s">en</a>', $url_von);
		require('languages/czech.php');
	break;
}

$GLOBALS["lang"] = & $lang;


// seasons

if (isset($_SESSION["current_season"])) {
	$GLOBALS["current_season"] = $_SESSION["current_season"];
} else {
	$GLOBALS["current_season"] = $GLOBALS["cp_config"]["default_season"];
}
$GLOBALS["page"]->season_block = cphs_link("./?page=season",season_to_str($GLOBALS["current_season"]));

$GLOBALS["season_domain"] = array();
for ($i=3; $i<=7; $i++) {
	$GLOBALS["season_domain"][$i] = season_to_str($i);
}

// permissions

if ($_SESSION["is_logged_in"] == true) {
	$GLOBALS["page"]->login_block = $_SESSION['user_name'] . ' (<a href="./?page=logout">'.$lang['log out'].'</a>)';

	$GLOBALS["cps_lidi"] = $_SESSION['user_prava_lidi'];
	$GLOBALS["cps_kluby"] = $_SESSION['user_prava_kluby'];
	$GLOBALS["cps_debaty"] = $_SESSION['user_prava_debaty'];
	$GLOBALS["cps_souteze"] = $_SESSION['user_prava_souteze'];
	$GLOBALS["cps_teze"] = $_SESSION['user_prava_teze'];
} else {
	$GLOBALS["page"]->login_block = $lang['anonymous'] . ' <a href="./?page=login&amp;amp;von=' . $url_von .'">'.$lang['log in'].'</a>';

	$GLOBALS["cps_lidi"] = -1;
	$GLOBALS["cps_kluby"] = -1;
	$GLOBALS["cps_debaty"] = -1;
	$GLOBALS["cps_souteze"] = -1;
	$GLOBALS["cps_teze"] = -1;
}

?>
