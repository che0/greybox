<?php
// clovek.inc
include_once("i_domeny.inc");

$clovek_id = $_REQUEST["clovek_id"];

if (!cpdb_fetch_one_row("select * from clovek where clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id), $clovek_p)) {
	pg_achtung($lang["no record"]);
	return;
}
$clovek = de_html_s($clovek_p);

$GLOBALS["page"]->title = join_name($clovek["jmeno"], $clovek["nick"], $clovek["prijmeni"]);
pg_head();
if (isset($clovek["komentar"])) cph_p($clovek["komentar"]);

$can_edit =
	($GLOBALS["cps_lidi"] >= 2)
	|| (
		$GLOBALS["cps_lidi"] == 1
		&& $clovek["klub_ID"] == $_SESSION["user_klub_ID"]
);

$self_edit = ($clovek["clovek_ID"] == $_SESSION["user_clovek_ID"]);

// basic details
if ($can_edit) {
	// edit all 
	$clovek_f = new cpykhen;
	$clovek_f->form_attr["action"] = "./?page=clovek.edit.exec";
	$clovek_f->form[] = new cpf_hidden("clovek_id",$clovek["clovek_ID"]);
	$clovek_f->form[] = new cpf_hidden("cur_klub_id",$clovek["klub_ID"]);
	$clovek_f->form[] = new cpf_text($lang["name"] . ": ");
	$clovek_f->form[] = new cpf_db_textbox("jmeno");
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["surname"] . ": ");
	$clovek_f->form[] = new cpf_db_textbox("prijmeni");
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["nick"] . ": ");
	$clovek_f->form[] = new cpf_db_textbox("nick");
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["club"] . ": ");
	$clovek_f->form[] = new cpf_db_listbox("klub_id",array(
		"select klub_ID, nazev from klub union select 0, :no_club",array(":no_club"=>$lang["no club"])),"klub_ID");
	$clovek_f->form[] = new cpf_db_link("klub_ID","./?page=klub&klub_id=%s",$lang["link"]);
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["born"] . ": ");
	$clovek_f->form[] = new cpf_db_textbox("narozen");
	$clovek_f->form[] = new cpf_text(" " . $lang["yyyy-mm-dd"],array("class"=>"desc"));
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["comment"] . ": ");
	$clovek_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["active debater"] . ": ");
	$clovek_f->form[] = new cpf_db_listbox_array("debater",array(1 => $lang['yes'], 0 => $lang['no']));
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_button("save",$lang["save"]);
	$clovek_f->form[] = new cpf_button("delete",$lang["delete"]);
	$clovek_f->load_this($clovek_p);
	$clovek_f->render();
	
} elseif ($self_edit) {
	// edit self
	$clovek_f = new cpykhen;
	$clovek_f->form_attr["action"] = "./?page=clovek.self.exec";
	$clovek_f->form[] = new cpf_hidden("clovek_id",$clovek["clovek_ID"]);
	$clovek_f->form[] = new cpf_text($lang["name"] . ": ");
	$clovek_f->form[] = new cpf_db_text("jmeno");
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["surname"] . ": ");
	$clovek_f->form[] = new cpf_db_text("prijmeni");
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["nick"] . ": ");
	$clovek_f->form[] = new cpf_db_textbox("nick");
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["comment"] . ": ");
	$clovek_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_button("save",$lang["save"]);
	$clovek_f->load_this($clovek_p);
	$clovek_f->render();
	
	if (is_null($clovek["narozen"])) {
		$clovek_f = new cpykhen;
		$clovek_f->form_attr["action"] = "./?page=clovek.self.exec";
		$clovek_f->form[] = new cpf_hidden("clovek_id",$clovek["clovek_ID"]);
		$clovek_f->form[] = new cpf_text($lang["born"] . ": ");
		$clovek_f->form[] = new cpf_db_textbox("narozen");
		$clovek_f->form[] = new cpf_button("born",$lang["save"]);
		$clovek_f->render_new();
	} else {
		cph_p($lang["born"] . ": " . $clovek["narozen"]);
	}
	
	if (isset($clovek["klub_ID"])) cph_p($lang["club"] . ": " . cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],cpdb_fetch_one_value("select nazev from klub where klub_ID = :klub_id",array(":klub_id"=>$clovek["klub_ID"]))));
	
	cph_p($lang["active debater"] . ": " . ($clovek["debater"] ? $lang["yes"] : $lang["no"]));
	
} else {
	// just view
	
	if (isset($clovek["klub_ID"])) cph_p($lang["club"] . ": " . cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],cpdb_fetch_one_value("select nazev from klub where klub_ID = :klub_id",array(":klub_id"=>$clovek["klub_ID"]))));
	
	cph_p($lang["active debater"] . ": " . ($clovek["debater"] ? $lang["yes"] : $lang["no"]));
}

