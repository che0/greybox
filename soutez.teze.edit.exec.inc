<?php

if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

$st_id = $_REQUEST["st_id"];
$soutez_id = $_REQUEST["soutez_id"];
$teze_id = $_REQUEST["teze_id"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into soutez_teze (soutez_ID, teze_ID) values (:soutez_id, :teze_id)", array(":soutez_id"=>$soutez_id, ":teze_id"=>$teze_id))) {
		pg_achtung("resolution update ok");
		include("soutez.detaily.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update soutez_teze set teze_ID = :teze_id where st_ID = :st_id", array(":teze_id"=>$teze_id, ":st_id"=>$st_id))) {
		pg_achtung("resolution update ok");
		include("soutez.detaily.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from soutez_teze where st_ID = :st_id", array(":st_id" => $st_id))) {
		pg_achtung("resolution update ok");
		include("soutez.detaily.inc");
	}
}

?> 
