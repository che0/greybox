<?php
if ($GLOBALS['cps_teze'] < 1) {
	pg_achtung($lang['access denied']);
	return;
}


$teze_id = $_REQUEST["teze_id"];
$tx = $_REQUEST["tx"];
$jazyk = $_REQUEST["jazyk"];
$komentar = $_REQUEST["komentar"];

if (isset($_REQUEST["delete"])) {
	// delete
	if (cpdb_exec("delete from teze where teze_ID = :teze_id", array(":teze_id"=>$teze_id))) {
		pg_achtung($lang["delete ok"]);
		include("teze.inc");
	}
} else {
	// update
	if (cpdb_exec(
		"update teze set jazyk = :jazyk, tx = :tx, komentar = :komentar where teze_ID = :teze_id", 
		array(":teze_id"=>$teze_id, ":jazyk"=>$jazyk, ":tx" => $tx, ":komentar"=>$komentar))
	) {
		pg_achtung($lang["update ok"]);
		include("teze.detaily.inc");
	}
}
?> 
