<?php

include_once("lib/points.inc");

if ($GLOBALS["cps_debaty"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

$dc_id = $_REQUEST["dc_id"];
$debata_id = $_REQUEST["debata_id"];
$clovek_id = $_REQUEST["clovek_id"];
$role = $_REQUEST["role"];
$rozhodnuti = $_REQUEST["rozhodnuti"] - 1;
$presvedcive = $_REQUEST["presvedcive"] - 1;

if (!isset($_REQUEST["delete"])) switch ($role) {
	case "r": // adjudicator
		// accreditation check
		if (cpdb_fetch_one_value("select rozhodci.clovek_ID from debata left join soutez on debata.soutez_ID = soutez.soutez_ID left join rozhodci on soutez.rocnik = rozhodci.rocnik and find_in_set(soutez.jazyk, rozhodci.jazyk) where debata.debata_ID = :debata_id and rozhodci.clovek_ID = :clovek_id",array(":debata_id"=>$debata_id, ":clovek_id"=>$clovek_id)) != $clovek_id) {
			pg_achtung($lang["adjudicator does not have accreditation for competition language or season"]);
			return;
		}
		
		if ($rozhodnuti == -1) {
			pg_achtung($lang["refusing to set adjudicator without a decision"]);
			return;
		}
		if ($presvedcive == -1) {
			pg_achtung($lang["persuasiveness not set, falling to default 3:0"],1);
			$presvedcive = 1;
		}
	break;
	
	case "o": // organizer
		if ($rozhodnuti + $presvedcive != -2) {
			pg_achtung($lang["organizer cannot have a decision set, resetting to N/A"],1);
			$rozhodnuti = $presvedcive = -1;
		}
	break;
}

if ($rozhodnuti == -1) { $rozhodnuti = NULL; }
if ($presvedcive == -1) { $presvedcive = NULL; }

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_debata (clovek_ID, debata_ID, role, rozhodnuti, presvedcive) values (:clovek_id, :debata_id, :role, :rozhodnuti, :presvedcive)", array(":clovek_id"=>$clovek_id, ":debata_id"=>$debata_id, ":role"=>$role, ":rozhodnuti"=>$rozhodnuti, ":presvedcive"=>$presvedcive))) {
		recount_debate($debata_id);
		ipts_recount_debate($debata_id);
		pg_achtung($lang["debate update ok"]);
		include("debata.edit.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_debata set clovek_ID = :clovek_id, role = :role, rozhodnuti = :rozhodnuti, presvedcive = :presvedcive where debata_ID = :debata_id and dc_ID = :dc_id", array(":dc_id"=>$dc_id, ":clovek_id"=>$clovek_id, ":debata_id"=>$debata_id, ":role"=>$role, ":rozhodnuti"=>$rozhodnuti, ":presvedcive"=>$presvedcive))) {
		recount_debate($debata_id);
		ipts_recount_debate($debata_id);
		pg_achtung($lang["debate update ok"]);
		include("debata.edit.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_debata where dc_ID = :dc_id and debata_ID = :debata_id", array(":dc_id"=>$dc_id, ":debata_id"=>$debata_id))) {
		recount_debate($debata_id);
		ipts_recount_debate($debata_id);
		pg_achtung($lang["debate update ok"]);
		include("debata.edit.inc");
	}
}

?>
