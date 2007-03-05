<?php
/*
 * greybox
 * $Id: points.inc,v 1.15 2007/03/05 15:32:00 che0 Exp $
 */

function iptsr_internal_deb($debata_id) {
	$rocnik = cpdb_fetch_one_value("select rocnik from debata left join soutez using (soutez_ID) where debata_ID = :debata_id", array(":debata_id"=>$debata_id));
	
	$status = true;
	cpdb_transaction();
	
	// vycistit
	$status &= cpdb_exec("delete from clovek_debata_ibody where debata_ID = :debata_id", array(":debata_id"=>$debata_id));
	
	// --- debateri
	// (podle poharovych bodu)
	$status &= cpdb_exec("insert into clovek_debata_ibody (clovek_ID, debata_ID, rocnik, role, ibody) select clovek_debata.clovek_ID, clovek_debata.debata_ID, :rocnik, 'debater', debata_tym.body * 0.1 from clovek_debata left join debata_tym on clovek_debata.debata_ID = debata_tym.debata_ID and substring(clovek_debata.role,1,1) = elt(debata_tym.pozice + 1,'n','a') where clovek_debata.debata_ID = :debata_id and find_in_set(clovek_debata.role,'a1,a2,a3,n1,n2,n3')", array(":debata_id"=>$debata_id, ":rocnik"=>$rocnik));
		
	// --- rozhodci
	$status &= cpdb_exec("insert into clovek_debata_ibody (clovek_ID, debata_ID, rocnik, role, ibody) select clovek_ID, debata_ID, :rocnik, 'rozhodci', 1 from clovek_debata where debata_ID = :debata_id and role = 'r'", array(":debata_id"=>$debata_id, ":rocnik"=>$rocnik));
	
	// --- treneri
	$treneri_dostanou = array();
	
	if (cpdb_fetch_all("select clovek_ID, ibody from clovek_debata_ibody where debata_ID = :debata_id and role = 'debater'", array(":debata_id"=>$debata_id), $debateri)) {
		foreach ($debateri as $debater) {
			$pocet_treneru = cpdb_fetch_all("
				select
					dk.clovek_ID as clovek_ID
				from
					clovek d, clovek_klub dk
				where
					d.clovek_ID = :clovek_ID
					and d.klub_ID = dk.klub_ID
					and dk.role = 't'
					and dk.rocnik = :rocnik
				", array(":clovek_ID"=>$debater["clovek_ID"], ":rocnik"=>$rocnik), $treneri);
			
			foreach ($treneri as $trener) {
				$treneri_dostanou[$trener["clovek_ID"]] += 0.1 * $debater["ibody"] / $pocet_treneru;
			}
		}
	}
	
	foreach ($treneri_dostanou as $trener_cid => $trener_ib) {
		$status &= cpdb_exec("insert into clovek_debata_ibody (clovek_ID, debata_ID, rocnik, role, ibody) values (:clovek_id, :debata_id, :rocnik, 'trener', :ibody)", array(":clovek_id"=>$trener_cid, ":debata_id"=>$debata_id, ":rocnik"=>$rocnik, ":ibody"=>$trener_ib));
	}
	
	// --- organizatori
	$dostanou = 0.15; // kolik IB je z debaty
	$zasluhy_primych = 1; // defaultni zasluhy u primych ogranizatoru
	$celkem_zasluhy = 0;
	$zasluhy = array();
	
	// primi
	cpdb_fetch_all(
		"select clovek_ID from clovek_debata where clovek_debata.role = 'o' and debata_ID = :debata_id", array(":debata_id"=>$debata_id), $res_primi);
	
	foreach ($res_primi as $org) {
		$celkem_zasluhy += $zasluhy_primych;
		$zasluhy[$org["clovek_ID"]] += $zasluhy_primych;
	}
	
	// neprimi
	cpdb_fetch_all("
		select
			clovek_turnaj.clovek_ID,
			clovek_turnaj.mocnost
		from
			debata, clovek_turnaj
		where
			debata.debata_ID = :debata_id
			and clovek_turnaj.turnaj_ID = debata.turnaj_ID
			and clovek_turnaj.role = 'o'
	", array(":debata_id"=>$debata_id), $res_neprimi);
	
	foreach($res_neprimi as $org) {
		$celkem_zasluhy += $org["mocnost"];
		$zasluhy[$org["clovek_ID"]] += $org["mocnost"];
	}
	
	foreach($zasluhy as $org_cid => $org_zasluhy) {
		$status &= cpdb_exec(
			"insert into clovek_debata_ibody (clovek_ID, debata_ID, rocnik, role, ibody) values (:clovek_id, :debata_id, :rocnik, 'organizator', :ibody)",
			array(
				":clovek_id" => $org_cid,
				":debata_id" => $debata_id,
				":rocnik" => $rocnik,
				":ibody" => $dostanou * $org_zasluhy / $celkem_zasluhy
			)
		);
	}
	
	// zaver
	if ($status) {
		cpdb_commit();
	} else {
		cpdb_rollback();
	}
}

function ipts_recount_clovek_role($clovek_id, $role) {
	// vycisti vsechny ibody dane role pro cloveka a spocita je znovu
	ipts_recount_clovek($clovek_id);
}

function ipts_recount_klub($klub_id) {
	// vycisti a prepocita vsechny debaty, kde jsou lidi z klubu
	cpdb_fetch_all("
		select distinct
			debata_ID
		from
			clovek
			left join clovek_debata using (clovek_ID)
		where
			clovek.klub_ID = :klub_id
	", array(":klub_id"=>$klub_id), $debaty);
	foreach ($debaty as $debata) {
		iptsr_internal_deb($debata["debata_ID"]);
	}
}

function ipts_recount_clovek($clovek_id) {
	// vycisti a prepocita vsechny ibody pro cloveka
	cpdb_fetch_all("
		select distinct
			debata_ID
		from
			clovek_debata
		where
			clovek_ID = :clovek_id
		union distinct
		select distinct
			clovek_ID
		from
			clovek_turnaj
			left join turnaj on clovek_turnaj.turnaj_ID = turnaj.turnaj_ID
			left join debata on turnaj.turnaj_ID = debata.turnaj_ID
		where
			clovek_turnaj.clovek_ID = :clovek_id
			and clovek_turnaj.role = 'o'
		union distinct
		select distinct
			debata_ID
		from
			clovek_debata_ibody
		where
			clovek_ID = :clovek_id
	", array(":clovek_id"=>$clovek_id), $debaty);
	foreach ($debaty as $debata) {
		iptsr_internal_deb($debata["debata_ID"]);
	}
}

function ipts_recount_debate($debata_id) {
	// vycisti a prepocita vsechny ibody pro debatu
	iptsr_internal_deb($debata_id);
}

function ipts_recount_turnaj($turnaj_id) {
	// vycisti a prepocita vsechny ibody pro turnaj
	cpdb_fetch_all("select debata_ID from debata where turnaj_ID = :turnaj_id",
		array(":turnaj_id"=>$turnaj_id), $debaty);
	
	foreach ($debaty as $debata) {
		iptsr_internal_deb($debata["debata_ID"]);
	}
}

function ipts_recount() {
	// vycisti a prepocita vsechny ibody
	cpdb_fetch_all("select debata_ID from debata", array(), $debaty);
	foreach ($debaty as $debata) {
		iptsr_internal_deb($debata["debata_ID"]);
	}
}

function recount_debate($debata_id) {
	// prepocita vysledky a body v debate a v pripadne lize
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
			$vitez = $debata["vitez"];
			$presvedcive = 0;
			pg_achtung(sprintf($lang["debate %s: tie votes, leaving default decision: %s"], $debata_id, result_to_str($vitez, $presvedcive)), 1);
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
	switch ($vitez) {
		case DRES_NEG:
			// neg won
			$points_aff = 5 - $presvedcive + $different_clubs + $official_resolution;
			$points_neg = 7 + $presvedcive + $different_clubs + $official_resolution;
		break;
		
		case DRES_AFF:
			// aff won
			$points_aff = 7 + $presvedcive + $different_clubs + $official_resolution;
			$points_neg = 5 - $presvedcive + $different_clubs + $official_resolution;
		break;
		
		case DRES_DRAW:
			// draw;
			$points_aff = 6 + $different_clubs + $official_resolution;
			$points_neg = 6 + $different_clubs + $official_resolution;
		break;
	}
	
	// write changes
	$status = true;
	cpdb_transaction();
	$status &= cpdb_exec("update debata set vitez = :vitez, presvedcive = :presvedcive where debata_ID = :debata_id", array(":debata_id"=>$debata_id, ":vitez"=>$vitez, ":presvedcive"=>$presvedcive));
	$status &= cpdb_exec("update debata_tym set body = :body where debata_ID = :debata_id and pozice = 1", array(":debata_id"=>$debata_id, ":body"=>$points_aff));
	$status &= cpdb_exec("update debata_tym set body = :body where debata_ID = :debata_id and pozice = 0", array(":debata_id"=>$debata_id, ":body"=>$points_neg));
	
	$liga_id = cpdb_fetch_one_value("select liga_ID from debata left join turnaj on turnaj.turnaj_ID = debata.turnaj_ID where debata_ID = :debata_id", array(":debata_id"=>$debata_id));
	if (isset($liga_id)) {
		lg_recount_league($liga_id);
	}
	
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

function lg_add_debate($liga_id, $debata_id) {
	// prida do tabulek ligy dalsi debatu
	// neresi transakce, vraci true/false
	
	$status = true;
	
	$status &= cpdb_fetch_one_row("
		select
			vitez,
			presvedcive,
			dt_aff.tym_ID as aff_ID,
			dt_neg.tym_ID as neg_ID,
			(
				select avg(liga_vytezek) / 2
				from debata_tym
				where tym_ID = dt_aff.tym_ID
					and debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID)
			) as aff_koef,
			(
				select avg(liga_vytezek) / 2
				from debata_tym
				where tym_ID = dt_neg.tym_ID
					and debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID)
			) as neg_koef
		from
			debata
			left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
			left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		where
			debata.debata_ID = :debata_id
	", array(":liga_id"=>$liga_id, ":debata_id"=>$debata_id), $debata);
	
	if ($debata["aff_koef"] === NULL) { $debata["aff_koef"] = 25; }
	if ($debata["neg_koef"] === NULL) { $debata["neg_koef"] = 25; }
	
	if ($debata["vitez"] == DRES_DRAW) {
		// draw debate in league --> illegal
		pg_achtung(sprintf($lang["debate %s ended up in draw, that is illegal in a league"], $debata_id));
		$status &= cpdb_exec("update debata_tym set liga_vytezek = 0 where debata_ID = :debata_id",  array(":debata_id"=>$debata_id));
		return $status;
	}
	
	// all ok, go by normal rules
	$rezultat_aff = ($debata["vitez"]) ? (40 + 10 * $debata["presvedcive"]) : (10 * (1 - $debata["presvedcive"]));
	$rezultat_neg = 50 - $rezultat_aff;
	
	// update vytezku
	$status &= cpdb_exec("update debata_tym set liga_vytezek = :vytezek where debata_ID = :debata_id and tym_ID = :tym_id", array(":debata_id"=>$debata_id, ":tym_id"=>$debata["aff_ID"], ":vytezek"=>$rezultat_aff + $debata["neg_koef"]));
	$status &= cpdb_exec("update debata_tym set liga_vytezek = :vytezek where debata_ID = :debata_id and tym_ID = :tym_id", array(":debata_id"=>$debata_id, ":tym_id"=>$debata["neg_ID"], ":vytezek"=>$rezultat_neg + $debata["aff_koef"]));
	
	return $status;
}

function lg_finalize($liga_id) {
	// pro kazdy tym dopocita skrtani vysledku a konecny tab
	// neresi transakce, vraci true/false
	
	$status = true;
	
	cpdb_fetch_all("select distinct tym_ID from debata_tym where debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID)", array(":liga_id"=>$liga_id), $r_tymy);
	
	foreach ($r_tymy as $tym) {
		$pocet_debat = cpdb_fetch_all("select debata_ID from debata_tym where tym_ID = :tym_id and debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID) order by liga_vytezek", array(":liga_id"=>$liga_id, ":tym_id"=>$tym["tym_ID"]), $r_debaty);
		
		if ($pocet_debat >= 20) { $skrtame = 4; }
		elseif ($pocet_debat >= 16) { $skrtame = 3; }
		elseif ($pocet_debat >= 12) { $skrtame = 2; }
		elseif ($pocet_debat >= 8) { $skrtame = 1; }
		else { $skrtame = 0; }
		
		$skrtnuto = array();
		for ($i=0; $i < $skrtame; $i++) {
			$skrtnuto[] = $r_debaty[$i]["debata_ID"];
		}
		
		$status &= cpdb_exec(sprintf("
			insert into liga_tym(liga_ID, tym_ID, liga_vytezek, skrtnute_debaty)
			values (
				:liga_id,
				:tym_id,
				(
					select avg(liga_vytezek)
					from debata_tym
					where tym_ID = :tym_id
						and debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID)
						%s
				),
				:skrtnute_debaty
			)
		", ($skrtame > 0) ? "and debata_ID not in (" . implode(",",$skrtnuto) . ")" : ""),
		array(":liga_id"=>$liga_id, ":tym_id"=>$tym["tym_ID"], ":skrtnute_debaty"=>implode(",",$skrtnuto)));
		
	} // konec iterace podle tymu
	
	return $status;
}

function lg_recount_league($liga_id) {
	// prepocita celou ligu
	
	$status = true;
	cpdb_transaction();
	
	// zrusit soucasne vysledky
	$status &= cpdb_exec("delete from liga_tym where liga_ID = :liga_id", array(":liga_id"=>$liga_id));
	$status &= cpdb_exec("update debata_tym set liga_vytezek = NULL where debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID) or debata_ID not in (select debata_ID from turnaj, debata where turnaj.liga_ID is not null and turnaj.turnaj_ID = debata.turnaj_ID)", array(":liga_id"=>$liga_id));
	// ^ tohle taky vycisti vytezky u neligovych debat
	
	// pridat postupne vysledky debat
	cpdb_fetch_all("select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and debata.turnaj_ID = turnaj.turnaj_ID order by debata.datum", array(":liga_id"=>$liga_id), $r_debaty);
	foreach($r_debaty as $debata) {
		$status &= lg_add_debate($liga_id, $debata["debata_ID"]);
	}
	
	// prepocitat skrtani vysledky a konecny tab
	$status &= lg_finalize($liga_id);
	
	if ($status) {
		cpdb_commit();
		return true;
	} else {
		pg_achtung(sprintf($lang["league %s: recount failed"],$liga_id));
		cpdb_rollback();
		return false;
	}
}

?>
