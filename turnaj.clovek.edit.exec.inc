<?php
/*
 * greybox
 * $Id: turnaj.clovek.edit.exec.inc,v 1.2 2005/01/09 00:47:32 che0 Exp $
 */

include_once("lib/points.inc");

if ($GLOBALS["cps_souteze"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$ct_id = $_REQUEST["ct_id"];
$turnaj_id = $_REQUEST["turnaj_id"];
$clovek_id = $_REQUEST["clovek_id"];
$role = $_REQUEST["role"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_turnaj (clovek_ID, turnaj_ID, role) values (:clovek_id, :turnaj_id, :role)", array(":clovek_id"=>$clovek_id, ":turnaj_id"=>$turnaj_id, ":role"=>$role))) {
		ipts_recount_clovek_role($clovek_id, "organizator");
		pg_achtung($lang["person update ok"]);
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_turnaj set clovek_ID = :clovek_id, role = :role where ct_ID = :ct_id", array(":clovek_id"=>$clovek_id, ":ct_id"=>$ct_id, ":role"=>$role))) {
		ipts_recount_clovek_role($clovek_id, "organizator");
		pg_achtung($lang["person update ok"]);
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_turnaj where ct_ID = :ct_id", array(":ct_id"=>$ct_id))) {
		ipts_recount_clovek_role($clovek_id, "organizator");
		pg_achtung($lang["person update ok"]);
		include("turnaj.inc");
	}
}

?> 
