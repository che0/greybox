<?php

$dbhost = 'localhost';
$dbname = 'greybox';
$dbuser = 'greybox';
$dbpasswd = 'greybox';

/******************************************************************
	some initialisation
*/

include('inc/template.php');
$template = new template;

$template->load('default/global');
$template->editvar('page_headers','');
?>
