<?php

$dbhost = 'localhost';
$dbname = 'greybox';
$dbuser = 'greybox';
$dbpasswd = 'greybox';

/******************************************************************
	some initialisation
*/


$skin = "default";

require('inc/template.php');
$template = new template($skin);

$template->load('global');
$template->editvar('page_headers','');

require('inc/session.php');

$lang_sel = 'english';
require('inc/language.php');
?>
