<?php
pg_head();

$teze = new cpykhen;
$teze->select = "select * from teze where teze_ID = :teze_id";
$teze->select_args = array (":teze_id" => $_REQUEST["teze_id"]);
$teze->form_attr["action"] = "./?page=teze.uprav.exec";
$teze->form[] = new cpf_db_hidden("teze_ID");
$teze->form[] = new cpf_text($lang["language"] . ": ");
$teze->form[] = new cpf_db_listbox_array("jazyk",array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"));
$teze->form[] = new cpf_nl;
$teze->form[] = new cpf_text($lang["resolution"] . ": ");
$teze->form[] = new cpf_db_textbox("tx","tx",array("size"=>60));
$teze->form[] = new cpf_nl;
$teze->form[] = new cpf_text($lang["comment"] . ": ");
$teze->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$teze->form[] = new cpf_nl;
$teze->form[] = new cpf_button("save",$lang["save"]);
$teze->form[] = new cpf_button("delete",$lang["delete"]);
$teze->render_select();

?>