<?php
/*
 * greybox
 * $Id: dblink.inc,v 1.4 2005/01/15 00:21:15 che0 Exp $
 */

$GLOBALS["dblink"] = mysql_connect(
	$GLOBALS["cp_config"]["db_server"],
	$GLOBALS["cp_config"]["db_user"],
	$GLOBALS["cp_config"]["db_password"])
	or die($GLOBALS["lang"]["no database"]);
mysql_select_db ($GLOBALS["cp_config"]["db_database"], $GLOBALS["dblink"]); 

mysql_query("set names 'utf8'");

?>
