<?php
/*
 * greybox
 * $Id: soutez.inc,v 1.18 2005/01/09 00:53:08 che0 Exp $
 * vypis informaci o soutezi
 */

$soutez_id = $_REQUEST["soutez_id"];

if (!cpdb_fetch_one_row("select * from soutez where soutez_ID = :soutez_id", array(":soutez_id" => $soutez_id),$soutez_p)) {
	pg_achtung($lang["no record"]);
	return;
}

$soutez = de_html_s($soutez_p);
$GLOBALS["page"]->title = $soutez["nazev"];
pg_head();
if (isset($soutez["komentar"])) cph_p($soutez["komentar"]);

if ($GLOBALS["cps_souteze"] >= 2) {
	// edit/delete competition
	
	$soutez_f = new cpykhen;
	$soutez_f->form_attr["action"] = "./?page=soutez.edit.exec";
	$soutez_f->form[] = new cpf_db_hidden("soutez_id","soutez_ID");
	$soutez_f->form[] = new cpf_text($lang["competition"] . ": ");
	$soutez_f->form[] = new cpf_db_textbox("nazev","nazev",array("size"=>"40"));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["language"] . ": ");
	$soutez_f->form[] = new cpf_db_listbox_array("jazyk",array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["season"] . ": ");
	$soutez_f->form[] = new cpf_db_listbox_array("rocnik",$GLOBALS["season_domain"]);
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["comment"] . ": ");
	$soutez_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["active"] . ": ");
	$soutez_f->form[] = new cpf_db_listbox_array("zamceno",array(0=>$lang["active"], 1=>$lang["locked"]));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_button("save",$lang["save"]);
	$soutez_f->form[] = new cpf_button("delete",$lang["delete"]);
	$soutez_f->load_this($soutez_p);
	$soutez_f->render();
	
	$soutez_teze = new cpykhen;
	$soutez_teze->form_attr["action"] = "./?page=soutez.teze.edit.exec";
	$soutez_teze->select = "select * from soutez_teze where soutez_ID = :soutez_id";
	$soutez_teze->select_args = array(":soutez_id"=>$soutez_id);
	$soutez_teze->form["sid"] = new cpf_db_hidden("soutez_id","soutez_ID");
	$soutez_teze->form[] = new cpf_db_hidden("st_id","st_ID");
	$soutez_teze->form[] = new cpf_text($lang["official resolution"] . ": ");
	$soutez_teze->form[] = new cpf_db_listbox("teze_id",
		array("select teze_ID, tx from teze where jazyk = :jazyk", array(":jazyk"=>$soutez["jazyk"])),
		"teze_ID");
	$soutez_teze->form["link"] = new cpf_db_link("teze_ID","./?page=teze.detaily&teze_id=%s",$lang["link"]);
	$soutez_teze->form["b1"] = new cpf_button("save",$lang["save"]);
	$soutez_teze->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$soutez_teze->render_select();
	
	$soutez_teze->form["sid"] = new cpf_hidden("soutez_id",$soutez["soutez_ID"]);
	$soutez_teze->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($soutez_teze->form["b2"]);
	unset($soutez_teze->form["link"]);
	$soutez_teze->load_new();
	$soutez_teze->render_hidden($lang["new official resolution"]);
	
} else {
	// just view
	
	cph_p($lang["language"] . ": " . $soutez["jazyk"] . cphs_br()
		. $lang["season"] . ": " . season_to_str($soutez["rocnik"]));
	
	if (cpdb_fetch_all("
			select 
				teze.tx,
				teze.teze_ID,
				count(debata.debata_ID) as debates
			from
				soutez_teze
				left join teze
					on soutez_teze.teze_ID = teze.teze_ID
				left join debata
					on soutez_teze.teze_ID = debata.teze_ID
					and soutez_teze.soutez_ID = debata.soutez_ID
			where
				soutez_teze.soutez_ID = :soutez_id
			group by
				teze.tx,
				teze.teze_ID
		",
		array(":soutez_id"=>$soutez_id), $r_teze)) {
		
		cph_table_head(array($lang["official resolution"],$lang["debates"]),array("class"=>"visible"));
		foreach ($r_teze as $teze) {
			cph_table_row(array(
				cphs_link("./?page=teze.detaily&teze_id=" . $teze["teze_ID"], $teze["tx"]),
				$teze["debates"]
			));
		}
		cph_table_end();
	}
	
}

