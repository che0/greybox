<?php
require('inc/head.php');
$template->load('klub');

switch ($_GET['akce']) {
	// akce rozdeleni

	default:
		if ($_GET['id']) {
			// print out one club
			echo $template->make('head');
		} else {
			// print out club list
			set_title($lang['clubs']);
			echo $template->make('head');
		}
	break;
}


echo $template->make('tail');

require('inc/tail.php');
?>
