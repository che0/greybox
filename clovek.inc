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

$self_edit = ($clovek["clovek_ID"] == $_SESSION["user_ID"]);

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
	
} else {
	// just view
}

// club positions (coach)

// accreditations

// contacts

// login

// teams

// tournaments (org)

// points overview

// debates
?>
