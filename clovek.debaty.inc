<?php
/*
 * greybox
 * $Id: clovek.debaty.inc,v 1.1 2005/10/27 12:10:46 che0 Exp $
 */

include_once("i_domeny.inc");

$clovek_id = $_REQUEST["clovek_id"];

if (!cpdb_fetch_one_row("select jmeno, nick, prijmeni from clovek where clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id), $clovek_p)) {
	pg_achtung($lang["no record"]);
	return;
}
$clovek = de_html_s($clovek_p);

$GLOBALS["page"]->title = join_name($clovek["jmeno"], $clovek["nick"], $clovek["prijmeni"]) . " / " . $lang["debates"];
pg_head();

// debates
$total_debaty = cpdb_fetch_all("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		teze.tx as teze,
		teze.tx_short as teze_short,
		teze.teze_ID as teze_ID,
		debata.datum,
		debata.vitez,
		debata.presvedcive,
		debata.achtung,
		debata.debata_ID,
		clovek_debata.role,
		clovek_debata.kidy,
		clovek_debata.rozhodnuti as adj_rozhodnuti,
		clovek_debata.presvedcive as adj_presvedcive
	from
		clovek_debata
		left join debata on debata.debata_ID = clovek_debata.debata_ID
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
	where
		clovek_debata.clovek_ID = :clovek_id
	order by
		datum desc
	", array(":clovek_id"=>$clovek_id, ":rocnik"=>$GLOBALS["current_season"]),$r_debaty
);

cph_p(sprintf($lang["%s: %s total debates"], cphs_link("./?page=clovek&clovek_id=" . $clovek_id, join_name($clovek["jmeno"], $clovek["nick"], $clovek["prijmeni"])), $total_debaty));

if ($total_debaty) {
	// if there are any debates at all...
	
	de_html($r_debaty);
	cph_table_head(array($lang["date"],$lang["aff"],$lang["neg"],$lang["resolution"],$lang["result"],"",$lang["role"],""));
	foreach ($r_debaty as $debata) {
		if ($debata["vitez"] == 1) { $result = $lang["aff"]; } else { $result = $lang["neg"]; }
		$result .= " ";
		if ($debata["presvedcive"] == 1) { $result .= $lang["3:0"]; } else { $result .= $lang["2:1"]; }
		switch ($debata["role"]) {
			case "o": // organizer
				$detaily = "";
			break;
			case "r": // adjudicator
				$detaily = ($debata["adj_rozhodnuti"] ? "A" : "N") . ($debata["adj_presvedcive"] ? "+" : "-");
			break;
			default: // debater
				$detaily = $debata["kidy"];
			break;
		}
		cph_table_row(array(
			format_date($debata["datum"]),
			cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"],$debata["aff_tym"]),
			cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"],$debata["neg_tym"]),
			cphs_link("./?page=teze.detaily&teze_id=" . $debata["teze_ID"],$debata["teze_short"],array("title"=>$debata["teze"])),
			$result,
			cphs_link("./?page=debata&debata_id=" . $debata["debata_ID"], $lang["debate details"]),
			$domain_debata_role[$debata["role"]],
			$detaily
		),
			$debata["achtung"] ? array("class"=>"achtung") : array()
		);
	}
	cph_table_end();
}

?>
