<?php
/******************************************************************
	you configure this
*/

$setup['db_server'] = 'localhost';
$setup['db_database'] = 'greybox';
$setup['db_user'] = 'greybox';
$setup['db_password'] = 'greybox';

$setup['global_title'] = 'greybox';
$setup['global_skin'] = 'default';

$setup['current_season'] = 0; // = 2000/2001

/******************************************************************
	some initialisation
*/

set_magic_quotes_runtime(true);

require('inc/functions.php');


require('inc/template.php');

$template = new template($setup['global_skin']);

$template->load('global');
$template->editvar('page_title',$setup['global_title']);
$template->editvar('pure_title','');
$template->editvar('page_headers','');

require('inc/session.php');

// connect do the database
$db_link = mysql_connect($setup['db_server'], $setup['db_user'], $setup['db_password']) or die($lang['no database']);
mysql_select_db ($setup['db_database'], $db_link);

// language for linkblock
$template->editvar('link_homepage',$lang['link_homepage']);
$template->editvar('link_everyone',$lang['link_everyone']);
$template->editvar('link_clubs',$lang['clubs']);

?>
