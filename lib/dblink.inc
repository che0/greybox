<?php
/*
 * greybox
 * $Id: dblink.inc,v 1.6 2005/02/06 17:44:20 che0 Exp $
 */
 
$nodb = "no database" . ($GLOBALS["embed"] ? "_embed" : "");

$GLOBALS["dblink"] = @mysql_connect(
	$GLOBALS["cp_config"]["db_server"],
	$GLOBALS["cp_config"]["db_user"],
	$GLOBALS["cp_config"]["db_password"])
	or die(sprintf($GLOBALS["cp_config"][$nodb],mysql_error()));

@mysql_select_db ($GLOBALS["cp_config"]["db_database"], $GLOBALS["dblink"])
	or die(sprintf($GLOBALS["cp_config"][$nodb],mysql_error()));

mysql_query("set names 'utf8'");

?>
