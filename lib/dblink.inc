<?php
/*
 * greybox
 * $Id: dblink.inc,v 1.3 2005/01/09 00:47:38 che0 Exp $
 */

$GLOBALS["dblink"] = mysql_connect(
	$GLOBALS["cp_config"]["db_server"],
	$GLOBALS["cp_config"]["db_user"],
	$GLOBALS["cp_config"]["db_password"])
	or die($GLOBALS["lang"]["no database"]);
mysql_select_db ($GLOBALS["cp_config"]["db_database"], $GLOBALS["dblink"]); 

?>
