<?php
// this will define functions for getting full name, recounting points, etc

function set_title($title)
{
	global $template, $setup;
	$template->editvar('page_title',$setup['global_title'].' '.$title);
}

function body_message($text)
{
	echo '<p class="body_message">' . $text . '</p>';
}
?>
