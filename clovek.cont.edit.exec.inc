<?php

$kontakt_id = $_REQUEST["kontakt_id"];
$clovek_id = $_REQUEST["clovek_id"];
$druh = $_REQUEST["druh"];
$viditelnost = $_REQUEST["viditelnost"];
$tx = $_REQUEST["tx"];

if (!(($GLOBALS["cps_lidi"] >= 2) || ($GLOBALS["cps_lidi"] == 1 && cpdb_fetch_one_value("select klub_ID from clovek where clovek_ID = :clovek_id",array(":clovek_id"=>$clovek_id)) == $_SESSION["user_klub_ID"]) || ($_SESSION["user_clovek_ID"] == $clovek_id))) {
	pg_achtung($lang["access denied"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into kontakt (clovek_ID, druh, tx, viditelnost) values (:clovek_id, :druh, :tx, :viditelnost)", array(":druh"=>$druh, ":tx"=>$tx, ":viditelnost"=>$viditelnost, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["contact update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update kontakt set druh = :druh, tx = :tx, viditelnost = :viditelnost where kontakt_id = :kontakt_id and clovek_id = :clovek_id", array(":druh"=>$druh, ":tx"=>$tx, ":viditelnost"=>$viditelnost, ":kontakt_id"=>$kontakt_id, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["contact update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from kontakt where kontakt_ID = :kontakt_id and clovek_id = :clovek_id", array(":kontakt_id"=>$kontakt_id, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["contact update ok"]);
		include("clovek.inc");
	}
}

?>
