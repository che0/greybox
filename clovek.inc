<?php
// clovek.inc

$clovek_id = $_REQUEST["clovek_id"];

if (!cpdb_fetch_one_row("select * from clovek where clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id), $clovek)) {
	pg_achtung($lang["no record"]);
	return;
}

$GLOBALS["page"]->title = join_name($clovek["jmeno"], $clovek["nick"], $clovek["prijmeni"]);
pg_head();

$can_edit =
	($GLOBALS["cps_lidi"] >= 2)
	|| (
		$GLOBALS["cps_lidi"] == 1
		&& $clovek["klub_ID"] == $_SESSION["user_klub_ID"]
);

$self_edit = ($clovek["clovek_ID"] == $_SESSION["user_clovek_ID"]);

$domain_klub_role = array(
	"t"=>$lang["coach"]
);

$domain_kontakt_viditelnost = array(
	0=>$lang['public'],
	1=>$lang['registered'],
	2=>$lang['club only']
);

$domain_kontakt_druh = array(
	'telefon'=>$lang['phone'],
	'email'=>$lang['email'],
	'adresa'=>$lang['postal'],
	'icq'=>$lang['icq'],
	'jabber'=>$lang['jabber'],
	'web'=> $lang['web']
);

$domain_prava_souteze = array(
	0=>$lang['access competitions0'],
	1=>$lang['access competitions1'],
	2=>$lang['access competitions2']
);

$domain_prava_lidi = array(
	0=>$lang['access people0'],
	1=>$lang['access people1'],
	2=>$lang['access people2'],
	3=>$lang['access people3']
);

$domain_prava_kluby = array(
	0=>$lang['access clubs0'],
	1=>$lang['access clubs1'],
	2=>$lang['access clubs2']
);

$domain_prava_souteze = array(
	0=>$lang['access competitions0'],
	1=>$lang['access competitions1'],
	2=>$lang['access competitions2']
);

$domain_prava_debaty = array(
	0=>$lang['access debates0'],
	1=>$lang['access debates1']
);

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
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["comment"] . ": ");
	$clovek_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_text($lang["active debater"] . ": ");
	$clovek_f->form[] = new cpf_db_listbox_array("debater",array(1 => $lang['yes'], 0 => $lang['no']));
	$clovek_f->form[] = new cpf_nl;
	$clovek_f->form[] = new cpf_button("save",$lang["save"]);
	$clovek_f->form[] = new cpf_button("delete",$lang["delete"]);
	$clovek_f->load_this($clovek);
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
	$clovek_f->load_this($clovek);
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
	if (isset($clovek["komentar"])) cph_p($clovek["komentar"]);
	
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
	$accr->load_new();
	$accr->render_hidden($lang["new accreditation"]);
} else {
	if (cpdb_fetch_all("
		select rocnik, misto, jazyk from rozhodci where clovek_ID = :clovek_id order by rocnik",
		array(":clovek_id"=>$clovek_id),
		$accrs)) {
		
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
	$accr->form[] = new cpf_db_textbox("tx");
	$accr->form[] = new cpf_db_listbox_array("viditelnost", $domain_kontakt_viditelnost);
	$accr->form["b1"] = new cpf_button("save",$lang["save"]);
	$accr->form["b2"] = new cpf_button("delete",$lang["delete"]);
	$accr->render_select();
	
	$accr->form["b1"] = new cpf_button("add",$lang["add"]);
	unset($accr->form["b2"]);
	$accr->load_new();
	$accr->render_hidden($lang["new contact"]);
} else {

	if ($_SESSION["user_klub_ID"] == $clovek["klub_ID"]) {
		$thresh = 2;
	} elseif ($_SESSION["is_loggen_in"]) {
		$thresh = 1;
	} else {
		$thresh = 0;
	}
	
	if (cpdb_fetch_all("
		select druh, tx from kontakt where clovek_ID = :clovek_id and viditelnost <= :thresh",
		array(":clovek_id"=>$clovek_id, ":thresh"=>$thresh),
		$contacts)) {
		
		cph_h2($lang["contacts"]);
		cph_table_head(array($lang["type"],$lang["contact"]),array("class"=>"visible"));
		foreach ($contacts as $cont) {
			cph_table_row(array(
				$domain_kontakt_druh[$cont["druh"]],
				$cont["tx"]
			));
		}
		cph_table_end();
	}
}

// login

// teams

// tournaments (org)

// points overview

// debates
?>
