<?php
/*
 * greybox
 * $Id: i_turnaj.inc,v 1.1 2005/03/29 10:09:50 che0 Exp $
 */

function get_pykhen_turnaj() {
	global $lang;
	
	$turnaj_f = new cpykhen;
	$turnaj_f->form_attr["action"] = "./?page=turnaj.edit.exec";
	$turnaj_f->form[] = new cpf_db_hidden("turnaj_id","turnaj_ID");
	$turnaj_f->form[] = new cpf_text($lang["title"] . ": ");
	$turnaj_f->form[] = new cpf_db_textbox("nazev","nazev",array("maxlength"=>255));
	$turnaj_f->form[] = new cpf_nl;
	$turnaj_f->form[] = new cpf_text($lang["comment"] . ": ");
	$turnaj_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$turnaj_f->form[] = new cpf_nl;
	$turnaj_f->form[] = new cpf_text($lang["competition"] . ": ");
	$turnaj_f->form[] = new cpf_db_listbox("soutez_id",array("select soutez_ID, nazev from soutez",array()),"soutez_ID");
	$turnaj_f->form[] = new cpf_nl;
	$turnaj_f->form[] = new cpf_text($lang["league"] . ": ");
	$turnaj_f->form[] = new cpf_db_listbox("liga_id",array("select 0, :no_league union select liga_ID, nazev from liga",array(":no_league"=>$lang["no league"])),"liga_ID");
	$turnaj_f->form[] = new cpf_db_link("liga_ID","./?page=liga&liga_id=%s",$lang["link"]);
	$turnaj_f->form[] = new cpf_nl;
	$turnaj_f->form[] = new cpf_text($lang["date_from"] . ": ");
	$turnaj_f->form[] = new cpf_db_textbox("datum_od","datum_od",array("maxlength"=>10));
	$turnaj_f->form[] = new cpf_nl;
	$turnaj_f->form[] = new cpf_text($lang["date_to"] . ": ");
	$turnaj_f->form[] = new cpf_db_textbox("datum_do","datum_do",array("maxlength"=>10));
	$turnaj_f->form[] = new cpf_nl;
	return $turnaj_f;
}

?>