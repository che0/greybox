<?php
/*
 * greybox
 * $Id: teze.add.inc,v 1.6 2005/01/09 00:53:08 che0 Exp $
 */

pg_head();

$teze = new cpykhen;
$teze->form_attr["action"] = "./?page=teze.add.exec";
$teze->form[] = new cpf_text($lang["language"] . ": ");
$teze->form[] = new cpf_db_listbox_array("jazyk",array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"));
$teze->form[] = new cpf_nl;
$teze->form[] = new cpf_text($lang["resolution"] . ": ");
$teze->form[] = new cpf_db_textbox("tx","tx",array("size"=>60));
$teze->form[] = new cpf_nl;
$teze->form[] = new cpf_text($lang["comment"] . ": ");
$teze->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$teze->form[] = new cpf_nl;
$teze->form[] = new cpf_button("add",$lang["add"]);
$teze->load_this(array("jazyk"=>$_REQUEST["jazyk"]));
$teze->render();

?>
