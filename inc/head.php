<?php
// pre init
$setup = array();

/******************************************************************
	you configure this
*/

$setup['db_host'] = 'localhost';
$setup['db_name'] = 'greybox';
$setup['db_user'] = 'greybox';
$setup['db_passwd'] = 'greybox';

$setup['global_title'] = 'greybox';
$setup['global_lang'] = 'english';
$setup['global_skin'] = 'default';

/******************************************************************
	some initialisation
*/


require('inc/functions.php');

require('inc/template.php');

$template = new template($setup['global_skin']);

$template->load('global');
$template->editvar('page_title',$setup['global_title']);
$template->editvar('page_headers','');

$lang = array();
require('languages/' . $setup['global_lang'] . '.php');

require('inc/session.php');
?>

