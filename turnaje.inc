<?php
/*
 * greybox
 * $Id: turnaje.inc,v 1.10 2005/03/23 20:05:48 che0 Exp $
 */

pg_head();

if ($GLOBALS["cps_souteze"] >= 1) cph_p(cphs_link("./?page=turnaj.add",$lang["add tournament"]));

cph_table_head(array($lang["tournament"], $lang["date_from"], $lang["date_to"] , $lang["competition"], $lang["debates"]));

cpdb_fetch_all("
	select
		turnaj.turnaj_ID,
		turnaj.nazev as turnaj,
		turnaj.datum_od,
		turnaj.datum_do,
		soutez.soutez_ID,
		soutez.nazev as soutez,
		liga.liga_ID,
		liga.nazev as liga
	from
		turnaj
		left join soutez on turnaj.soutez_ID = soutez.soutez_ID
		left join liga on turnaj.liga_ID = liga.liga_ID
	where
		soutez.rocnik = :rocnik
	order by
		turnaj.datum_od, soutez.jazyk
	", array(":rocnik"=>$GLOBALS["current_season"]), $turnaje);

de_html($turnaje);
foreach ($turnaje as $turnaj) {
	cph_table_row(array(
		cphs_link("./?page=turnaj&turnaj_id=" . $turnaj["turnaj_ID"], $turnaj["turnaj"]),
		$turnaj["datum_od"],
		$turnaj["datum_do"],
		cphs_link("./?page=soutez&soutez_id=" . $turnaj["soutez_ID"], $turnaj["soutez"])
		. ((isset($turnaj["liga_ID"]))
			? (", " . cphs_link("./?page=liga&liga_id=" . $turnaj["liga_ID"], $turnaj["liga"]))
			: ""),
		cpdb_fetch_one_value("select count(*) from debata where turnaj_ID = :turnaj_id",array(":turnaj_id"=>$turnaj["turnaj_ID"])),
	));
}

cph_table_end();

?>