// team rankings

if (cpdb_fetch_all("
	select 
		sum(body) as bodu, tym.tym_ID, tym.nazev, klub.kratky_nazev, klub.klub_ID
	from
		debata
		left join debata_tym on debata.debata_ID = debata_tym.debata_ID
		left join tym on debata_tym.tym_ID = tym.tym_ID
		left join klub on tym.klub_ID = klub.klub_ID
	where
		debata.soutez_ID = :soutez_id
	group by
		tym.tym_ID, tym.nazev, klub.kratky_nazev, klub.klub_ID
	order by
		bodu desc
	",
	array(":soutez_id"=>$soutez_id),$ranks)) {
	
	cph_p(cphs_link("./?page=soutez.kidy&soutez_id=" . $soutez_id, $lang["kidy overview"]));
	
	de_html($ranks);
	cph_h2($lang["team rankings"]);
	
	cph_p($lang["please note that the points and rankings are computed according to the debate cup system, which may not necessarily be identical to the one used at the actual competition"],array("class"=>"note"));
	
	cph_table_head(array($lang["rank"],$lang["team"],$lang["club"],$lang["pts"]),array("class"=>"visible"));
	$count = 0; $rank = 1; $pts = -1;
	foreach ($ranks as $team_rank) {
		$count++;
		if ($pts != $team_rank["bodu"]) {
			$pts = $team_rank["bodu"];
			$rank = $count;
		}
		cph_table_row(array(
			$rank,
			cphs_link("./?page=tym&tym_id=" . $team_rank["tym_ID"], $team_rank["nazev"]),
			cphs_link("./?page=klub&klub_id=" . $team_rank["klub_ID"], $team_rank["kratky_nazev"]),
			cphs_link("./?page=tym.soutez&tym_id=" . $team_rank["tym_ID"] . "&soutez_id=" . $soutez_id, $team_rank["bodu"])
		));
	}
	cph_table_end();
}

// tournaments w/ place, time, no of debates, link
if (cpdb_fetch_all("select turnaj_ID, nazev, datum_od from turnaj where soutez_ID = :soutez_id order by datum_od", array(":soutez_id"=>$soutez_id),$r_turnaje)) {

	de_html($r_turnaje);
	cph_h2($lang["tournaments"]);	
	cph_table_head(array($lang["tournament"],$lang["date"],$lang["debates"]),array("class"=>"visible"));
	foreach ($r_turnaje as $turnaj) {
		cph_table_row(array(
			cphs_link("./?page=turnaj&turnaj_id=" . $turnaj["turnaj_ID"], $turnaj["nazev"]),
			format_date($turnaj["datum_od"]),
			cpdb_fetch_one_value("select count(*) from debata where turnaj_ID = :turnaj_id",array(":turnaj_id"=>$turnaj["turnaj_ID"]))
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
		teze.teze_ID as teze_ID,
		datum,
		vitez,
		presvedcive,
		debata.debata_ID
	from
		debata
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		debata.soutez_ID = :soutez_id
	order by
		datum desc
	", array(":soutez_id"=>$soutez_id),$r_debaty)) {
	
	de_html($r_debaty);
	cph_h2($lang["debates"]);
	
	if ($GLOBALS["cps_debaty"] >= 1) cph_p(cphs_link(sprintf("./?page=debata.add&soutez_id=%s",$soutez_id), $lang["add debate"]));
	
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],""),array("class"=>"visible"));
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
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze"]),
			$result,
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["details"])
		));
	}
	cph_table_end();
}

?>
