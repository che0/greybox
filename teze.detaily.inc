<?php
/*
 * greybox
 * $Id: teze.detaily.inc,v 1.22 2007/03/05 15:38:24 che0 Exp $
 */

$teze_id = $_REQUEST["teze_id"];

if (!cpdb_fetch_one_row("select * from teze where teze_ID = :teze_id", array(":teze_id" => $teze_id), $teze_p)) {
	pg_achtung($lang["no record"]);
	return;
}
$teze = de_html_s($teze_p);
$GLOBALS["page"]->title = substr($teze["tx"],0,56);
pg_head();

if (isset($teze["komentar"])) cph_p($teze["komentar"], array("class"=>"komentar"));

if ($GLOBALS['cps_teze'] >= 1) {
	$teze_f = new cpykhen;
	$teze_f->form_attr["action"] = "./?page=teze.edit.exec";
	$teze_f->form[] = new cpf_db_hidden("teze_id","teze_ID");
	$teze_f->form[] = new cpf_text($lang["language"] . ": ");
	$teze_f->form[] = new cpf_db_listbox_array("jazyk",array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"));
	$teze_f->form[] = new cpf_nl;
	$teze_f->form[] = new cpf_text($lang["resolution"] . ": ");
	$teze_f->form[] = new cpf_db_textbox("tx","tx",array("size"=>60,"maxlength"=>255));
	$teze_f->form[] = new cpf_nl;
	$teze_f->form[] = new cpf_text($lang["short text"] . ": ");
	$teze_f->form[] = new cpf_db_textbox("tx_short","tx_short",array("size"=>40,"maxlength"=>40));
	$teze_f->form[] = new cpf_nl;
	$teze_f->form[] = new cpf_text($lang["comment"] . ": ");
	$teze_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$teze_f->form[] = new cpf_nl;
	$teze_f->form[] = new cpf_button("save",$lang["save"]);
	$teze_f->form[] = new cpf_button("delete",$lang["delete"]);
	$teze_f->load_this($teze_p);
	$teze_f->render();
} else {
	cph_p($lang["language"] . ": " . $teze["jazyk"] 
		. cphs_br() . $lang["resolution"] . ": " . $teze["tx"]
		. cphs_br() . $lang["short text"] . ": " . $teze["tx_short"]);
}

if (cpdb_fetch_all("select soutez.soutez_ID as soutez, rocnik, nazev from soutez_teze left join soutez using (soutez_ID) where teze_ID = :teze_id", array(":teze_id" => $teze_id), $souteze) > 0) {
	de_html($souteze);
	cph_tag("h2",array(),$lang["official in"]);
	cph_table_head(array($lang["season"], $lang["competition"],""));
	foreach ($souteze as $soutez) {
		cph_table_row(array(season_to_str($soutez["rocnik"]), $soutez["nazev"], cphs_link("./?page=soutez&soutez_id=" .  $soutez["soutez"],$lang["details"])));
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
	where
		debata.teze_ID = :teze_id
	order by
		datum desc
	", array(":teze_id"=>$teze_id),$r_debaty)) {

	de_html($r_debaty);
	cph_h2($lang["debated"]);
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["result"],""));
	foreach ($r_debaty as $debata) {
		cph_table_row(array(
			format_date($debata["datum"]),
			cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
			cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
			result_to_str($debata["vitez"], $debata["presvedcive"]),
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["details"])
		));
	}
	cph_table_end();
}

// average result aff:neg
cpdb_fetch_one_row("
	select
		avg(if (vitez != 2, 2 * vitez + vitez * presvedcive + (1-vitez) * (1-presvedcive), 2)) as aff,
		avg(if (vitez != 2, 2 * (1-vitez) + (1-vitez) * presvedcive + vitez * (1-presvedcive), 2)) as neg
	from
		debata
	where
		teze_ID = :teze_id
	", array(":teze_id"=>$teze_id), $r_vysledky);
cph_p(sprintf($lang["Average ballots (aff:neg): %s:%s"], $r_vysledky["aff"], $r_vysledky["neg"]));


?>
