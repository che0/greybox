<?php
/*
 * greybox
 * $Id: souteze.inc,v 1.8 2005/01/14 22:37:41 che0 Exp $
 * vypis vsech soutezi v sezone
 */

pg_head();

if ($GLOBALS["cps_souteze"] >= 2) cph_p(cphs_link("./?page=soutez.add",$lang["add competition"]));



if (! cpdb_fetch_all("select * from soutez order by rocnik desc, zamceno, jazyk", array(), $souteze)) {
	pg_achtung($lang["no records"]);
	return;
}

de_html($souteze);
cph_table_head(array($lang["season"], $lang["competition"], $lang["lang"], $lang["active"] ,$lang["debates"],),array("class"=>"visible"));

foreach ($souteze as $soutez) {
	cph_table_row(array(
		season_to_str($soutez["rocnik"]),
		cphs_link("./?page=soutez&soutez_id=" . $soutez["soutez_ID"], $soutez["nazev"]),
		$soutez["jazyk"],
		$soutez["zamceno"] ? $lang["no"] : $lang["yes"],
		cpdb_fetch_one_value("select count(*) from debata where soutez_ID = :soutez_ID",array(":soutez_ID"=>$soutez["soutez_ID"])),
	));
}

cph_table_end();

?>
