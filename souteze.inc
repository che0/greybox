<?php
// souteze.inc
// vypis vsech soutezi v sezone

pg_head();

cph_p($lang["season"] . ": " . season_to_str($GLOBALS["current_season"]));

if ($GLOBALS["cps_souteze"] >= 2) cph_p(cphs_link("./?page=soutez.add",$lang["add competition"]));

cph_table_head(array($lang["competition"], $lang["lang"], $lang["active"] ,$lang["debates"], ""),array("class"=>"visible"));

cpdb_fetch_all("select * from soutez where rocnik = :rocnik order by zamceno, jazyk", array(":rocnik"=>$GLOBALS["current_season"]), $souteze);

foreach ($souteze as $soutez) {
	cph_table_row(array(
		$soutez["nazev"],
		$soutez["jazyk"],
		$soutez["zamceno"] ? $lang["no"] : $lang["yes"],
		cpdb_fetch_one_value("select count(*) from debata where soutez_ID = :soutez_ID",array(":soutez_ID"=>$soutez["soutez_ID"])),
		cphs_link("./?page=soutez.detaily&soutez_id=" . $soutez["soutez_ID"], $lang["details"])
	));
}

cph_table_end();

?>