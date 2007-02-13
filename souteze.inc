<?php
/*
 * greybox
 * $Id: souteze.inc,v 1.13 2007/02/13 21:04:31 che0 Exp $
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
			zamceno,
			(select count(*) from debata where soutez_ID = soutez.soutez_ID) as debaty
		from
			soutez
		union
		select
			1 as liga,
			liga_ID as ID,
			rocnik,
			nazev,
			(
				select group_concat(distinct jazyk separator ',')
				from turnaj left join soutez using (soutez_ID)
				where liga_ID = liga.liga_ID
			) as jazyk,
			0 as zamceno,
			(
				select count(debata_ID)
				from turnaj, debata
				where turnaj.liga_ID = liga.liga_ID
					and debata.turnaj_ID = turnaj.turnaj_ID
			) as debaty
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
cph_table_head(array($lang["season"], $lang["competition"], $lang["type"], $lang["lang"], $lang["active"] ,$lang["debates"]));

foreach ($souteze as $soutez) {
	if ($soutez["liga"]) {
		// league
		cph_table_row(array(
			season_to_str($soutez["rocnik"]),
			cphs_link("./?page=liga&liga_id=" . $soutez["ID"], $soutez["nazev"]),
			$lang["league"],
			is_null($soutez["jazyk"]) ? $lang["n/a"] : $soutez["jazyk"],
			$lang["n/a"],
			$soutez["debaty"]
		));

	} else {
		// regular competition
		cph_table_row(array(
			season_to_str($soutez["rocnik"]),
			cphs_link("./?page=soutez&soutez_id=" . $soutez["ID"], $soutez["nazev"]),
			$lang["cup"],
			$soutez["jazyk"],
			$soutez["zamceno"] ? $lang["no"] : $lang["yes"],
			$soutez["debaty"]
		));
	}
}

cph_table_end();

?>
