<?php
// FIXME

$klub_id = $_REQUEST["klub_id"];
$misto = $_REQUEST["misto"];
$nazev = $_REQUEST["nazev"];
$kratky_nazev = $_REQUEST["kratky_nazev"];
$komentar = $_REQUEST["komentar"];

if (!($GLOBALS["cps_kluby"] >= 2
	|| ($GLOBALS["cps_kluby"] == 1 && $klub_id == $_SESSION["user_klub_ID"]))) {
		pg_achtung($lang["access denied"]);
		return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into klub (misto, nazev, kratky_nazev, komentar) values (:misto, :nazev, :kratky_nazev, :komentar)", array(":misto"=>$misto, ":kratky_nazev"=>$kratky_nazev, ":nazev"=>$nazev, ":komentar"=>$komentar))) {
		pg_achtung("club insert ok");
		$_REQUEST["klub_id"] = cpdb_insert_id();
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update klub set misto = :misto, nazev = :nazev, kratky_nazev = :kratky_nazev, komentar = :komentar where klub_ID = :klub_id", array(":misto"=>$misto, ":kratky_nazev"=>$kratky_nazev, ":nazev"=>$nazev, ":komentar"=>$komentar, ":klub_id"=>$klub_id))) {
		pg_achtung("club update ok");
		include("klub.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from klub where klub_ID = :klub_id", array(":klub_id"=>$klub_id))) {
		pg_achtung("club delete ok");
		include("kluby.inc");
	}
}

?> 
