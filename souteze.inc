<?php
/*
 * greybox
 * $Id: souteze.inc,v 1.9 2005/03/22 18:18:22 che0 Exp $
 * vypis vsech soutezi v sezone
 */

pg_head();

if ($GLOBALS["cps_souteze"] >= 2) cph_p(
	cphs_link("./?page=soutez.add",$lang["add competition"])
	. " | " . cphs_link("./?page=liga.add",$lang["add league"])
);



if (! cpdb_fetch_all("
		select
			0 as liga,
			soutez_ID as ID,
			rocnik,
			nazev,
			jazyk,
			zamceno
		from
			soutez
		union
		select
			1 as liga,
			liga_ID as ID,
			rocnik,
			nazev,
			'' as jazyk,
			0 as zamceno
		from
			liga
		order by
			rocnik desc,
			liga desc,
			zamceno,
			jazyk
	", array(), $souteze)) {
	pg_achtung($lang["no records"]);
	return;
}

de_html($souteze);
cph_table_head(array($lang["season"], $lang["competition"], $lang["lang"], $lang["active"] ,$lang["debates"],),array("class"=>"visible"));

foreach ($souteze as $soutez) {
	if ($soutez["liga"]) {
		// league
		cph_table_row(array(
			season_to_str($soutez["rocnik"]),
			cphs_link("./?page=liga&liga_id=" . $soutez["ID"], $soutez["nazev"]),
			$lang["n/a"],
			$lang["n/a"],
			$lang["n/a"]
		));

	} else {
		// regular competition
		cph_table_row(array(
			season_to_str($soutez["rocnik"]),
			cphs_link("./?page=soutez&soutez_id=" . $soutez["ID"], $soutez["nazev"]),
			$soutez["jazyk"],
			$soutez["zamceno"] ? $lang["no"] : $lang["yes"],
			cpdb_fetch_one_value("select count(*) from debata where soutez_ID = :soutez_ID",array(":soutez_ID"=>$soutez["ID"]))
		));
	}
}

cph_table_end();

?>
