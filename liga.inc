<?php
/*
 * greybox
 * $Id: liga.inc,v 1.3 2005/03/23 20:05:48 che0 Exp $
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
		liga_tym.liga_vytezek
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
	cph_table_head(array($lang["rank"], $lang["team"], $lang["club"], $lang["tab value"]));
	$count = 0; $rank = 1; $tab = -1;
	foreach ($ranks as $team_rank) {
		$count++;
		if ($tab != $team_rank["liga_vytezek"]) {
			$tab = $team_rank["liga_vytezek"];
			$rank = $count;
		}
		cph_table_row(array(
			$rank,
			cphs_link("./?page=tym&tym_id=" . $team_rank["tym_ID"], $team_rank["nazev"]),
			cphs_link("./?page=klub&klub_id=" . $team_rank["klub_ID"], $team_rank["kratky_nazev"]),
			cphs_link(sprintf("./?page=tym.liga&tym_id=%s&liga_id=%s",$team_rank["tym_ID"],$liga_id), $team_rank["liga_vytezek"])
		));
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


?>
