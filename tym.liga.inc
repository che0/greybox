<?php
/*
 * greybox
 * $Id: tym.liga.inc,v 1.4 2005/03/23 20:05:48 che0 Exp $
 */

$tym_id = $_REQUEST["tym_id"];
$liga_id = $_REQUEST["liga_id"];

$tym_name = de_html_s(cpdb_fetch_one_value("select nazev from tym where tym_ID = :tym_id",array(":tym_id"=>$tym_id)));
$liga_name = de_html_s(cpdb_fetch_one_value("select nazev from liga where liga_ID = :liga_id",array(":liga_id"=>$liga_id)));

$GLOBALS["page"]->title = $tym_name . " / " . $liga_name;

pg_head();

cpdb_fetch_one_row("select liga_vytezek, skrtnute_debaty from liga_tym where liga_ID = :liga_id and tym_ID = :tym_id", array(":liga_id"=>$liga_id, ":tym_id"=>$tym_id), $stav_tymu);
$skrtnute_debaty = explode(",",$stav_tymu["skrtnute_debaty"]);

cph_p(
	$lang["team"] . ": "
	. cphs_link("./?page=tym&tym_id=" . $tym_id, $tym_name) . cphs_br()
	. $lang["league"] . ": "
	. cphs_link("./?page=liga&liga_id=" . $liga_id, $liga_name) . cphs_br()
	. $lang["current tab"] . ": "
	. $stav_tymu["liga_vytezek"]
);

cpdb_fetch_all("
	select
		tym_oni.tym_ID as oni_ID,
		tym_oni.nazev as oni_tym,
		teze.tx as teze,
		teze.tx_short as teze_short,
		teze.teze_ID as teze_ID,
		datum,
		vitez,
		presvedcive,
		debata.debata_ID,
		dt_my.pozice,
		dt_my.liga_vytezek,
		(
			select avg(liga_vytezek)
			from debata_tym dt_v, debata d_v
			where 
				d_v.debata_ID = dt_v.debata_ID
				and dt_v.tym_ID = dt_my.tym_ID
				and dt_v.debata_ID in (select debata_ID from turnaj, debata where turnaj.liga_ID = :liga_id and turnaj.turnaj_ID = debata.turnaj_ID)
				and d_v.datum <= debata.datum
		) as novy_tab
	from
		debata_tym dt_my
		left join debata on dt_my.debata_ID = debata.debata_ID
		left join debata_tym dt_oni on dt_oni.debata_ID = debata.debata_ID and dt_oni.pozice != dt_my.pozice
		left join tym tym_oni on dt_oni.tym_ID = tym_oni.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		debata.turnaj_ID in (select turnaj_ID from turnaj where liga_ID = :liga_id)
		and dt_my.tym_ID = :tym_id
	order by
		datum desc
	", array(":liga_id"=>$liga_id, ":tym_id"=>$tym_id),$r_debaty);
	
de_html($r_debaty);

cph_table_head(array($lang["date"],$lang["position"],$lang["opponent"],$lang["resolution"],$lang["result"],$lang["gain"],$lang["new tab"],""));
foreach ($r_debaty as $debata) {
	if ($debata["vitez"] == 1) { $result = $lang["aff"]; } else { $result = $lang["neg"]; }
	$result .= " ";
	if ($debata["presvedcive"] == 1) { $result .= $lang["3:0"]; } else { $result .= $lang["2:1"]; }
	
	cph_table_row(array(
		format_date($debata["datum"]),
		($debata["pozice"] == 1) ? $lang["aff"] : $lang["neg"],
		cphs_link("./?page=tym&tym_id=" . $debata["oni_ID"],$debata["oni_tym"]),
		cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze_short"],array("title"=>$debata["teze"])),
		$result,
		$debata["liga_vytezek"],
		$debata["novy_tab"],
		cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"])
	), in_array($debata["debata_ID"], $skrtnute_debaty) ? array("class"=>"inactive") : array()
	);
}
cph_table_end();

?>