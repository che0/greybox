<?php

function ipts_recount_clovek_role($clovek_id, $role) {
	// vycisti vsechny ibody dane role pro cloveka a spocita je znovu
	// FIXME
}

function ipts_recount_clovek($clovek_id) {
	// vycisti a prepocita vsechny ibody pro cloveka
	// FIXME
}

function ipts_recount_debate($debata_id) {
	// vycisti a prepocita vsechny ibody pro debatu
	// FIXME
}

function ipts_recount() {
	// vycisti a prepocita vsechny ibody
	// FIXME
}

function recount_debate($debata_id) {
	// prepocita vysledky a body v debate
	global $lang; // for $lang
	
	cpdb_fetch_one_row("select teze_ID, vitez, presvedcive from debata where debata_ID = :debata_id",array(":debata_id"=>$debata_id), $debata);
	
	$n_roz = cpdb_fetch_all("select rozhodnuti, presvedcive from clovek_debata where debata_ID = :debata_id and role = 'r'", array(":debata_id"=>$debata_id), $r_roz);
	
	// result
	if ($n_roz == 0) {
		// no entered adjudicator
		$vitez = $debata["vitez"];
		$presvedcive = $debata["presvedcive"];
	} elseif ($n_roz == 1) {
		// one adjudicator
		$vitez = $r_roz[0]["rozhodnuti"];
		$presvedcive = $r_roz[0]["presvedcive"];
	} else {
		// 2+ adjudicators
		$pro_aff = $pro_neg = 0;
		foreach($r_roz as $roz) {
			if ($roz["rozhodnuti"]) {
				$pro_aff++;
			} else {
				$pro_neg++;
			}
		}
		if ($pro_aff == $pro_neg) {
			// tie
			pg_achtung(sprintf($lang["debate %s: tie votes, leaving default decision: %s"],$debata_id, $debata["vitez"] ? $lang["aff"] : $lang["neg"]), 1);
			$vitez = $debata["vitez"];
			$presvedcive = 0;
		} elseif ($pro_aff > $pro_neg) {
			// aff win
			$vitez = 1;
			$presvedcive = ($pro_neg == 0 ? 1 : 0);
		} else {
			// neg win
			$vitez = 0;
			$presvedcive = ($pro_aff == 0 ? 1 : 0);
		}
	}
	
	// different clubs
	$different_clubs = cpdb_fetch_one_value("
		select
			aff.klub_ID != neg.klub_ID
		from 
			debata
			left join debata_tym aff_dt
				on aff_dt.debata_ID = debata.debata_ID
				and aff_dt.pozice = 1
			left join debata_tym neg_dt
				on neg_dt.debata_ID = debata.debata_ID
				and neg_dt.pozice = 0
			left join tym aff
				on aff.tym_ID = aff_dt.tym_ID
			left join tym neg
				on neg.tym_ID = neg_dt.tym_ID
		where
			debata.debata_ID = :debata_id
		", array(":debata_id"=>$debata_id));
	
	// official resolution
	$official_resolution = cpdb_fetch_one_value("
		select
			soutez_teze.teze_ID <=> debata.teze_ID
		from
			debata
			left join soutez_teze
				on debata.soutez_ID = soutez_teze.soutez_ID
				and debata.teze_ID = soutez_teze.teze_ID
		where
			debata.debata_ID = :debata_id
		", array(":debata_id"=>$debata_id));
	
	// count points
	if ($vitez) {
		// aff won
		$points_aff = 7 + $presvedcive + $different_clubs + $official_resolution;
		$points_neg = 5 - $presvedcive + $different_clubs + $official_resolution;
	} else {
		// neg won
		$points_aff = 5 - $presvedcive + $different_clubs + $official_resolution;
		$points_neg = 7 + $presvedcive + $different_clubs + $official_resolution;
	}
	
	// write changes
	$status = true;
	cpdb_transaction();
	$status &= cpdb_exec("update debata set vitez = :vitez, presvedcive = :presvedcive where debata_ID = :debata_id", array(":debata_id"=>$debata_id, ":vitez"=>$vitez, ":presvedcive"=>$presvedcive));
	$status &= cpdb_exec("update debata_tym set body = :body where debata_ID = :debata_id and pozice = 1", array(":debata_id"=>$debata_id, ":body"=>$points_aff));
	$status &= cpdb_exec("update debata_tym set body = :body where debata_ID = :debata_id and pozice = 0", array(":debata_id"=>$debata_id, ":body"=>$points_neg));
	if ($status) {
		cpdb_commit();
		return true;
	} else {
		pg_achtung(sprintf($lang["debate %s: recount failed"],$debata_id));
		cpdb_rollback();
		return false;
	}
}

function recount_competition($soutez_id) {
	// prepocita vysledky vsech debat v soutezi
	cpdb_fetch_all("select debata_ID from debata where soutez_ID = :soutez_id",array(":soutez_id"=>$soutez_id),$debaty);
	foreach ($debaty as $debata) {
		recount_debate($debata["debata_ID"]);
	}
}

function recount_team($tym_id) {
	// prepocita vysledky vsech debat zadaneho tymu
	cpdb_fetch_all("select debata_ID from debata_tym where tym_ID = :tym_id",array(":tym_id"=>$tym_id),$debaty);
	foreach ($debaty as $debata) {
		recount_debate($debata["debata_ID"]);
	}
}

function recount_all_debates() {
	// prepocita vysledky a body u vsech debat
	cpdb_fetch_all("select debata_ID from debata",array(),$debaty);
	foreach ($debaty as $debata) {
		recount_debate($debata["debata_ID"]);
	}
}

?>