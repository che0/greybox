<?php

include_once("lib/points.inc");

if (!(($GLOBALS["cps_kluby"] >= 2)
	|| (($GLOBALS["cps_kluby"] == 1) && ($klub_id == $_SESSION['user_klub_ID'])))) {
	pg_achtung($lang["access denied"]);
	return;
}

$ck_id = $_REQUEST["ck_id"];
$klub_id = $_REQUEST["klub_id"];
$clovek_id = $_REQUEST["clovek_id"];
$role = $_REQUEST["role"];
$rocnik = $_REQUEST["rocnik"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_klub (clovek_ID, klub_ID, role, rocnik) values (:clovek_id, :klub_id, :role, :rocnik)", array(":clovek_id"=>$clovek_id, ":klub_id"=>$klub_id, ":role"=>$role, ":rocnik"=>$rocnik))) {
		ipts_recount_klub($klub_id);
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_klub set clovek_ID = :clovek_id, role = :role, rocnik = :rocnik where ck_ID = :ck_id and klub_id = :klub_id", array(":clovek_id"=>$clovek_id, ":rocnik"=>$rocnik, ":ck_id"=>$ck_id, ":role"=>$role, ":klub_id"=>$klub_id))) {
		ipts_recount_klub($klub_id);
		ipts_recount_clovek($clovek_id);
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_klub where ck_ID = :ck_id and klub_ID = :klub_id", array(":ck_id"=>$ck_id, ":klub_id"=>$klub_id))) {
		ipts_recount_clovek($clovek_id);
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
}

?>
