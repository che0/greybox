<?php

pg_head();

if ($GLOBALS["cps_kluby"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

$klub_f = new cpykhen;
$klub_f->form_attr["action"] = "./?page=klub.edit.exec";
$klub_f->form[] = new cpf_text($lang["club name"] . ": ");
$klub_f->form[] = new cpf_db_textbox("nazev");
$klub_f->form[] = new cpf_nl;
$klub_f->form[] = new cpf_text($lang["short name"] . ": ");
$klub_f->form[] = new cpf_db_textbox("kratky_nazev");
$klub_f->form[] = new cpf_nl;
$klub_f->form[] = new cpf_text($lang["place"] . ": ");
$klub_f->form[] = new cpf_db_textbox("misto");
$klub_f->form[] = new cpf_nl;
$klub_f->form[] = new cpf_text($lang["comment"] . ": ");
$klub_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$klub_f->form[] = new cpf_nl;
$klub_f->form[] = new cpf_button("add",$lang["add"]);
$klub_f->load_new();
$klub_f->render();
?>