<?php

if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

pg_head();

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
$soutez_f->form[] = new cpf_button("add",$lang["add"]);
$soutez_f->render_new();

?> 
