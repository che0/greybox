<?php
/*
 * greybox
 * $Id: klub.kidy.inc,v 1.1 2005/02/15 14:33:34 che0 Exp $
 */

$klub_id = $_REQUEST["klub_id"];
$klub_nazev = cpdb_fetch_one_value("select nazev from klub where klub_ID = :klub_id", array(":klub_id"=>$klub_id));

if ($klub_nazev === false) {
	pg_achtung($lang["no record"]);
	return;
}

$GLOBALS["page"]->title = $klub_nazev . " / " . $lang["kidy overview"];

pg_head();

// show individual rankings

if (! cpdb_fetch_all("
	select
		clovek.clovek_ID,
		clovek.jmeno,
		clovek.nick,
		clovek.prijmeni,
		avg(kidy) as avg,
		count(kidy) as samples
	from
		clovek
		left join clovek_debata on clovek.clovek_ID = clovek_debata.clovek_ID
			and find_in_set(clovek_debata.role,'a1,a2,a3,n1,n2,n3')
		left join debata on clovek_debata.debata_ID = debata.debata_ID
		left join soutez on debata.soutez_ID = soutez.soutez_ID
	where
		clovek.klub_ID = :klub_id
		and clovek.debater = 1
		and clovek.clen = 1
		and soutez.rocnik = :rocnik
	group by
		clovek_ID, jmeno, nick, prijmeni
	having
		samples > 0
	order by
		avg desc
	", array(":rocnik"=>$GLOBALS["current_season"], ":klub_id"=>$klub_id), $r_kidy)) {
	
	pg_achtung($lang["no record"]);
	return;
}

cph_p($lang["ranks only for full members"], array("class"=>"note"));

de_html($r_kidy);
cph_table_head(array($lang["rank"], $lang["rank"], $lang["debater"], $lang["average"], $lang["samples"]),array("class"=>"visible"));
$count = 0; $rank = 1; $pts = -1;
$countF = 0; $rankF = 1; $ptsF = -1;
foreach ($r_kidy as $kidy) {
	$count++;
	if ($pts != $kidy["avg"]) {
		$pts = $kidy["avg"];
		$rank = $count;
	}
	$fullrank = ($kidy["samples"] >= $GLOBALS["cp_config"]["min_kidy_rank"]);
	if ($fullrank) {
		$countF++;
		if ($ptsF != $kidy["avg"]) {
		$ptsF = $kidy["avg"];
		$rankF = $countF;
		}
	}
	cph_table_row(array(
		$fullrank ? $rankF : "",
		$rank,
		cphs_link("./?page=clovek&clovek_id=" . $kidy["clovek_ID"], join_name($kidy["jmeno"],$kidy["nick"],$kidy["prijmeni"])),
		$kidy["avg"],
		$kidy["samples"]
	),
		$fullrank ? array() : array("class"=>"inactive") 
	);
}

cph_table_end();

?>
