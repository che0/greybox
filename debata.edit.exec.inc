<?php
include("lib/points.inc");

if ($GLOBALS["cps_debaty"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$debata_id = $_REQUEST["debata_id"];
$_SESSION["dbt_cache"]["soutez_ID"] = $soutez_id = $_REQUEST["soutez_id"];
$_SESSION["dbt_cache"]["turnaj_ID"] = $turnaj_id = $_REQUEST["turnaj_id"];
$_SESSION["dbt_cache"]["datum"] = $datum = $_REQUEST["datum"];
$_SESSION["dbt_cache"]["misto"] = $misto = $_REQUEST["misto"];
$_SESSION["dbt_cache"]["teze_ID"] = $teze_id = $_REQUEST["teze_id"];
$_SESSION["dbt_cache"]["aff_ID"] = $teams["aff"]["id"] = $_REQUEST["aff_id"];
$teams["aff"]["pozice"] = 1;
$_SESSION["dbt_cache"]["a1_ID"] = $speakers["a1"]["id"] = $_REQUEST["a1_id"];
$_SESSION["dbt_cache"]["a1_kidy"] = $speakers["a1"]["kidy"] = $_REQUEST["a1_kidy"];
$speakers["a1"]["tym"] = "aff";
$_SESSION["dbt_cache"]["a2_ID"] = $speakers["a2"]["id"] = $_REQUEST["a2_id"];
$_SESSION["dbt_cache"]["a2_kidy"] = $speakers["a2"]["kidy"] = $_REQUEST["a2_kidy"];
$speakers["a2"]["tym"] = "aff";
$_SESSION["dbt_cache"]["a3_ID"] = $speakers["a3"]["id"] = $_REQUEST["a3_id"];
$_SESSION["dbt_cache"]["a3_kidy"] = $speakers["a3"]["kidy"] = $_REQUEST["a3_kidy"];
$speakers["a3"]["tym"] = "aff";
$_SESSION["dbt_cache"]["neg_ID"] = $teams["neg"]["id"] = $_REQUEST["neg_id"];
$teams["neg"]["pozice"] = 0;
$_SESSION["dbt_cache"]["n1_ID"] = $speakers["n1"]["id"] = $_REQUEST["n1_id"];
$_SESSION["dbt_cache"]["n1_kidy"] = $speakers["n1"]["kidy"] = $_REQUEST["n1_kidy"];
$speakers["n1"]["tym"] = "neg";
$_SESSION["dbt_cache"]["n2_ID"] = $speakers["n2"]["id"] = $_REQUEST["n2_id"];
$_SESSION["dbt_cache"]["n2_kidy"] = $speakers["n2"]["kidy"] = $_REQUEST["n2_kidy"];
$speakers["n2"]["tym"] = "neg";
$_SESSION["dbt_cache"]["n3_ID"] = $speakers["n3"]["id"] = $_REQUEST["n3_id"];
$_SESSION["dbt_cache"]["n3_kidy"] = $speakers["n3"]["kidy"] = $_REQUEST["n3_kidy"];
$speakers["n3"]["tym"] = "neg";
$_SESSION["dbt_cache"]["vysledek"] = $vysledek = $_REQUEST["vysledek"];

$vitez = substr($vysledek,0,1) == "A" ? 1 : 0;
$presvedcive = substr($vysledek,1,1) == "+" ? 1 : 0;

$datum = deformat_date($datum);

// sanity tests
if (!isset($_REQUEST["delete"])) {
	foreach ($speakers as $key => $speaker) {
		if (cpdb_fetch_one_value("select clovek_ID from clovek_debata where clovek_ID = :clovek_id and debata_ID = :debata_id", array(":clovek_id"=>$speaker["id"], ":debata_id"=>$debata_id)) != $speaker["id"]) {
		// the debater wasn't in the debate before
		
			// check for active debater status
			if (cpdb_fetch_one_value("select debater from clovek where clovek_ID = :clovek_id", array(":clovek_id"=>$speaker["id"])) != 1) {
				pg_achtung(sprintf($lang["speaker %s is not an active debater, dropping"],$key), 1);
				$speaker[$key]["id"] = 0;
			}
			
			// check for active team membership
			if (cpdb_fetch_one_value("select aktivni from clovek_tym where clovek_ID = :clovek_id and tym_ID = :tym_id",array(":clovek_id"=>$speaker["id"], ":tym_id"=>$teams[$speaker["tym"]]["id"])) != 1) {
				pg_achtung(sprintf($lang["speaker %s is not active member of %s team, dropping"],$key, $lang[$speaker["tym"]]), 1);
				$speaker[$key]["id"] = 0;
			}
		}
	}
	
	// check tournament belongs to the competitions
	if (($turnaj_id != 0) && ($turnaj_soutez = cpdb_fetch_one_value("select soutez_ID from turnaj where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id))) != $soutez_id) {
		pg_achtung($lang["tournament does not match competition, changing competition"],1);
		$soutez_id = $turnaj_soutez;
	}

	// check resolution is in the right language
	if (cpdb_fetch_one_value("select teze.jazyk = soutez.jazyk from soutez, teze where soutez.soutez_ID = :soutez_id and teze.teze_ID = :teze_id", array(":soutez_id"=>$soutez_id, ":teze_id"=>$teze_id)) != 1) {
		pg_achtung($lang["resolution language does not match competition language"]);
		return;
	}
	
	// check the competition is active
	if (cpdb_fetch_one_value("select zamceno from soutez where soutez_ID = :soutez_id", array(":soutez_id"=>$soutez_id)) == 1) {
		pg_achtung($lang["selected competition is locked"]);
		return;
	}
}

// switch turnaj_ID: 0 => NULL
if ($turnaj_id == 0) $turnaj_id = NULL;

if (isset($_REQUEST["add"])) {
	// add debate
	$status = true;
	cpdb_transaction();
	
	// add main record
	$status &= cpdb_exec("insert into debata (soutez_ID, turnaj_ID, teze_ID, datum, misto, komentar, vitez, presvedcive) values (:soutez_id, :turnaj_id, :teze_id, :datum, :misto, :komentar, :vitez, :presvedcive)", array(":soutez_id"=>$soutez_id, ":turnaj_id"=>$turnaj_id, ":teze_id"=>$teze_id, ":datum"=>$datum, ":misto"=>$misto, ":komentar"=>$komentar, ":vitez"=>$vitez, ":presvedcive"=>$presvedcive));
	
	$_REQUEST["debata_id"] = $debata_id = cpdb_insert_id();
	
	// add teams
	foreach ($teams as $team) {
		$status &= cpdb_exec("insert into debata_tym (debata_ID, tym_ID, pozice) values (:debata_id, :tym_id, :pozice)", array(":debata_id"=>$debata_id, ":tym_id"=>$team["id"], ":pozice"=>$team["pozice"]));
	}
	
	// add speakers
	foreach ($speakers as $role => $speaker) {
		if ($speaker["id"]) // if speaker wasn't set to null
		$status &= cpdb_exec("insert into clovek_debata (debata_ID, clovek_ID, role, kidy) values (:debata_id, :clovek_id, :role, :kidy)", array(":debata_id"=>$debata_id, ":clovek_id"=>$speaker["id"], ":kidy"=>$speaker["kidy"], ":role"=>$role));
	}
	
	if ($status) {
		cpdb_commit();
		recount_debate($debata_id);
		ipts_recount_debate($debata_id);
		pg_achtung($lang["debate add ok"]);
		include("debata.inc");
	} else {
		cpdb_rollback();
		include("debata.edit.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update debate
	$status = true;
	cpdb_transaction();
	
	// update main record
	$status &= cpdb_exec("update debata set soutez_ID = :soutez_id, turnaj_ID = :turnaj_id, teze_ID = :teze_id, datum = :datum, misto = :misto, komentar = :komentar, vitez = :vitez, presvedcive = :presvedcive where debata_ID = :debata_id", array(":soutez_id"=>$soutez_id, ":turnaj_id"=>$turnaj_id, ":teze_id"=>$teze_id, ":datum"=>$datum, ":misto"=>$misto, ":komentar"=>$komentar, ":vitez"=>$vitez, ":presvedcive"=>$presvedcive, ":debata_id"=>$debata_id));
	
	// delete teams
	$status &= cpdb_exec("delete from debata_tym where debata_ID = :debata_id", array(":debata_id"=>$debata_id));
	
	// reinsert teams
	foreach ($teams as $team) {
		$status &= cpdb_exec("insert into debata_tym (debata_ID, tym_ID, pozice) values (:debata_id, :tym_id, :pozice)", array(":debata_id"=>$debata_id, ":tym_id"=>$team["id"], ":pozice"=>$team["pozice"]));
	}
	
	// delete speakers
	$status &= cpdb_exec("delete from clovek_debata where debata_ID = :debata_id and find_in_set(role,'a1,a2,a3,n1,n2,n3')", array(":debata_id"=>$debata_id));
	
	// reinsert speakers
	foreach ($speakers as $role => $speaker) {
		if ($speaker["id"]) // if speaker wasn't set to null
		$status &= cpdb_exec("insert into clovek_debata (debata_ID, clovek_ID, role, kidy) values (:debata_id, :clovek_id, :role, :kidy)", array(":debata_id"=>$debata_id, ":clovek_id"=>$speaker["id"], ":kidy"=>$speaker["kidy"], ":role"=>$role));
	}
		
	if ($status) {
		cpdb_commit();
		recount_debate($debata_id);
		ipts_recount_debate($debata_id);
		pg_achtung($lang["debate update ok"]);
		include("debata.edit.inc");
	} else {
		cpdb_rollback();
		return;
	}
	
	
} elseif (isset($_REQUEST["delete"])) {
	// delete debate
	if (cpdb_exec("delete from debata where debata_ID = :debata_id", array(":debata_id"=>$debata_id))) {
		pg_achtung($lang["debate deleted ok"]);
		include("debaty.inc");
	}
}

?>
