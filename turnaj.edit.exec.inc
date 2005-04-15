<?php
/*
 * greybox
 * $Id: turnaj.edit.exec.inc,v 1.9 2005/04/15 09:08:15 che0 Exp $
 */

include_once("lib/points.inc");

if ($GLOBALS["cps_souteze"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$soutez_id = $_REQUEST["soutez_id"];
$turnaj_id = $_REQUEST["turnaj_id"];
$liga_id = $_REQUEST["liga_id"];
$nazev = $_REQUEST["nazev"];
$komentar = filter_comment($_REQUEST["komentar"]);
$datum_od = deformat_date($_REQUEST["datum_od"]);
$datum_do = deformat_date($_REQUEST["datum_do"]);


if ($nazev == "" && !isset($_REQUEST["delete"])) {
	pg_achtung($lang["a tournament has to have a name"]);
	return;
}

if ($liga_id == 0) $liga_id = NULL;

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into turnaj (soutez_ID, liga_ID, nazev, komentar, datum_od, datum_do) values (:soutez_id, :liga_id, :nazev, :komentar, :datum_od, :datum_do)", array(":soutez_id"=>$soutez_id, ":liga_id"=>$liga_id, ":nazev"=>$nazev, ":komentar"=>$komentar, ":datum_od"=>$datum_od, ":datum_do"=>$datum_do))) {
		pg_achtung($lang["tournament insert ok"]);
		$_REQUEST["turnaj_id"] = cpdb_insert_id();
		if (isset($liga_id)) { lg_recount_league($liga_id); }
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	$liga_pre = cpdb_fetch_one_value("select liga_ID from turnaj where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id));
	
	if (cpdb_exec("update turnaj set soutez_ID = :soutez_id, liga_ID = :liga_id, nazev = :nazev, komentar = :komentar, datum_od = :datum_od, datum_do = :datum_do where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id, ":soutez_id"=>$soutez_id, ":liga_id"=>$liga_id, ":nazev"=>$nazev, ":komentar"=>$komentar, ":datum_od"=>$datum_od, ":datum_do"=>$datum_do))) {
		pg_achtung($lang["tournament update ok"]);
		if ($liga_pre !== $liga_id) {
			if (isset($liga_pre)) { lg_recount_league($liga_pre); }
			if (isset($liga_id)) { lg_recount_league($liga_id); }
		}
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from turnaj where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id))) {
		pg_achtung($lang["tournament delete ok"]);
		include("turnaje.inc");
	}
}

?> 
