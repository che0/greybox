<?php
/*
 * greybox
 * $Id: turnaj.edit.exec.inc,v 1.6 2005/01/09 00:47:32 che0 Exp $
 */

if ($GLOBALS["cps_souteze"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$soutez_id = $_REQUEST["soutez_id"];
$turnaj_id = $_REQUEST["turnaj_id"];
$nazev = $_REQUEST["nazev"];
$komentar = $_REQUEST["komentar"];
$datum_od = deformat_date($_REQUEST["datum_od"]);
$datum_do = deformat_date($_REQUEST["datum_do"]);

if ($nazev == "" && !isset($_REQUEST["delete"])) {
	pg_achtung($lang["a tournament has to have a name"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into turnaj (soutez_ID, nazev, komentar, datum_od, datum_do) values (:soutez_id, :nazev, :komentar, :datum_od, :datum_do)", array(":soutez_id"=>$soutez_id, ":nazev"=>$nazev, ":komentar"=>$komentar, ":datum_od"=>$datum_od, ":datum_do"=>$datum_do))) {
		pg_achtung($lang["tournament insert ok"]);
		$_REQUEST["turnaj_id"] = cpdb_insert_id();
		include("turnaj.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update turnaj set soutez_ID = :soutez_id, nazev = :nazev, komentar = :komentar, datum_od = :datum_od, datum_do = :datum_do where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id, ":soutez_id"=>$soutez_id, ":nazev"=>$nazev, ":komentar"=>$komentar, ":datum_od"=>$datum_od, ":datum_do"=>$datum_do))) {
		pg_achtung($lang["tournament update ok"]);
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
