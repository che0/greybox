<?php
/*
 * greybox
 * $Id: misc.inc,v 1.7 2005/03/19 22:10:34 che0 Exp $
 */

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


?>
