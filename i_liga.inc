<?php
/*
 * greybox
 * $Id: i_liga.inc,v 1.1 2005/03/22 18:18:22 che0 Exp $
 */

function get_pykhen_liga() {
	global $lang;
	
	$liga_f = new cpykhen;
	$liga_f->form_attr["action"] = "./?page=liga.edit.exec";
	$liga_f->form[] = new cpf_db_hidden("liga_id","liga_ID");
	$liga_f->form[] = new cpf_text($lang["title"] . ": ");
	$liga_f->form[] = new cpf_db_textbox("nazev");
	$liga_f->form[] = new cpf_nl;
	$liga_f->form[] = new cpf_text($lang["season"] . ": ");
	$liga_f->form[] = new cpf_db_listbox_array("rocnik",$GLOBALS["season_domain"]);
	$liga_f->form[] = new cpf_nl;
	$liga_f->form[] = new cpf_text($lang["comment"] . ": ");
	$liga_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$liga_f->form[] = new cpf_nl;
	return $liga_f;
}

?>