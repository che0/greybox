<?php
//FIXME

pg_head();

if ($GLOBALS["cps_souteze"] >= 1) cph_p(cphs_link("./?page=turnaj.add",$lang["add tournament"]));

cph_table_head(array($lang["competition"], $lang["tournament"], $lang["date_from"], $lang["date_to"] ,$lang["debates"], ""),array("class"=>"visible"));

cpdb_fetch_all("
	select
		turnaj.turnaj_ID,
		turnaj.nazev as turnaj,
		turnaj.datum_od,
		turnaj.datum_do,
		soutez.soutez_ID,
		soutez.nazev as soutez
	from
		turnaj
		left join soutez using (soutez_ID)
	order by
		datum_od desc
	", array(), $turnaje);

foreach ($turnaje as $turnaj) {
	cph_table_row(array(
		cphs_link("./?page=soutez.detaily&soutez_id=" . $turnaj["soutez_ID"], $turnaj["soutez"]),
		cphs_link("./?page=turnaj&turnaj_id=" . $turnaj["turnaj_ID"], $turnaj["turnaj"]),
		$turnaj["datum_od"],
		$turnaj["datum_do"],
		cpdb_fetch_one_value("select count(*) from debata where turnaj_ID = :turnaj_id",array(":turnaj_id"=>$turnaj["turnaj_ID"])),
	));
}

cph_table_end();

?>