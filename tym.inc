<?php
/*
 * greybox
 * $Id: tym.inc,v 1.26 2007/03/05 15:31:59 che0 Exp $
 */

$tym_id = $_REQUEST["tym_id"];

if (!cpdb_fetch_one_row("select * from tym where tym_ID = :tym_id",array(":tym_id"=>$tym_id), $tym_p)) {
	pg_achtung($lang["no record"]);
	return;
}
$tym = de_html_s($tym_p);

$GLOBALS["page"]->title = $tym["nazev"];
pg_head();

if (isset($tym["komentar"])) cph_p($tym["komentar"], array("class"=>"komentar"));

if ($GLOBALS["cps_kluby"] >= 2
	|| ($GLOBALS["cps_kluby"] == 1 && $tym["klub_ID"] == $_SESSION["user_klub_ID"])
	|| (cpdb_fetch_one_value("select aktivni from clovek_tym where clovek_ID = :clovek_id and tym_ID = :tym_id", array(":clovek_id"=>$_SESSION["user_clovek_ID"], ":tym_id"=>$tym_id)) == 1)) {

	// may edit team	
	$tym_f = new cpykhen;
	$tym_f->form_attr["action"] = "./?page=tym.edit.exec";
	$tym_f->form[] = new cpf_db_hidden("tym_id","tym_ID");
	$tym_f->form[] = new cpf_text($lang["team name"] . ": ");
	$tym_f->form[] = new cpf_db_textbox("nazev","nazev",array("size"=>40,"maxlength"=>255));
	$tym_f->form[] = new cpf_nl;
	$tym_f->form[] = new cpf_text($lang["club"] . ": ");
	$tym_f->form[] = new cpf_db_listbox("klub_id",array("select klub_ID, nazev from klub order by nazev", array()), "klub_ID");
	$tym_f->form[] = new cpf_db_link("klub_ID","./?page=klub&klub_id=%s","link");
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
	$members->select = "select clovek_tym.clovek_ID, clovek_tym.tym_ID, clovek_tym.aktivni, concat_ws(' ', clovek.jmeno, clovek.nick, clovek.prijmeni) as clovek from clovek_tym left join clovek using(clovek_ID) where tym_ID = :tym_id order by aktivni desc";
	$members->select_args = array(":tym_id"=>$tym_id);
	$members->form["tid"] = new cpf_db_hidden("tym_id","tym_ID");
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
	
	if (cpdb_fetch_all("select clovek_tym.clovek_ID, clovek_tym.aktivni, clovek.jmeno, clovek.nick, clovek.prijmeni from clovek_tym left join clovek using(clovek_ID) where tym_ID = :tym_id order by aktivni desc", array(":tym_id"=>$tym_id), $members)) {
		
		de_html($members);
		cph_h2($lang["members"]);
		cph_table_head(array($lang["status"],$lang["person"]));
		foreach ($members as $member) {
			cph_table_row(array(
				$member["aktivni"] ? $lang["active"] : $lang["disabled"],
				cphs_link("./?page=clovek&clovek_id=" . $member["clovek_ID"], join_name($member["jmeno"], $member["nick"], $member["prijmeni"]))
			), $member["aktivni"] ? array() : array("class"=>"inactive"));
		}
		cph_table_end();
	}
}

// competitions
if (cpdb_fetch_all("
	select
		liga.liga_ID as ID,
		liga.nazev,
		liga.rocnik,
		liga_tym.liga_vytezek as body,
		1 as liga
	from
		liga_tym
		left join liga using (liga_ID)
	where
		liga_tym.tym_ID = :tym_id
	union
	
	select
		soutez.soutez_ID as ID,
		soutez.nazev,
		soutez.rocnik,
		sum(debata_tym.body) as body,
		0 as liga
	from
		debata
		left join soutez on debata.soutez_ID = soutez.soutez_ID
		left join debata_tym on debata_tym.debata_ID = debata.debata_ID
	where
		debata_tym.tym_ID = :tym_id
	group by
		soutez.soutez_ID,
		soutez.nazev,
		soutez.rocnik

	order by
		rocnik desc,
		liga desc,
		nazev
", array(":tym_id"=>$tym_id), $souteze)) {

	de_html($souteze);
	cph_h2($lang["competitions"]);
	cph_table_head(array($lang["season"], $lang["competition"],$lang["pts"]."/".$lang["tab value"]));
	foreach ($souteze as $soutez) {
		if ($soutez["liga"] == 0) {
			// cup
			cph_table_row(array(
				season_to_str($soutez["rocnik"]),
				cphs_link("./?page=soutez&soutez_id=" . $soutez["ID"], $soutez["nazev"]),
				cphs_link(sprintf("./?page=tym.soutez&soutez_id=%s&tym_id=%s",$soutez["ID"],$tym_id),intval($soutez["body"]))
			));
		} else {
			// league
			cph_table_row(array(
				season_to_str($soutez["rocnik"]),
				cphs_link("./?page=liga&liga_id=" . $soutez["ID"], $soutez["nazev"]),
				cphs_link(sprintf("./?page=tym.liga&liga_id=%s&tym_id=%s",$soutez["ID"],$tym_id),$soutez["body"])
			));
		}
	}
	cph_table_end();
}

// debates
if (cpdb_fetch_all("
	select
		tym_oni.tym_ID as oni_ID,
		tym_oni.nazev as oni_tym,
		dt_my.pozice,
		dt_my.body,
		dt_my.liga_vytezek,
		teze.tx as teze,
		teze.tx_short as teze_short,
		teze.teze_ID as teze_ID,
		datum,
		vitez,
		presvedcive,
		achtung,
		debata.debata_ID
	from
		debata_tym dt_my
		left join debata on dt_my.debata_ID = debata.debata_ID
		left join debata_tym dt_oni on dt_oni.debata_ID = debata.debata_ID and dt_oni.pozice != dt_my.pozice
		left join tym tym_oni on dt_oni.tym_ID = tym_oni.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		dt_my.tym_ID = :tym_id
	order by
		datum desc
	", array(":tym_id"=>$tym_id),$r_debaty)) {

	de_html($r_debaty);
	cph_h2($lang["debates"]);
	cph_table_head(array($lang["date"],$lang["position"],$lang["opponent"],$lang["resolution"],$lang["result"],$lang["pts"],$lang["gain"],""));
	foreach ($r_debaty as $debata) {
		cph_table_row(array(
			format_date($debata["datum"]),
			($debata["pozice"] == 1) ? $lang["aff"] : $lang["neg"],
			cphs_link("./?page=tym&tym_id=" . $debata["oni_ID"],$debata["oni_tym"]),
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze_short"],array("title"=>$debata["teze"])),
			result_for_side($debata["vitez"], $debata["presvedcive"], $debata["pozice"]),
			$debata["body"],
			$debata["liga_vytezek"],
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"])
		),
			$debata["achtung"] ? array("class"=>"achtung") : array()
		);
	}
	cph_table_end();
}

?>
