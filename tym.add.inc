<?php
/*
 * greybox
 * $Id: tym.add.inc,v 1.6 2005/03/30 11:07:19 che0 Exp $
 */

if ($GLOBALS["cps_kluby"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

pg_head();

$tym_f = new cpykhen;
$tym_f->form_attr["action"] = "./?page=tym.edit.exec";
$tym_f->form[] = new cpf_db_hidden("tym_id","tym_ID");
$tym_f->form[] = new cpf_text($lang["club"] . ": ");
$tym_f->form[] = new cpf_db_listbox("klub_id",array("select klub_ID, nazev from klub", array()), "klub_ID");
$tym_f->form[] = new cpf_nl;
$tym_f->form[] = new cpf_text($lang["team name"] . ": ");
$tym_f->form[] = new cpf_db_textbox("nazev","nazev",array("size"=>40,"maxlength"=>255));
$tym_f->form[] = new cpf_nl;
$tym_f->form[] = new cpf_text($lang["comment"] . ": ");
$tym_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$tym_f->form[] = new cpf_nl;
$tym_f->form[] = new cpf_button("add",$lang["add"]);
$tym_f->load_this(array("klub_ID"=>$_REQUEST["klub_id"]));
$tym_f->render();

?>
