<?php

// FIXME adapt!

include_once("lib/points.inc");

if ($GLOBALS["cps_debaty"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$dc_id = $_REQUEST["dc_id"];
$klub_id = $_REQUEST["klub_id"];
$clovek_id = $_REQUEST["clovek_id"];
$role = $_REQUEST["role"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_klub (clovek_ID, klub_ID, role, rocnik) values (:clovek_id, :klub_id, :role, :rocnik)", array(":clovek_id"=>$clovek_id, ":klub_id"=>$klub_id, ":role"=>$role, ":rocnik"=>$rocnik))) {
		ipts_recount_clovek_role($clovek_id, "trener");
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_klub set clovek_ID = :clovek_id, role = :role, rocnik = :rocnik where ck_ID = :ck_id and klub_id = :klub_id", array(":clovek_id"=>$clovek_id, ":rocnik"=>$rocnik, ":ck_id"=>$ck_id, ":role"=>$role, ":klub_id"=>$klub_id))) {
		ipts_recount_clovek_role($clovek_id, "trener");
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_klub where ck_ID = :ck_id and klub_id = :klub_id", array(":ck_id"=>$ck_id, ":klub_id"=>$klub_id))) {
		ipts_recount_clovek_role($clovek_id, "trener");
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
}

?>
