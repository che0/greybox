<?php
// teze.inc
// vypis vsech tezi

if (isset($_REQUEST["teze_id"])) {
	include("teze.detaily.inc");
	return;
}

$lang_choice = (isset ($_REQUEST["jazyk"])) ? $_REQUEST["jazyk"] : "cz";

pg_head();
if ($GLOBALS["cps_teze"] >= 1) cph_p(cphs_link("./?page=teze.add&jazyk=" . $lang_choice,$lang["add resolution"]));


cph_switch($lang["language"], "./?page=teze&jazyk=%s", array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"), $lang_choice);

cpdb_fetch_all("select teze_ID, tx, not isnull(komentar) as mame_komentar from teze where jazyk = :lang",array(":lang"=>$lang_choice),$result);

de_html($result);
cph_tag_start("ul");
foreach ($result as $row) {
	cph_tag("li", array(), cphs_link("./?page=teze.detaily&teze_id=" . $row["teze_ID"], $row["tx"]));
	cph_nl();
}
cph_tag_end("ul");
?>
