<?php
/*
 * greybox
 * $Id: liga.inc,v 1.5 2005/03/30 12:20:54 che0 Exp $
 */

include_once("i_liga.inc");

$liga_id = $_REQUEST["liga_id"];

if (!cpdb_fetch_one_row("select * from liga where liga_ID = :liga_id", array(":liga_id" => $liga_id),$liga_p)) {
	pg_achtung($lang["no record"]);
	return;
}

$liga = de_html_s($liga_p);
$GLOBALS["page"]->title = $liga["nazev"];
pg_head();

if (isset($liga["komentar"])) cph_p($liga["komentar"]);

if ($GLOBALS["cps_souteze"] >= 2) {
	// edit/delete league
	$liga_f = get_pykhen_liga();
	$liga_f->form[] = new cpf_button("save",$lang["save"]);
	$liga_f->form[] = new cpf_button("delete",$lang["delete"]);
	$liga_f->load_this($liga_p);
	$liga_f->render();
	
} else {
	// just view
	
	cph_p(
		$lang["season"] . ": " . season_to_str($liga["rocnik"])
	);
}

// teams w/ ratings
if (cpdb_fetch_all("
	select
		tym.tym_ID,
		tym.nazev,
		klub.klub_ID,
		klub.kratky_nazev,
		liga_tym.liga_vytezek,
		(
			select
				count(debata.debata_ID)
			from
				debata_tym
				inner join debata on debata_tym.debata_ID = debata.debata_ID
				inner join turnaj on debata.turnaj_ID = turnaj.turnaj_ID
			where
				debata_tym.tym_ID = tym.tym_ID -- vazba na vnejsi dotaz
				and turnaj.liga_ID = :liga_id
		) as pocet_debat
	from
		liga_tym
		left join tym on tym.tym_ID = liga_tym.tym_ID
		left join klub on klub.klub_ID = tym.klub_ID
	where
		liga_tym.liga_ID = :liga_id
	order by
		liga_vytezek desc
	", array(":liga_id"=>$liga_id), $ranks)) {
	
	cph_h2($lang["teams"]);
	de_html($ranks);
	cph_table_head(array($lang["rank"], $lang["rank"], $lang["team"], $lang["club"], $lang["tab value"], $lang["debates"]));
	$count = 0; $rank = 1; $tab = -1;
	$countF = 0; $rankF = 1; $ptsF = -1;
	foreach ($ranks as $team_rank) {
		$count++;
		if ($tab != $team_rank["liga_vytezek"]) {
			$tab = $team_rank["liga_vytezek"];
			$rank = $count;
		}
		$fullrank = ($team_rank["pocet_debat"] >= $GLOBALS["cp_config"]["min_league_debates"]);
		if ($fullrank) {
			$countF++;
			if ($ptsF != $team_rank["liga_vytezek"]) {
			$ptsF = $team_rank["liga_vytezek"];
			$rankF = $countF;
			}
		}
		cph_table_row(array(
			$fullrank ? $rankF : "",
			$rank,
			cphs_link("./?page=tym&tym_id=" . $team_rank["tym_ID"], $team_rank["nazev"]),
			cphs_link("./?page=klub&klub_id=" . $team_rank["klub_ID"], $team_rank["kratky_nazev"]),
			cphs_link(sprintf("./?page=tym.liga&tym_id=%s&liga_id=%s",$team_rank["tym_ID"],$liga_id), $team_rank["liga_vytezek"]),
			$team_rank["pocet_debat"]
		),
			$fullrank ? array() : array("class"=>"inactive")
		);
	}
	cph_table_end();
}

// tournaments
if (cpdb_fetch_all("select turnaj_ID, nazev, datum_od, (select count(*) from debata where turnaj_ID = turnaj.turnaj_ID) as debaty from turnaj where liga_ID = :liga_id order by datum_od", array(":liga_id"=>$liga_id),$r_turnaje)) {

	de_html($r_turnaje);
	cph_h2($lang["tournaments"]);
	cph_table_head(array($lang["tournament"],$lang["date"],$lang["debates"]));
	foreach ($r_turnaje as $turnaj) {
		cph_table_row(array(
			cphs_link("./?page=turnaj&turnaj_id=" . $turnaj["turnaj_ID"], $turnaj["nazev"]),
			format_date($turnaj["datum_od"]),
			$turnaj["debaty"]
		));
	}
	cph_table_end();
}

// debates w/ teams, resolutions, result, link
if (cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		teze.tx as teze,
		teze.tx_short as teze_short,
		teze.teze_ID as teze_ID,
		datum,
		vitez,
		presvedcive,
		debata.debata_ID
	from
		turnaj
		inner join debata on turnaj.turnaj_ID = debata.turnaj_ID
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		turnaj.liga_ID = :liga_id
	order by
		datum desc
	", array(":liga_id"=>$liga_id),$r_debaty)) {
	
	de_html($r_debaty);
	cph_h2($lang["debates"]);
	
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],""));
	foreach ($r_debaty as $debata) {
		if ($debata["vitez"] == 1) {
			$result = $lang["aff"];
		} else {
			$result = $lang["neg"];
		}
		$result .= " ";
		if ($debata["presvedcive"] == 1) {
			$result .= $lang["3:0"];
		} else {
			$result .= $lang["2:1"];
		}
		cph_table_row(array(
			format_date($debata["datum"]),
			cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
			cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze_short"],array("title"=>$debata["teze"])),
			$result,
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["details"])
		));
	}
	cph_table_end();
}

?>