<?php
/*
 * greybox
 * $Id: turnaj.add.inc,v 1.4 2005/01/09 00:47:32 che0 Exp $
 */

if ($GLOBALS["cps_souteze"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

pg_head();

$turnaj_f = new cpykhen;
$turnaj_f->form_attr["action"] = "./?page=turnaj.edit.exec";
$turnaj_f->form[] = new cpf_text($lang["title"] . ": ");
$turnaj_f->form[] = new cpf_db_textbox("nazev");
$turnaj_f->form[] = new cpf_nl;
$turnaj_f->form[] = new cpf_text($lang["comment"] . ": ");
$turnaj_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$turnaj_f->form[] = new cpf_nl;
$turnaj_f->form[] = new cpf_text($lang["competition"] . ": ");
$turnaj_f->form[] = new cpf_db_listbox("soutez_id",array("select soutez_ID, nazev from soutez",array()),"soutez_ID");
$turnaj_f->form[] = new cpf_nl;
$turnaj_f->form[] = new cpf_text($lang["date_from"] . ": ");
$turnaj_f->form[] = new cpf_db_textbox("datum_od");
$turnaj_f->form[] = new cpf_nl;
$turnaj_f->form[] = new cpf_text($lang["date_to"] . ": ");
$turnaj_f->form[] = new cpf_db_textbox("datum_do");
$turnaj_f->form[] = new cpf_nl;
$turnaj_f->form[] = new cpf_button("add",$lang["add"]);
$turnaj_f->render_new();

?> 
