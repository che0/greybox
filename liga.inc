<?php
/*
 * greybox
 * $Id: liga.inc,v 1.1 2005/03/22 18:18:22 che0 Exp $
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
/* FIXME league ratings
if (cpdb_fetch_all("
	select
		tym.tym_ID,
		tym.nazev,
		klub.klub_ID,
		klub.kratky_nazev,
		sum(debata_tym.body) as bodu,
		sum((2+debata.presvedcive) * (debata_tym.pozice * debata.vitez + (1-debata_tym.pozice) * (1-debata.vitez)) + (1-debata.presvedcive) * ((1 - debata_tym.pozice) * debata.vitez + (1 - debata.vitez) * debata_tym.pozice)) as ballotu,
		sum(debata_tym.pozice * debata.vitez + (1-debata_tym.pozice) * (1-debata.vitez)) as vitezstvi
	from
		debata_tym
		left join tym on tym.tym_ID = debata_tym.tym_ID
		left join klub on klub.klub_ID = tym.klub_ID
		left join debata on debata.debata_ID = debata_tym.debata_ID
	where
		debata.turnaj_ID = :turnaj_id
	group by
		tym.tym_ID, tym.nazev
	order by
		vitezstvi desc, ballotu desc, bodu desc
	", array(":turnaj_id"=>$turnaj_id), $ranks)) {
	
	cph_h2($lang["teams"]);
	de_html($ranks);
	cph_table_head(array($lang["rank"], $lang["team"], $lang["club"], $lang["wins"], $lang["ballots"], $lang["pts"]), array("class"=>"visible"));
	$count = 0; $rank = 1; $pts = -1;
	foreach ($ranks as $team_rank) {
		$count++;
		if ($pts != $team_rank["vitezstvi"] * 1024 + $team_rank["ballotu"]) {
			$pts = $team_rank["vitezstvi"] * 1024 + $team_rank["ballotu"];
			$rank = $count;
		}
		cph_table_row(array(
			$rank,
			cphs_link("./?page=tym&tym_id=" . $team_rank["tym_ID"], $team_rank["nazev"]),
			cphs_link("./?page=klub&klub_id=" . $team_rank["klub_ID"], $team_rank["kratky_nazev"]),
			$team_rank["vitezstvi"],
			$team_rank["ballotu"],
			$team_rank["bodu"]
		));
	}
	cph_table_end();
}
*/

// tournaments
// FIXME

?>
