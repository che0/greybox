<?php

$tym_id = $_REQUEST["tym_id"];
$soutez_id = $_REQUEST["soutez_id"];

$tym_name = cpdb_fetch_one_value("select nazev from tym where tym_ID = :tym_id",array(":tym_id"=>$tym_id));
$soutez_name = cpdb_fetch_one_value("select nazev from soutez where soutez_ID = :soutez_id",array(":soutez_id"=>$soutez_id));

$GLOBALS["page"]->title = $tym_name . " / " . $soutez_name;

pg_head();

cph_p(
	$lang["team"] . ": "
	. cphs_link("./?page=tym&tym_id=" . $tym_id, $tym_name) . cphs_br()
	. $lang["competition"] . ": "
	. cphs_link("./?page=soutez&soutez_id=" . $soutez_id, $soutez_name) . cphs_br()
	. $lang["total points"] . ": "
	. cpdb_fetch_one_value("select sum(body) from debata_tym left join debata on debata.debata_ID = debata_tym.debata_ID where debata.soutez_ID = :soutez_id and debata_tym.tym_ID = :tym_id", array(":soutez_id"=>$soutez_id, ":tym_id"=>$tym_id))
);

cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		dt_aff.body as aff_body,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		dt_neg.body as neg_body,
		teze.tx as teze,
		teze.teze_ID as teze_ID,
		datum,
		vitez,
		presvedcive,
		debata.debata_ID
	from
		debata
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		dt_aff.pozice = 1
		and dt_neg.pozice = 0
		and debata.soutez_ID = :soutez_id
		and (tym_aff.tym_ID = :tym_id or tym_neg.tym_ID = :tym_id)
	order by
		datum desc
	", array(":soutez_id"=>$soutez_id, ":tym_id"=>$tym_id),$r_debaty);
cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],$lang["pts"],""),array("class"=>"visible"));
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
		cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze"]),
		$result,
		( $debata["aff_ID"] == $tym_id ) ? $debata["aff_body"] : $debata["neg_body"],
		cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"])
	));
}
cph_table_end();
?>
