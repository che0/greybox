<?php
/*
 * greybox
 * $Id: turnaj.clovek.edit.exec.inc,v 1.5 2005/01/15 15:14:12 che0 Exp $
 */

include_once("lib/points.inc");

if ($GLOBALS["cps_souteze"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$turnaj_id = $_REQUEST["turnaj_id"];
$clovek_id = $_REQUEST["clovek_id"];
$clovek_old = $_REQUEST["clovek_old"];
$role = $_REQUEST["role"];
$mocnost = $_REQUEST["mocnost"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_turnaj (clovek_ID, turnaj_ID, role, mocnost) values (:clovek_id, :turnaj_id, :role, :mocnost)", array(":clovek_id"=>$clovek_id, ":turnaj_id"=>$turnaj_id, ":role"=>$role, ":mocnost"=>$mocnost))) {
		ipts_recount_turnaj($turnaj_id);
		pg_achtung($lang["person update ok"]);
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_turnaj set clovek_ID = :clovek_id, role = :role, mocnost = :mocnost where clovek_ID = :clovek_old and turnaj_ID = :turnaj_id", array(":clovek_id"=>$clovek_id, ":role"=>$role, ":mocnost"=>$mocnost, ":turnaj_id"=>$turnaj_id, ":clovek_old"=>$clovek_old))) {
		ipts_recount_turnaj($turnaj_id);
		pg_achtung($lang["person update ok"]);
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_turnaj where clovek_ID = :clovek_old and turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id, ":clovek_old"=>$clovek_old))) {
		ipts_recount_turnaj($turnaj_id);
		pg_achtung($lang["person update ok"]);
		include("turnaj.inc");
	}
}

?> 
