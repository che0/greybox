<?php

$db_link = mysql_connect($setup['db_server'], $setup['db_user'], $setup['db_password']) or die($lang['no database']);
mysql_select_db ($setup['db_database'], $db_link); 

?>