// club positions (coach)
if (cpdb_fetch_all("
	select
		clovek_klub.role,
		clovek_klub.rocnik,
		klub.klub_ID,
		klub.nazev
	from
		clovek_klub
		left join klub using (klub_ID)
	where
		clovek_klub.clovek_ID = :clovek_id
	order by
		clovek_klub.rocnik
	",
	array(":clovek_id"=>$clovek_id),
	$positions)) {
	
	de_html($positions);
	cph_h2($lang["club positions"]);
	cph_table_head(array($lang["role"],$lang["season"],$lang["club"]),array("class"=>"visible"));
	foreach ($positions as $pos) {
		cph_table_row(array(
			$domain_klub_role[$pos["role"]],
			season_to_str($pos["rocnik"]),
			cphs_link("./?page=klub&klub_id=" . $pos["klub_ID"],$pos["nazev"])
		));
	}
	cph_table_end();
}

// accreditations
if ($can_edit) {
	cph_h2($lang["accreditations"]);
	
	$accr = new cpykhen;
	$accr->form_attr["action"] = "./?page=clovek.accr.edit.exec";
	$accr->select = "select * from rozhodci where clovek_ID = :clovek_id";
	$accr->select_args = array(":clovek_id"=>$clovek_id);
	$accr->form[] = new cpf_hidden("clovek_id",$clovek_id);
	$accr->form[] = new cpf_db_hidden("rozhodci_id","rozhodci_ID");
	$accr->form[] = new cpf_text($lang["season"] . ": ");
	$accr->form[] = new cpf_db_listbox_array("rocnik", $GLOBALS["season_domain"]);
	$accr->form[] = new cpf_text(" " . $lang["language"] . ": ");
	$accr->form[] = new cpf_db_textbox("jazyk");
	$accr->form[] = new cpf_text(" " . $lang["separate by commas: en,cz"],array("class"=>"desc"));
	$accr->form[] = new cpf_nl;
	$accr->form[] = new cpf_text($lang["place"] . ": ");
	$accr->form[] = new cpf_db_textbox("misto");
	$accr->form["b1"] = new cpf_button("save",$lang["save"]);
	$accr->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$accr->render_select();
	
	$accr->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($accr->form["b2"]);
	$accr->load_this(array("rocnik"=>$GLOBALS["current_season"]));
	$accr->render_hidden($lang["new accreditation"]);
} else {
	if (cpdb_fetch_all("
		select rocnik, misto, jazyk from rozhodci where clovek_ID = :clovek_id order by rocnik",
		array(":clovek_id"=>$clovek_id),
		$accrs)) {
		
		de_html($accrs);
		cph_h2($lang["accreditations"]);
		cph_table_head(array($lang["season"],$lang["language"],$lang["place"]),array("class"=>"visible"));
		foreach ($accrs as $accr) {
			cph_table_row(array(
				season_to_str($accr["rocnik"]),
				$accr["jazyk"],
				$accr["misto"]
			));
		}
		cph_table_end();
	}
}

// contacts
if ($can_edit || $self_edit) {
	cph_h2($lang["contacts"]);
	
	$accr = new cpykhen;
	$accr->form_attr["action"] = "./?page=clovek.cont.edit.exec";
	$accr->select = "select * from kontakt where clovek_ID = :clovek_id";
	$accr->select_args = array(":clovek_id"=>$clovek_id);
	$accr->form[] = new cpf_hidden("clovek_id",$clovek_id);
	$accr->form[] = new cpf_db_hidden("kontakt_id","kontakt_ID");
	$accr->form[] = new cpf_db_listbox_array("druh", $domain_kontakt_druh);
	$accr->form[] = new cpf_db_textbox("tx","tx",array("size"=>"40"));
	$accr->form[] = new cpf_db_listbox_array("viditelnost", $domain_kontakt_viditelnost);
	$accr->form["b1"] = new cpf_button("save",$lang["save"]);
	$accr->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$accr->render_select();
	
	$accr->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($accr->form["b2"]);
	$accr->load_new();
	$accr->render_hidden($lang["new contact"]);
} else {

	if (!is_null($clovek["klub_ID"]) && $_SESSION["user_klub_ID"] == $clovek["klub_ID"]) {
		$thresh = 2;
	} elseif ($_SESSION["is_logged_in"] == true) {
		$thresh = 1;
	} else {
		$thresh = 0;
	}
	
	if (cpdb_fetch_all("
		select druh, tx from kontakt where clovek_ID = :clovek_id and viditelnost <= :thresh",
		array(":clovek_id"=>$clovek_id, ":thresh"=>$thresh),
		$contacts)) {
		
		de_html($contacts);
		cph_h2($lang["contacts"]);
		cph_table_head(array($lang["type"],$lang["contact"]),array("class"=>"visible"));
		foreach ($contacts as $cont) {
			switch ($cont["druh"]) {
				case "email":
					$tx = cphs_link("mailto:" . $cont["tx"], $cont["tx"]);
				break;
				
				case "web":
					$tx = cphs_link($cont["tx"], $cont["tx"]);
				break;
				
				default:
					$tx = $cont["tx"];
				break;
			}
			cph_table_row(array(
				$domain_kontakt_druh[$cont["druh"]],
				$tx
			));
		}
		cph_table_end();
	}
}

