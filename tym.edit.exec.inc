<?php

$tym_id = $_REQUEST["tym_id"];
$klub_id = $_REQUEST["klub_id"];
$nazev = $_REQUEST["nazev"];
$komentar = $_REQUEST["komentar"];

if (!($GLOBALS["cps_kluby"] >= 2
	|| ($GLOBALS["cps_kluby"] == 1 && $tym["klub_ID"] == $_SESSION["user_klub_ID"])
	|| (cpdb_fetch_one_value("select aktivni from clovek_tym where clovek_ID = :clovek_id and tym_ID = :tym_id", array(":clovek_id"=>$_SESSION["user_ID"], ":tym_id"=>$tym_id)) == 1))) {
		pg_achtung($lang["access denied"]);
		return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into tym (klub_ID, nazev, komentar) values (:klub_id, :nazev, :komentar)", array(":klub_id"=>$klub_id, ":nazev"=>$nazev, ":komentar"=>$komentar))) {
		pg_achtung($lang["team insert ok"]);
		$_REQUEST["tym_id"] = cpdb_insert_id();
		include("tym.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update tym set klub_ID = :klub_id, nazev = :nazev, komentar = :komentar where tym_ID = :tym_id", array(":tym_id"=>$tym_id, ":nazev"=>$nazev, ":komentar"=>$komentar, ":klub_id"=>$klub_id))) {
		pg_achtung($lang["team update ok"]);
		include("tym.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from tym where tym_ID = :tym_id", array(":tym_id"=>$tym_id))) {
		pg_achtung($lang["team delete ok"]);
		include("klub.inc");
	}
}

?> 
