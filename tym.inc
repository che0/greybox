<?php
/*
 * greybox
 * $Id: tym.inc,v 1.15 2005/01/09 00:47:32 che0 Exp $
 */

$tym_id = $_REQUEST["tym_id"];

if (!cpdb_fetch_one_row("select * from tym where tym_ID = :tym_id",array(":tym_id"=>$tym_id), $tym_p)) {
	pg_achtung($lang["no record"]);
	return;
}
$tym = de_html_s($tym_p);

$GLOBALS["page"]->title = $tym["nazev"];
pg_head();

if (isset($tym["komentar"])) cph_p($tym["komentar"]);

if ($GLOBALS["cps_kluby"] >= 2
	|| ($GLOBALS["cps_kluby"] == 1 && $tym["klub_ID"] == $_SESSION["user_klub_ID"])
	|| (cpdb_fetch_one_value("select aktivni from clovek_tym where clovek_ID = :clovek_id and tym_ID = :tym_id", array(":clovek_id"=>$_SESSION["user_clovek_ID"], ":tym_id"=>$tym_id)) == 1)) {

	// may edit team	
	$tym_f = new cpykhen;
	$tym_f->form_attr["action"] = "./?page=tym.edit.exec";
	$tym_f->form[] = new cpf_db_hidden("tym_id","tym_ID");
	$tym_f->form[] = new cpf_text($lang["club"] . ": ");
	$tym_f->form[] = new cpf_db_listbox("klub_id",array("select klub_ID, nazev from klub", array()), "klub_ID");
	$tym_f->form[] = new cpf_db_link("klub_ID","./?page=klub&klub_id=%s","link");
	$tym_f->form[] = new cpf_nl;
	$tym_f->form[] = new cpf_text($lang["team name"] . ": ");
	$tym_f->form[] = new cpf_db_textbox("nazev","nazev",array("size"=>"40"));
	$tym_f->form[] = new cpf_nl;
	$tym_f->form[] = new cpf_text($lang["comment"] . ": ");
	$tym_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$tym_f->form[] = new cpf_nl;
	$tym_f->form[] = new cpf_button("save",$lang["save"]);
	$tym_f->form[] = new cpf_button("delete",$lang["delete"]);
	$tym_f->load_this($tym_p);
	$tym_f->render();
	
	$members = new cpykhen;
	$members->form_attr["action"] = "./?page=tym.clovek.edit.exec";
	$members->select = "select clovek_tym.ct_ID as ct_ID, clovek_tym.clovek_ID, clovek_tym.tym_ID, clovek_tym.aktivni, concat_ws(' ', clovek.jmeno, clovek.nick, clovek.prijmeni) as clovek from clovek_tym left join clovek using(clovek_ID) where tym_ID = :tym_id order by aktivni desc";
	$members->select_args = array(":tym_id"=>$tym_id);
	$members->form["tid"] = new cpf_db_hidden("tym_id","tym_ID");
	$members->form[] = new cpf_db_hidden("ct_id","ct_ID");
	$members->form["clovek"] = new cpf_db_text("clovek");
	$members->form["clovek2"] = new cpf_db_hidden("clovek_id","clovek_ID");
	$members->form["clovek3"] = new cpf_text(" ");
	$members->form["clovek4"] = new cpf_db_link("clovek_ID","./?page=clovek&clovek_id=%s","link");
	$members->form[] = new cpf_text(" ");
	$members->form[] = new cpf_db_listbox_array("aktivni",array(1=>$lang["active"], 0=>$lang["disabled"]));
	$members->form["b1"] = new cpf_button("save",$lang["save"]);
	$members->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$members->render_select();
	
	
	
	$members->form["tid"] = new cpf_hidden("tym_id",$tym_id);
	$members->form["clovek"] = new cpf_db_listbox("clovek_id",
		array("select clovek_ID, concat_ws(' ', jmeno, nick, prijmeni), concat_ws(' ', prijmeni, jmeno, nick) as srt from clovek where clovek.debater = 1 and clovek.klub_ID = :klub_id order by srt", array(":klub_id"=>$tym["klub_ID"])),
		"clovek_ID");
	$members->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($members->form["b2"], $members->form["clovek2"], $members->form["clovek3"], $members->form["clovek4"]);
	$members->init_force();
	$members->load_this(array("aktivni"=>1));
	$members->render_hidden($lang["add member from club"]);
	
	$members->form["clovek"] = new cpf_db_listbox("clovek_id",
		array("select clovek_ID, concat_ws(' ', jmeno, nick, prijmeni), concat_ws(' ', prijmeni, jmeno, nick) as srt from clovek where clovek.debater = 1 order by srt", array()),
		"clovek_ID");
	$members->init_force();
	$members->load_this(array("aktivni"=>1));
	$members->render_hidden($lang["add member"]);
	
} else {
	cph_p(cphs_link("./?page=klub&klub_id=" . $tym["klub_ID"], cpdb_fetch_one_value("select nazev from klub where klub_ID = :klub_id", array(":klub_id"=>$tym["klub_ID"]))));
	
	cpdb_fetch_all("select clovek_tym.clovek_ID, clovek_tym.aktivni, clovek.jmeno, clovek.nick, clovek.prijmeni from clovek_tym left join clovek using(clovek_ID) where tym_ID = :tym_id order by aktivni desc", array(":tym_id"=>$tym_id), $members);
	
	de_html($members);
	cph_h2($lang["members"]);
	cph_table_head(array($lang["status"],$lang["person"]),array("class"=>"visible"));
	foreach ($members as $member) {
		cph_table_row(array(
			$member["aktivni"] ? $lang["active"] : $lang["disabled"],
			cphs_link("./?page=clovek&clovek_id=" . $member["clovek_ID"], join_name($member["jmeno"], $member["nick"], $member["prijmeni"]))
		), $member["aktivni"] ? array() : array("class"=>"inactive"));
	}
	cph_table_end();
}


if (cpdb_fetch_all("
	select
		soutez.soutez_ID,
		soutez.nazev,
		sum(debata_tym.body) as body
	from
		debata
		left join soutez on debata.soutez_ID = soutez.soutez_ID
		left join debata_tym on debata_tym.debata_ID = debata.debata_ID
	where
		debata_tym.tym_ID = :tym_id
	group by
		soutez.soutez_ID,
		soutez.nazev
", array(":tym_id"=>$tym_id), $souteze)) {

	de_html($souteze);
	cph_h2($lang["competitions"]);
	cph_table_head(array($lang["competition"],$lang["pts"]),array("class"=>"visible"));
	foreach ($souteze as $soutez) {
		cph_table_row(array(
			cphs_link("./?page=soutez&soutez_id=" . $soutez["soutez_ID"], $soutez["nazev"]),
			cphs_link(sprintf("./?page=tym.soutez&soutez_id=%s&tym_id=%s",$soutez["soutez_ID"],$tym_id),$soutez["body"])
		));
	}
	cph_table_end();
}

if (cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		dt_aff.body as aff_body,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		dt_neg.body as neg_body,
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
		tym_aff.tym_ID = :tym_id or tym_neg.tym_ID = :tym_id
	order by
		datum desc
	", array(":tym_id"=>$tym_id),$r_debaty)) {

	de_html($r_debaty);
	cph_h2($lang["debates"]);
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],$lang["pts"],""),array("class"=>"visible"));
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
			( $debata["aff_ID"] == $tym_id ) ? $debata["aff_body"] : $debata["neg_body"],
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"])
		));
	}
	cph_table_end();
}

?>
