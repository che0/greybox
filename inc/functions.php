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
}
?>
