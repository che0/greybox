<?php
/*
 * greybox
 * $Id: liga.edit.exec.inc,v 1.1 2005/03/22 18:18:22 che0 Exp $
 */

if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

$liga_id = $_REQUEST["soutez_id"];
$nazev = $_REQUEST["nazev"];
$rocnik = $_REQUEST["rocnik"];
$komentar = $_REQUEST["komentar"];

if ($nazev == "" && !isset($_REQUEST["delete"])) {
	pg_achtung($lang["a league has to have a name"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into liga (nazev, rocnik, komentar) values (:nazev, :rocnik, :komentar)", array(":nazev"=>$nazev, ":rocnik"=>$rocnik, ":komentar"=>$komentar))) {
		pg_achtung($lang["league insert ok"]);
		$_REQUEST["liga_id"] = cpdb_insert_id();
		include("liga.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update liga set nazev = :nazev, rocnik = :rocnik, komentar = :komentar where liga_ID = :liga_id", array(":liga_id"=>$liga_id, ":nazev"=>$nazev, ":rocnik"=>$rocnik, ":komentar"=>$komentar))) {
		pg_achtung($lang["league update ok"]);
		include("liga.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from liga where liga_ID = :liga_id", array(":liga_id"=>$liga_id))) {
		pg_achtung($lang["league delete ok"]);
		include("souteze.inc");
	}
}

?> 
