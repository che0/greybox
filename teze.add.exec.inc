<?php
/*
 * greybox
 * $Id: teze.add.exec.inc,v 1.2 2005/01/09 00:47:32 che0 Exp $
 */
if ($GLOBALS['cps_teze'] < 1) {
	pg_achtung($lang['access denied']);
	return;
}

$tx = $_REQUEST["tx"];
$jazyk = $_REQUEST["jazyk"];
$komentar = $_REQUEST["komentar"];

if (cpdb_exec(
	"insert into teze (jazyk, tx, komentar) values (:jazyk, :tx, :komentar)", 
	array(":jazyk"=>$jazyk, ":tx" => $tx, ":komentar"=>$komentar))
) {
	pg_achtung($lang["add ok"]);
	$_REQUEST["teze_id"] = cpdb_insert_id();
	include("teze.detaily.inc");
}
?>
