<?php
/*
 * greybox
 * $Id: soutez.edit.exec.inc,v 1.6 2005/01/09 00:47:32 che0 Exp $
 */

if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

$soutez_id = $_REQUEST["soutez_id"];
$jazyk = $_REQUEST["jazyk"];
$nazev = $_REQUEST["nazev"];
$komentar = $_REQUEST["komentar"];
$zamceno = $_REQUEST["zamceno"];
$rocnik = $_REQUEST["rocnik"];

if ($nazev == "" && !isset($_REQUEST["delete"])) {
	pg_achtung($lang["a competition has to have a name"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into soutez(nazev, jazyk, komentar, zamceno, rocnik) values (:nazev, :jazyk, :komentar, :zamceno, :rocnik)", array(":nazev"=>$nazev, ":jazyk"=>$jazyk, ":komentar"=>$komentar, ":zamceno"=>$zamceno, ":rocnik" => $rocnik))) {
		pg_achtung($lang["competition insert ok"]);
		$_REQUEST["soutez_id"] = cpdb_insert_id();
		include("soutez.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update soutez set nazev = :nazev, jazyk = :jazyk, komentar = :komentar, zamceno = :zamceno, rocnik = :rocnik where soutez_ID = :soutez_id", array(":nazev"=>$nazev, ":jazyk"=>$jazyk, ":komentar"=>$komentar, ":zamceno"=>$zamceno, ":soutez_id" => $soutez_id, ":rocnik" => $rocnik))) {
		pg_achtung($lang["competition update ok"]);
		include("soutez.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from soutez where soutez_ID = :soutez_id", array(":soutez_id" => $soutez_id))) {
		pg_achtung($lang["competition delete ok"]);
		include("souteze.inc");
	}
}

?> 
