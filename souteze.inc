<?php
/*
 * greybox
 * $Id: souteze.inc,v 1.6 2005/01/09 00:47:32 che0 Exp $
 */
// souteze.inc
// vypis vsech soutezi v sezone

pg_head();

cph_p($lang["season"] . ": " . season_to_str($GLOBALS["current_season"]));

if ($GLOBALS["cps_souteze"] >= 2) cph_p(cphs_link("./?page=soutez.add",$lang["add competition"]));

cph_table_head(array($lang["competition"], $lang["lang"], $lang["active"] ,$lang["debates"],),array("class"=>"visible"));

cpdb_fetch_all("select * from soutez where rocnik = :rocnik order by zamceno, jazyk", array(":rocnik"=>$GLOBALS["current_season"]), $souteze);
de_html($souteze);

foreach ($souteze as $soutez) {
	cph_table_row(array(
		cphs_link("./?page=soutez&soutez_id=" . $soutez["soutez_ID"], $soutez["nazev"]),
		$soutez["jazyk"],
		$soutez["zamceno"] ? $lang["no"] : $lang["yes"],
		cpdb_fetch_one_value("select count(*) from debata where soutez_ID = :soutez_ID",array(":soutez_ID"=>$soutez["soutez_ID"])),
	));
}

cph_table_end();

?>
