<?php
// teze.inc
// vypis vsech tezi

$lang_choice = (isset ($_REQUEST["lang"])) ? $_REQUEST["lang"] : "cz";

pg_head();
if ($GLOBALS["cps_teze"] >= 1) cph_p(cphs_link("./?page=teze.add&lang=" . $lang_choice,$lang["add resolution"]));


cph_switch($lang["Language"], "./?page=teze&lang=%s", array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"), $lang_choice);

cpdb_fetch_all("select teze_ID, tx, not isnull(komentar) as mame_komentar from teze where jazyk = :lang",array(":lang"=>$lang_choice),$result);

cph_table_head(array($lang["resolution"],$lang["comment"],""),array("class"=>"visible"));
foreach ($result as $row) {
	cph_table_row(array(
		$row["tx"],
		($row["mame_komentar"] == 1) ? $lang["yes"] : "",
		cphs_link("./?page=teze.detaily&teze_id=" . $row["teze_ID"], $lang["details"])
	));
}
cph_table_end();
?>
