<?php

$GLOBALS["dblink"] = mysql_connect(
	$GLOBALS["cp_config"]["db_server"],
	$GLOBALS["cp_config"]["db_user"],
	$GLOBALS["cp_config"]["db_password"])
	or die($GLOBALS["lang"]["no database"]);
mysql_select_db ($GLOBALS["cp_config"]["db_database"], $GLOBALS["dblink"]); 

?>
