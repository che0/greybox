<?php
/*
 * greybox
 * $Id: klub.clovek.edit.exec.inc,v 1.9 2005/01/15 15:14:12 che0 Exp $
 */

include_once("lib/points.inc");

$klub_id = $_REQUEST["klub_id"];
$clovek_id = $_REQUEST["clovek_id"];
$clovek_old = $_REQUEST["clovek_old"];
$role = $_REQUEST["role"];
$rocnik = $_REQUEST["rocnik"];
$rocnik_old = $_REQUEST["rocnik_old"];

if (!(($GLOBALS["cps_kluby"] >= 2)
	|| (($GLOBALS["cps_kluby"] == 1) && ($klub_id == $_SESSION['user_klub_ID'])))) {
	pg_achtung($lang["access denied"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_klub (clovek_ID, klub_ID, role, rocnik) values (:clovek_id, :klub_id, :role, :rocnik)", array(":clovek_id"=>$clovek_id, ":klub_id"=>$klub_id, ":role"=>$role, ":rocnik"=>$rocnik))) {
		ipts_recount_klub($klub_id);
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_klub set clovek_ID = :clovek_id, role = :role, rocnik = :rocnik where klub_id = :klub_id and clovek_ID = :clovek_old and rocnik = :rocnik_old", array(":klub_id"=>$klub_id, ":clovek_old"=>$clovek_old, ":rocnik_old"=>$rocnik_old, ":clovek_id"=>$clovek_id, ":rocnik"=>$rocnik, ":role"=>$role))) {
		ipts_recount_klub($klub_id);
		ipts_recount_clovek($clovek_id);
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_klub where klub_id = :klub_id and clovek_ID = :clovek_old and rocnik = :rocnik_old", array(":klub_id"=>$klub_id, ":clovek_old"=>$clovek_old, ":rocnik_old"=>$rocnik_old))) {
		ipts_recount_clovek($clovek_id);
		pg_achtung($lang["coach update ok"]);
		include("klub.inc");
	}
}

?>