// access

if ($GLOBALS["cps_lidi"] >= 3) {
	cph_h2($lang["access levels"]);
	$access = new cpykhen;
	$access->form_attr["action"] = "./?page=clovek.edit.exec";
	$access->form[] = new cpf_hidden("clovek_id",$clovek_id);
	$access->form[] = new cpf_text($lang["people"] . ": ");
	$access->form[] = new cpf_db_listbox_array("prava_lidi",$domain_prava_lidi);
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["clubs"] . ": ");
	$access->form[] = new cpf_db_listbox_array("prava_kluby",$domain_prava_kluby);
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["competitions"] . ": ");
	$access->form[] = new cpf_db_listbox_array("prava_souteze",$domain_prava_souteze);
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["debates"] . ": ");
	$access->form[] = new cpf_db_listbox_array("prava_debaty",$domain_prava_debaty);
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_button("access",$lang["save"]);
	$access->load_this($clovek_p);
	$access->render();
	
} elseif ($clovek["prava_lidi"]+$clovek["prava_kluby"]+$clovek["prava_souteze"]+$clovek["prava_debaty"] > 0) {
	cph_h2($lang["access levels"]);
	cph_table_head(array($lang["area"],$lang["access"]),array("class"=>"visible"));
	cph_table_row(array($lang["people"],$domain_prava_lidi[$clovek["prava_lidi"]]));
	cph_table_row(array($lang["clubs"],$domain_prava_kluby[$clovek["prava_kluby"]]));
	cph_table_row(array($lang["competitions"],$domain_prava_souteze[$clovek["prava_souteze"]]));
	cph_table_row(array($lang["debates"],$domain_prava_debaty[$clovek["prava_debaty"]]));
	cph_table_end();
}

// login
if ($GLOBALS["cps_lidi"] >= 3) {
	cph_h2($lang["login"]);
	
	$access = new cpykhen;
	$access->form_attr["action"] = "./?page=clovek.pw.edit.exec";
	$access->form[] = new cpf_hidden("clovek_id",$clovek_id);
	$access->form[] = new cpf_db_hidden("login_id","login_ID");
	$access->form[] = new cpf_text($lang["username"] . ": ");
	$access->form[] = new cpf_db_textbox("username");
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["password"] . ": ");
	$access->form[] = new cpf_db_password("n1");
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["password"] . ": ");
	$access->form[] = new cpf_db_password("n2");
	$access->form[] = new cpf_nl;
	
	if (cpdb_fetch_one_row("select login_ID, username from login where clovek_ID = :clovek_id",array(":clovek_id"=>$clovek_id),$login)) {
		$access->form[] = new cpf_button("save",$lang["save"]);
		$access->form[] = new cpf_button("delete",$lang["delete"]);
		$access->load_this($login);
		$access->render();
	} else {
		$access->form[] = new cpf_button("add",$lang["add"]);
		$access->render_new();

	}
	
} elseif ($self_edit) {
	cph_h2($lang["password"]);
	$access = new cpykhen;
	$access->form_attr["action"] = "./?page=clovek.pw.edit.exec";
	$access->form[] = new cpf_hidden("clovek_id",$clovek_id);
	$access->form[] = new cpf_text($lang["old"] . ": ");
	$access->form[] = new cpf_db_password("old");
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["new"] . ": ");
	$access->form[] = new cpf_db_password("n1");
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_text($lang["new"] . ": ");
	$access->form[] = new cpf_db_password("n2");
	$access->form[] = new cpf_nl;
	$access->form[] = new cpf_button("password",$lang["change password"]);
	$access->load_this($clovek_p);
	$access->render();
}

