<?php
/*
 * greybox
 * $Id: turnaj.inc,v 1.26 2007/03/05 15:31:59 che0 Exp $
 */

include_once("i_turnaj.inc");

$turnaj_id = $_REQUEST["turnaj_id"];

if (!cpdb_fetch_one_row("select turnaj.*, soutez.nazev as soutez, liga.nazev as liga from turnaj left join soutez on turnaj.soutez_ID = soutez.soutez_ID left join liga on turnaj.liga_ID = liga.liga_ID where turnaj_ID = :turnaj_id", array(":turnaj_id" => $turnaj_id),$turnaj_p)) {
	pg_achtung($lang["no record"]);
	return;
}

$turnaj = de_html_s($turnaj_p);
$GLOBALS["page"]->title = $turnaj["nazev"];
pg_head();

if (isset($turnaj["komentar"])) cph_p($turnaj["komentar"], array("class"=>"komentar"));

if ($GLOBALS["cps_souteze"] >= 1) {
	// edit/delete competition
	
	$turnaj_f = get_pykhen_turnaj();
	$turnaj_f->form[] = new cpf_button("save",$lang["save"]);
	$turnaj_f->form[] = new cpf_button("delete",$lang["delete"]);
	$turnaj_f->load_this($turnaj_p);
	$turnaj_f->render();
	
	$turnaj_clovek = new cpykhen;
	$turnaj_clovek->form_attr["action"] = "./?page=turnaj.clovek.edit.exec";
	$turnaj_clovek->select = "select * from clovek_turnaj where turnaj_ID = :turnaj_id";
	$turnaj_clovek->select_args = array(":turnaj_id"=>$turnaj_id);
	$turnaj_clovek->form["tid"] = new cpf_db_hidden("turnaj_id","turnaj_ID");
	$turnaj_clovek->form["cid"] = new cpf_db_hidden("clovek_old","clovek_ID");
	$turnaj_clovek->form[] = new cpf_db_listbox_array("role", array("o"=>$lang["organizer"]));
	$turnaj_clovek->form[] = new cpf_db_listbox("clovek_id",
		array("select clovek_ID, concat_ws(' ', jmeno, nick, prijmeni) from clovek", array()),
		"clovek_ID");
	$turnaj_clovek->form["link"] = new cpf_db_link("clovek_ID","./?page=clovek&clovek_id=%s",$lang["link"]);
	$turnaj_clovek->form[] = new cpf_text(" " . $lang["factor"] . ": ");
	$turnaj_clovek->form[] = new cpf_db_textbox("mocnost","mocnost",array("size"=>3,"maxlength"=>3));
	$turnaj_clovek->form["b1"] = new cpf_button("save",$lang["save"]);
	$turnaj_clovek->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$turnaj_clovek->render_select();
	
	$turnaj_clovek->form["tid"] = new cpf_hidden("turnaj_id",$turnaj["turnaj_ID"]);
	unset($turnaj_clovek->form["cid"]);
	$turnaj_clovek->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($turnaj_clovek->form["b2"]);
	unset($turnaj_clovek->form["link"]);
	$turnaj_clovek->load_this(array("mocnost"=>1));
	$turnaj_clovek->render_hidden($lang["new person"]);
	
} else {
	// just view
	
	cph_p(
		$lang["date_from"] . ": " . format_date($turnaj["datum_od"]) . cphs_br()
		. $lang["date_to"] . ": " . format_date($turnaj["datum_do"]) . cphs_br()
		. $lang["competition"] . ": " . cphs_link("./?page=soutez&soutez_id=" . $turnaj["soutez_ID"], $turnaj["soutez"])
		. ((isset($turnaj["liga_ID"])) 
			? (cphs_br() . $lang["league"] . ": " . cphs_link("./?page=liga&liga_id=" . $turnaj["liga_ID"], $turnaj["liga"]))
			: ""
		)
	);
	
	$role_domain = array("o"=>$lang["organizer"]);
	if (cpdb_fetch_all("select role, mocnost, clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni from clovek_turnaj left join clovek using (clovek_ID) where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id), $r_clovek) > 0) {
		de_html($r_clovek);
		cph_table_head(array($lang["role"],$lang["person"],$lang["factor"]));
		foreach ($r_clovek as $clovek) {
			cph_table_row(array(
				$role_domain[$clovek["role"]],
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"], join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				$clovek["mocnost"]
			));
		}
		cph_table_end();
	}
	
}

// teams w/ points gained (+kidy)
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
	
	// if we have any team rankings, we also have kidy rankings
	cph_p(cphs_link("./?page=turnaj.kidy&turnaj_id=" . $turnaj_id, $lang["kidy overview"]));
	
	cph_h2($lang["teams"]);
	cph_p($lang["please note that the points and rankings are computed according to the debate cup system, which may not necessarily be identical to the one used at the actual tournament"],array("class"=>"note"));
	de_html($ranks);
	cph_table_head(array($lang["rank"], $lang["team"], $lang["club"], $lang["wins"], $lang["ballots"], $lang["pts"]));
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
		achtung,
		debata.debata_ID
	from
		debata
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		debata.turnaj_ID = :turnaj_id
	order by
		datum desc
	", array(":turnaj_id"=>$turnaj_id),$r_debaty)) {

	cph_h2($lang["debates"]);
	de_html($r_debaty);
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],""));
	foreach ($r_debaty as $debata) {
		cph_table_row(array(
			format_date($debata["datum"]),
			cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
			cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze_short"],array("title"=>$debata["teze"])),
			result_to_str($debata["vitez"], $debata["presvedcive"]),
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["details"])
		),
			$debata["achtung"] ? array("class"=>"achtung") : array()
		);
	}
	cph_table_end();
}

if ($GLOBALS["cps_debaty"] >= 1) cph_p(cphs_link(sprintf("./?page=debata.add&soutez_id=%s&turnaj_id=%s",$turnaj["soutez_ID"],$turnaj_id), $lang["add debate"]));


?>
