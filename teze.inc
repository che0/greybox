<?php
/*
 * greybox
 * $Id: teze.inc,v 1.12 2005/01/09 00:53:08 che0 Exp $
 * vypis vsech tezi
 */

if (isset($_REQUEST["teze_id"])) {
	include("teze.detaily.inc");
	return;
}

$lang_choice = (isset ($_REQUEST["jazyk"])) ? $_REQUEST["jazyk"] : "cz";

pg_head();
if ($GLOBALS["cps_teze"] >= 1) cph_p(cphs_link("./?page=teze.add&jazyk=" . $lang_choice,$lang["add resolution"]));


cph_switch($lang["language"], "./?page=teze&jazyk=%s", array("cz"=>"cz", "en"=>"en", "de"=>"de", "fr"=>"fr"), $lang_choice);

cpdb_fetch_all("
	select
		teze.teze_ID,
		teze.tx,
		count(debata_ID) as debates
	from
		teze
		left join debata on teze.teze_ID = debata.teze_ID
	where
		jazyk = :lang
	group by
		teze.teze_ID,
		teze.tx
	order by
		teze.tx
	",
	array(":lang"=>$lang_choice),$result);

de_html($result);
cph_table_head(array($lang["resolution"],$lang["debates"]),array("class"=>"visible"));
foreach ($result as $row) {
	cph_table_row(array(
		cphs_link("./?page=teze.detaily&teze_id=" . $row["teze_ID"], $row["tx"]),
		$row["debates"]
	));
}
cph_table_end();
?>
