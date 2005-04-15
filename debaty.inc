<?php
/*
 * greybox
 * $Id: debaty.inc,v 1.9 2005/04/15 10:54:12 che0 Exp $
 */

pg_head();

if ($GLOBALS["cps_debaty"] >= 1) cph_p(cphs_link("./?page=debata.add",$lang["add debate"]));

if (! cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		teze.tx as teze,
		teze.tx_short as teze_short,
		teze.teze_ID as teze_ID,
		datum,
		vitez,
		presvedcive,
		achtung,
		debata.debata_ID
	from
		debata
		left join soutez on debata.soutez_ID = soutez.soutez_ID
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		soutez.rocnik = :rocnik
	order by
		datum desc
	", array(":rocnik"=>$GLOBALS["current_season"]),$r_debaty)) {
	
	pg_achtung($lang["no records"]);
	return;
}
	
de_html($r_debaty);
cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],""));
foreach ($r_debaty as $debata) {
	if ($debata["vitez"] == 1) {
		$result = $lang["aff"];
	} else {
		$result = $lang["neg"];
	}
	$result .= " ";
	if ($debata["presvedcive"] == 1) {
		$result .= $lang["3:0"];
	} else {
		$result .= $lang["2:1"];
	}
	cph_table_row(array(
		format_date($debata["datum"]),
		cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
		cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
		cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze_short"],array("title"=>$debata["teze"])),
		$result,
		cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["details"])
	),
		$debata["achtung"] ? array("class"=>"achtung") : array()
	);
}
cph_table_end();
?>
