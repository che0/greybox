<?php
// soutez.detaily.inc
// vypis vsech soutezi v sezone

$soutez_id = $_REQUEST["soutez_id"];

cpdb_fetch_one_row("select * from soutez where soutez_ID = :soutez_id", array(":soutez_id" => $soutez_id),$soutez);

$GLOBALS["page"]->title = $soutez["nazev"];
pg_head();

if ($GLOBALS["cps_souteze"] >= 2) {
	// edit/delete competition
	
	$soutez_f = new cpykhen;
	$soutez_f->form_attr["action"] = "./?page=soutez.edit.exec";
	$soutez_f->form[] = new cpf_db_hidden("soutez_id","soutez_ID");
	$soutez_f->form[] = new cpf_text($lang["title"] . ": ");
	$soutez_f->form[] = new cpf_db_textbox("nazev");
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["language"] . ": ");
	$soutez_f->form[] = new cpf_db_listbox_array("jazyk",array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["comment"] . ": ");
	$soutez_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_text($lang["active"] . ": ");
	$soutez_f->form[] = new cpf_db_listbox_array("zamceno",array(0=>$lang["active"], 1=>$lang["locked"]));
	$soutez_f->form[] = new cpf_nl;
	$soutez_f->form[] = new cpf_button("save",$lang["save"]);
	$soutez_f->form[] = new cpf_button("delete",$lang["delete"]);
	$soutez_f->load_this($soutez);
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
	$soutez_teze->form["b1"] = new cpf_button("save",$lang["save"]);
	$soutez_teze->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$soutez_teze->render_select();
	
	$soutez_teze->form["sid"] = new cpf_hidden("soutez_id",$soutez["soutez_ID"]);
	$soutez_teze->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($soutez_teze->form["b2"]);
	$soutez_teze->load_new();
	$soutez_teze->render_hidden($lang["new official resolution"]);
	
} else {
	// just view
	
	if (isset($soutez["komentar"])) cph_p($soutez["komentar"]);
	cph_p($lang["language"] . ": " . $soutez["jazyk"]);
	
	cpdb_fetch_all("select tx, teze.teze_ID from soutez_teze left join teze using (teze_ID) where soutez_ID = :soutez_id", array(":soutez_id"=>$soutez_id), $r_teze);
	
	cph_table_head(array($lang["official resolution"]),array("class"=>"visible"));
	foreach ($r_teze as $teze) {
		cph_table_row(array(cphs_link("./?page=teze.detaily&teze_id=" . $teze["teze_ID"], $teze["tx"])));
	}
	cph_table_end();
	
}

// team rankings

cph_h2($lang["team rankings"]);

cpdb_fetch_all("
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
	array(":soutez_id"=>$soutez_id),$ranks);
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
		cphs_link("./?page=tym.detaily&tym_id=" . $team_rank["tym_ID"], $team_rank["nazev"]),
		cphs_link("./?page=klub&klub_id=" . $team_rank["klub_ID"], $team_rank["kratky_nazev"]),
		cphs_link("./?page=tym.soutez&tym_id=" . $team_rank["tym_ID"] . "&soutez_id=" . $soutez_id, $team_rank["bodu"])
	));
}
cph_table_end();

// tournaments w/ place, time, no of debates, link
cph_h2($lang["tournaments"]);

cpdb_fetch_all("select turnaj_ID, nazev, datum_od from turnaj where soutez_ID = :soutez_id order by datum_od", array(":soutez_id"=>$soutez_id),$r_turnaje);
cph_table_head(array($lang["tournament"],$lang["date"],$lang["debates"]),array("class"=>"visible"));
foreach ($r_turnaje as $turnaj) {
	cph_table_row(array(
		cphs_link("./?page=turnaj.detaily&turnaj_id=" . $team_rank["turnaj_ID"], $turnaj["nazev"]),
		format_date($turnaj["datum_od"]),
		cpdb_fetch_one_value("select count(*) from debata where turnaj_ID = :turnaj_id",array(":turnaj_id"=>$turnaj["turnaj_ID"]))
	));
}
cph_table_end();



// debates w/ teams, resolutions, result, link
cph_h2($lang["debates"]);

cpdb_fetch_all("
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
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		dt_aff.pozice = 1
		and dt_neg.pozice = 0
		and debata.soutez_ID = :soutez_id
	order by
		datum desc
	", array(":soutez_id"=>$soutez_id),$r_debaty);
cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],""),array("class"=>"visible"));
foreach ($r_debaty as $debata) {
	if ($debata["vitez"] == 1) {
		$result = $lang["aff"];
	} else {
		$result = $lang["neg"];
	}
	$result .= " ";
	if ($debata["presvedcive"] == 1) {
		$result .= $lang["persuasively"];
	} else {
		$result .= $lang["less persuasively"];
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

?>