<?php
/*
 * greybox
 * $Id: soutez.kidy.inc,v 1.6 2005/01/10 01:34:06 che0 Exp $
 */

$soutez_id = $_REQUEST["soutez_id"];
$ukaz = isset($_REQUEST["ukaz"]) ? $_REQUEST["ukaz"] : "sum";

$GLOBALS["page"]->title = htmlspecialchars(cpdb_fetch_one_value("select nazev from soutez where soutez_ID = :soutez_id", array(":soutez_id"=>$soutez_id))) . " / " . $lang["kidy"];
pg_head();

cph_switch($lang["list"], "./?page=soutez.kidy&soutez_id=" . $soutez_id . "&ukaz=%s", array("sum"=>$lang["sum"], "avg"=>$lang["average"], "judges"=>$lang["judges"]), $ukaz);

if ($ukaz == "judges") {
	// show average point given by the judges
	cpdb_fetch_all("
		select
			clovek.clovek_ID,
			jmeno,
			nick,
			prijmeni,
			avg(cd_hodnoceni.kidy) as avg,
			count(cd_hodnoceni.kidy) as samples
		from
			clovek
			left join clovek_debata cd_rozhoduje
				on clovek.clovek_ID = cd_rozhoduje.clovek_ID and cd_rozhoduje.role = 'r'
			left join debata
				on cd_rozhoduje.debata_ID = debata.debata_ID
			left join clovek_debata cd_hodnoceni
				on cd_rozhoduje.debata_ID = cd_hodnoceni.debata_ID and find_in_set(cd_hodnoceni.role,'a1,a2,a3,n1,n2,n3')
		where
			debata.soutez_ID = :soutez_id
		group by
			clovek.clovek_ID, jmeno, nick, prijmeni
		having
			samples > 0
		order by
			avg desc
		",array(":soutez_id"=>$soutez_id), $r_kidy);
	
	de_html($r_kidy);
	cph_table_head(array($lang["name"], $lang["average"], $lang["samples"]),array("class"=>"visible"));
	foreach ($r_kidy as $kidy) {
		cph_table_row(array(
			cphs_link("./?page=clovek&clovek_id=" . $kidy["clovek_ID"], join_name($kidy["jmeno"],$kidy["nick"],$kidy["prijmeni"])),
			$kidy["avg"],
			$kidy["samples"]
		));
	}
	cph_table_end();

} else {
	// show point of debaters
	cph_p($lang["ranks only for full members"]);
	
	$ukaz = ($ukaz == "sum") ? "sum" : "avg";
	
	cpdb_fetch_all(sprintf("
		select
			clovek.clovek_ID,
			clovek.jmeno,
			clovek.nick,
			clovek.prijmeni,
			klub.kratky_nazev as klub,
			klub.klub_ID,
			sum(kidy) as sum,
			avg(kidy) as avg,
			count(kidy) as samples
		from
			clovek
			left join klub on clovek.klub_ID = klub.klub_ID
			left join clovek_debata on clovek.clovek_ID = clovek_debata.clovek_ID
				and find_in_set(clovek_debata.role,'a1,a2,a3,n1,n2,n3')
			left join debata on clovek_debata.debata_ID = debata.debata_ID
		where
			clovek.debater = 1
			and clovek.clen = 1
			and debata.soutez_ID = :soutez_id
		group by
			clovek_ID, jmeno, nick, prijmeni, klub, klub_ID
		having
			samples > 0
		order by
			%s desc
		", $ukaz), array(":soutez_id"=>$soutez_id), $r_kidy);
	
	de_html($r_kidy);
	cph_table_head(array($lang["rank"], $lang["debater"], $lang["club"], $lang["sum"], $lang["average"], $lang["samples"]),array("class"=>"visible"));
	$count = 0; $rank = 1; $pts = -1;
	foreach ($r_kidy as $kidy) {
		$count++;
		if ($pts != $kidy[$ukaz]) {
			$pts = $kidy[$ukaz];
			$rank = $count;
		}
		cph_table_row(array(
			$rank,
			cphs_link("./?page=clovek&clovek_id=" . $kidy["clovek_ID"], join_name($kidy["jmeno"],$kidy["nick"],$kidy["prijmeni"])),
			cphs_link("./?page=klub&klub_id=" . $kidy["klub_ID"], $kidy["klub"]),
			$kidy["sum"],
			$kidy["avg"],
			$kidy["samples"]
		));
	}
	cph_table_end();
}
	
?>