// teams
if (cpdb_fetch_all("select tym.tym_ID, tym.nazev, clovek_tym.aktivni from clovek_tym left join tym using (tym_ID) where clovek_tym.clovek_ID = :clovek_id order by clovek_tym.aktivni desc, tym.nazev", array(":clovek_id"=>$clovek_id),$tymy)) {
	cph_h2($lang["teams"]);
	cph_table_head(array($lang["status"],$lang["team"]),array("class"=>"visible"));
	foreach ($tymy as $tym) {
		cph_table_row(array(
			$tym["aktivni"] ? $lang["active"] : $lang["disabled"],
			cphs_link("./?page=tym&tym_id=" . $tym["tym_ID"], $tym["nazev"])
		), $tym["aktivni"] ? array() : array("class"=>"inactive"));
	}
	cph_table_end();
}

// tournaments (org)
if (cpdb_fetch_all("select clovek_turnaj.role, turnaj.nazev, turnaj.datum_od, turnaj.turnaj_ID from clovek_turnaj left join turnaj using (turnaj_ID) where clovek_turnaj.clovek_ID = :clovek_id order by turnaj.datum_od", array(":clovek_id"=>$clovek_id), $turnaje)) {
	de_html($turnaje);
	cph_h2($lang["tournaments"]);
	cph_table_head(array($lang["role"],$lang["tournament"],$lang["date"]), array("class"=>"visible"));
	foreach ($turnaje as $turnaj) {
		cph_table_row(array(
			$domain_turnaj_role[$turnaj["role"]],
			cphs_link("./?page=turnaj&turnaj_id=" . $turnaj["turnaj_ID"], $turnaj["nazev"]),
			format_date($turnaj["datum_od"])
		));
	}
	cph_table_end();
}

// debates
if (cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		teze.tx as teze,
		teze.teze_ID as teze_ID,
		debata.datum,
		debata.vitez,
		debata.presvedcive,
		debata.debata_ID,
		clovek_debata.role,
		clovek_debata.kidy,
		clovek_debata.rozhodnuti as adj_rozhodnuti,
		clovek_debata.presvedcive as adj_presvedcive
	from
		clovek_debata
		left join debata on debata.debata_ID = clovek_debata.debata_ID
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		clovek_debata.clovek_ID = :clovek_id
	order by
		datum desc
	", array(":clovek_id"=>$clovek_id),$r_debaty)) {
	
	de_html($r_debaty);
	cph_h2($lang["debates"]);
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],"",$lang["role"],""),array("class"=>"visible"));
	foreach ($r_debaty as $debata) {
		if ($debata["vitez"] == 1) { $result = $lang["aff"]; } else { $result = $lang["neg"]; }
		$result .= " ";
		if ($debata["presvedcive"] == 1) { $result .= $lang["3:0"]; } else { $result .= $lang["2:1"]; }
		switch ($debata["role"]) {
			case "o": // organizer
				$detaily = "";
			break;
			case "r": // adjudicator
				$detaily = ($debata["adj_rozhodnuti"] ? "A" : "N") . ($debata["adj_presvedcive"] ? "+" : "-");
			break;
			default: // debater
				$detaily = $debata["kidy"];
			break;
		}
		cph_table_row(array(
			format_date($debata["datum"]),
			cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
			cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze"]),
			$result,
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"]),
			$domain_debata_role[$debata["role"]],
			$detaily
		));
	}
	cph_table_end();
}

// ipoints overview
if (cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		teze.tx as teze,
		teze.teze_ID as teze_ID,
		debata.datum,
		debata.vitez,
		debata.presvedcive,
		debata.debata_ID,
		clovek_debata_ibody.role,
		clovek_debata_ibody.ibody
	from
		clovek_debata_ibody
		left join debata on debata.debata_ID = clovek_debata_ibody.debata_ID
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		clovek_debata_ibody.clovek_ID = :clovek_id
		and clovek_debata_ibody.rocnik = :rocnik
	order by
		datum desc
	", array(":clovek_id"=>$clovek_id, ":rocnik"=>$GLOBALS["current_season"]),$r_debaty)) {
	
	de_html($r_debaty);
	cph_h2($lang["points overview"]);
	cph_p($lang["total"] . ": " . cpdb_fetch_one_value("select sum(ibody) from clovek_debata_ibody where clovek_ID = :clovek_id and rocnik = :rocnik",array(":clovek_id"=>$clovek_id, ":rocnik"=>$GLOBALS["current_season"])));
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],"",$lang["role"],$lang["ipoints"]),array("class"=>"visible"));
	foreach ($r_debaty as $debata) {
		if ($debata["vitez"] == 1) { $result = $lang["aff"]; } else { $result = $lang["neg"]; }
		$result .= " ";
		if ($debata["presvedcive"] == 1) { $result .= $lang["3:0"]; } else { $result .= $lang["2:1"]; }
		cph_table_row(array(
			format_date($debata["datum"]),
			cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
			cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze"]),
			$result,
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"]),
			$domain_ibody_role[$debata["role"]],
			$debata["ibody"]
		));
	}
	cph_table_end();
}

?>
