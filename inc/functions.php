<?php
// this will define functions for getting full name, recounting points, etc

function set_title($title)
{
	global $template, $setup;
	$template->editvar('page_title',$setup['global_title'].' '.$title);
	$template->editvar('pure_title',$title);
}

function body_message($text)
{
	global $body_messages;
	$body_messages .= "<p class=\"body_message\">" . $text . "</p>\n";
}

function print_body_messages()
{
	global $body_messages;
	echo $body_messages;
	$body_messages = '';
}

function join_name($first, $nick, $last)
{
	if ($nick != "") {
		return $first . ' "' . $nick . '" ' . $last;
	} else {
		return $first . ' ' . $last;
	}
}

function print_comment($comment)
{
	if ($comment != "") {
		// can later make ot process bbcode
		echo '<div class="comment">' . htmlspecialchars($comment) . '</div>';
	}
}

function get_current_season()
{
	// can be later improved with season selection
	global $setup;
	return $setup['current_season'];
}

function season_to_str($season)
{
	return ($season + 2000) . "/" . ($season + 2001);
}

function format_date($date)
{
	return $date;
}
?>
