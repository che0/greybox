<?php
/*
 * greybox
 * $Id: misc.inc,v 1.9 2007/03/05 15:32:00 che0 Exp $
 */

// debate result constants
define("DRES_AFF", 1);
define("DRES_NEG", 0);
define("DRES_DRAW", 2);

function join_name($first, $nick, $last)
{
	if ($nick != "") {
		return $first . ' "' . $nick . '" ' . $last;
	} else {
		return $first . ' ' . $last;
	}
}

function season_to_str($season)
{
	return ($season + 2000) . "/" . ($season + 2001);
}

function format_date($date)
{
	return strftime('%Y-%m-%d',strtotime($date));
}

function format_full_date($date)
{
	return $date;
}


function deformat_date($date) {
	return $date;
}


function de_html_s($whatever) {
	if (is_array($whatever)) {
		$result = array();
		foreach ($whatever as $key => $value) {
			if ($key == "komentar") {
				include_once("bbcode.inc");
				$result["komentar"] = cp_bbcode($value);
			} else {
				$result[$key] = de_html_s($value);
			}
		}
		return $result;
	} elseif (is_string($whatever)) {
		return htmlspecialchars($whatever);
	} else {
		return $whatever;
	}
}

function de_html(& $whatever) {
	if (is_array($whatever)) {
		foreach ($whatever as $key => $value) {
			if ($key === "komentar") {
				include_once("bbcode.inc");
				$whatever["komentar"] = cp_bbcode($value);
			} else {
				de_html($whatever[$key]);
			}
		}
		reset($whatever);
	} elseif (is_string($whatever)) {
		$whatever = htmlspecialchars($whatever);
	}
}

function filter_comment($tx) {
	return str_replace("[sig]",sprintf("[small]([url=./?page=clovek&clovek_id=%s]%s[/url], %s)[/small]", $_SESSION['user_clovek_ID'], $_SESSION['user_name'], strftime('%Y-%m-%d')), $tx);
}

/// debate result to string
function result_to_str($vitez, $presvedcive) {
	global $lang;
	
	$pers = $presvedcive ? $lang["3:0"] : $lang["2:1"];
	
	switch ($vitez) {
		case DRES_NEG:
			// negative
			return $lang["neg"] . " " . $pers;
		break;
		
		case DRES_AFF:
			// affirmative
			return $lang["aff"] . " " . $pers;
		break;
		
		case DRES_DRAW:
			// draw
			return $lang["draw"];
		break;
	}
}

/// debate result to string, with respect to side
function result_for_side($vitez, $presvedcive, $strana) {
	global $lang;
	
	$pers = $presvedcive ? $lang["3:0"] : $lang["2:1"];
	if ($vitez == DRES_DRAW) {
		return $lang["draw"];
	} elseif ($vitez == $strana) {
		return $lang["won"] . " " . $pers;
	} else {
		return $lang["lost"] . " " . $pers;
	}
}

?>
