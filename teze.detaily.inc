<?php
pg_head();
$teze_id = $_REQUEST["teze_id"];

if ($GLOBALS['cps_teze'] >= 1) {
	$teze = new cpykhen;
	$teze->select = "select * from teze where teze_ID = :teze_id";
	$teze->select_args = array (":teze_id" => $teze_id);
	$teze->form_attr["action"] = "./?page=teze.edit.exec";
	$teze->form[] = new cpf_db_hidden("teze_id","teze_ID");
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
} else {
	cpdb_fetch_one_row("select * from teze where teze_ID = :teze_id", array (":teze_id" => $teze_id), $teze);
	cph_p($lang["language"] . ": " . $teze["jazyk"]);
	cph_p($lang["resolution"] . ": " . $teze["tx"]);
	if (isset($teze["komentar"])) cph_p($lang["comment"] . ":" . cphs_br() . $teze["komentar"]);
}

if (cpdb_fetch_all("select soutez.soutez_ID as soutez, nazev from soutez_teze left join soutez using (soutez_ID) where teze_ID = :teze_id", array(":teze_id" => $teze_id), $souteze) > 0) {
	cph_tag("h2",array(),$lang["official in"]);
	cph_table_head(array($lang["competition"],""),array("class"=>"visible"));
	foreach ($souteze as $soutez) {
		cph_table_row(array($soutez["nazev"], cphs_link("./?page=soutez&soutez_id=" .  $soutez["soutez"],$lang["details"])));
	}
	cph_table_end();
}

?>