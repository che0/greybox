<?php
/*
 * greybox
 * $Id: clovek.add.inc,v 1.9 2005/10/24 09:04:47 che0 Exp $
 */

$klub_id = $_REQUEST["klub_id"];

pg_head();

cph_p($lang['clovek.add.achtung']); // warning about adding already existing people

if ($GLOBALS["cps_lidi"] < 1) {
	pg_achtung($lang['access denied']);
	return;
}

$clovek_f = new cpykhen;
$clovek_f->form_attr["action"] = "./?page=clovek.edit.exec";
$clovek_f->form[] = new cpf_text($lang["name"] . ": ");
$clovek_f->form[] = new cpf_db_textbox("jmeno","jmeno",array("maxlength"=>40));
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["surname"] . ": ");
$clovek_f->form[] = new cpf_db_textbox("prijmeni","prijmeni",array("maxlength"=>60));
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["nick"] . ": ");
$clovek_f->form[] = new cpf_db_textbox("nick","nick",array("maxlength"=>60));
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["club"] . ": ");
$clovek_f->form[] = new cpf_db_listbox("klub_id",array(
	"select klub_ID, nazev from klub union select 0, :no_club as nazev", array(":no_club"=>$lang["no club"])),"klub_ID");
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["born"] . ": ");
$clovek_f->form[] = new cpf_db_textbox("narozen","narozen",array("maxlength"=>10));
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["comment"] . ": ");
$clovek_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["active debater"] . ": ");
$clovek_f->form[] = new cpf_db_listbox_array("debater",array(1 => $lang['yes'], 0 => $lang['no']));
$clovek_f->form[] = new cpf_nl;
$clovek_f->form[] = new cpf_text($lang["full member"] . ": ");
if ($GLOBALS["cps_lidi"] >= 3) {
	// only admins can edit membership
	$clovek_f->form[] = new cpf_db_listbox_array("clen", array(1 => $lang['yes'], 0 => $lang['no']));
} else {
	$clovek_f->form[] = new cpf_db_text("clen", array(), array(1 => $lang['yes'], 0 => $lang['no']));
}
$clovek_f->form[] = new cpf_nl;

$clovek_f->form[] = new cpf_button("add",$lang["add"]);
$clovek_f->load_this(array("klub_ID"=>$klub_id, "debater"=>1, "clen"=>0));
$clovek_f->render();


?>