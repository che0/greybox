<?php

if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

$soutez_id = $_REQUEST["soutez_id"];
$jazyk = $_REQUEST["jazyk"];
$nazev = $_REQUEST["nazev"];
$komentar = $_REQUEST["komentar"];
$zamceno = $_REQUEST["zamceno"];

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into soutez(nazev, jazyk, komentar, zamceno, rocnik) values (:nazev, :jazyk, :komentar, :zamceno, :rocnik)", array(":nazev"=>$nazev, ":jazyk"=>$jazyk, ":komentar"=>$komentar, ":zamceno"=>$zamceno, ":rocnik" => $GLOBALS["current_season"]))) {
		pg_achtung("competition insert ok");
		$_REQUEST["soutez_id"] = cpdb_insert_id();
		include("soutez.detaily.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update soutez set nazev = :nazev, jazyk = :jazyk, komentar = :komentar, zamceno = :zamceno where soutez_ID = :soutez_id", array(":nazev"=>$nazev, ":jazyk"=>$jazyk, ":komentar"=>$komentar, ":zamceno"=>$zamceno, ":soutez_id" => $soutez_id))) {
		pg_achtung("competition update ok");
		include("soutez.detaily.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from soutez where soutez_ID = :soutez_id", array(":soutez_id" => $soutez_id))) {
		pg_achtung("competition delete ok");
		include("souteze.inc");
	}
}

?> 