<?php
/*
 * greybox
 * $Id: soutez.teze.edit.exec.inc,v 1.7 2005/01/15 15:14:12 che0 Exp $
 */

include("lib/points.inc");

if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

$soutez_id = $_REQUEST["soutez_id"];
$teze_id = $_REQUEST["teze_id"];
$teze_old = $_REQUEST["teze_old"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into soutez_teze (soutez_ID, teze_ID) values (:soutez_id, :teze_id)", array(":soutez_id"=>$soutez_id, ":teze_id"=>$teze_id))) {
		recount_competition($soutez_id);
		pg_achtung($lang["resolution update ok"]);
		include("soutez.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update soutez_teze set teze_ID = :teze_id where soutez_ID = :soutez_id and teze_ID = :teze_old", array(":soutez_id"=>$soutez_id, ":teze_old"=>$teze_old, ":teze_id"=>$teze_id))) {
		recount_competition($soutez_id);
		pg_achtung($lang["resolution update ok"]);
		include("soutez.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from soutez_teze where soutez_ID = :soutez_id and teze_ID = :teze_old", array(":soutez_id"=>$soutez_id, ":teze_old"=>$teze_old))) {
		recount_competition($soutez_id);
		pg_achtung($lang["resolution update ok"]);
		include("soutez.inc");
	}
}

?> 
