<?php

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
	return $date;
}

function deformat_date($date) {
	return $date;
}

?>