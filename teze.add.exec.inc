<?php
/*
 * greybox
 * $Id: teze.add.exec.inc,v 1.4 2005/03/23 13:10:19 che0 Exp $
 */

if ($GLOBALS['cps_teze'] < 1) {
	pg_achtung($lang['access denied']);
	return;
}

$tx = $_REQUEST["tx"];
$tx_short = $_REQUEST["tx_short"];
$jazyk = $_REQUEST["jazyk"];
$komentar = $_REQUEST["komentar"];

if (cpdb_exec(
	"insert into teze (jazyk, tx, tx_short, komentar) values (:jazyk, :tx, :short, :komentar)", 
	array(":jazyk"=>$jazyk, ":tx" => $tx, ":short"=>$tx_short, ":komentar"=>$komentar))
) {
	pg_achtung($lang["add ok"]);
	$_REQUEST["teze_id"] = cpdb_insert_id();
	include("teze.detaily.inc");
}
?>
