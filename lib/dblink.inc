<?php
/*
 * greybox
 * $Id: dblink.inc,v 1.5 2005/02/06 17:37:47 che0 Exp $
 */

$GLOBALS["dblink"] = @mysql_connect(
	$GLOBALS["cp_config"]["db_server"],
	$GLOBALS["cp_config"]["db_user"],
	$GLOBALS["cp_config"]["db_password"])
	or die(sprintf($GLOBALS["cp_config"]["no database"],mysql_error()));

@mysql_select_db ($GLOBALS["cp_config"]["db_database"], $GLOBALS["dblink"])
	or die(sprintf($GLOBALS["cp_config"]["no database"],mysql_error()));

mysql_query("set names 'utf8'");

?>
