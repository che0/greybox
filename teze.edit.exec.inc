<?php
/*
 * greybox
 * $Id: teze.edit.exec.inc,v 1.6 2005/04/15 09:08:15 che0 Exp $
 */

if ($GLOBALS['cps_teze'] < 1) {
	pg_achtung($lang['access denied']);
	return;
}

$teze_id = $_REQUEST["teze_id"];
$tx = $_REQUEST["tx"];
$tx_short = $_REQUEST["tx_short"];
$jazyk = $_REQUEST["jazyk"];
$komentar = filter_comment($_REQUEST["komentar"]);

if (isset($_REQUEST["delete"])) {
	// delete
	if (cpdb_exec("delete from teze where teze_ID = :teze_id", array(":teze_id"=>$teze_id))) {
		pg_achtung($lang["delete ok"]);
		include("teze.inc");
	}
} else {
	// update
	if (cpdb_exec(
		"update teze set jazyk = :jazyk, tx = :tx, tx_short = :short, komentar = :komentar where teze_ID = :teze_id", 
		array(":teze_id"=>$teze_id, ":jazyk"=>$jazyk, ":tx" => $tx, ":short"=>$tx_short, ":komentar"=>$komentar))
	) {
		pg_achtung($lang["update ok"]);
		include("teze.detaily.inc");
	}
}
?> 
