<?php

$turnaj_id = $_REQUEST["turnaj_id"];

$GLOBALS["page"]->title = htmlspecialchars(cpdb_fetch_one_value("select nazev from turnaj where turnaj_ID = :turnaj_id", array(":turnaj_id"=>$turnaj_id))) . " / " . $lang["kidy"];
pg_head();

cpdb_fetch_all(sprintf("
	select
		clovek.clovek_ID,
		clovek.jmeno,
		clovek.nick,
		clovek.prijmeni,
		klub.kratky_nazev as klub,
		klub.klub_ID,
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
		and debata.turnaj_ID = :turnaj_id
	group by
		clovek_ID, jmeno, nick, prijmeni, klub, klub_ID
	having
		samples > 0
	order by
		avg desc
	", $ukaz), array(":turnaj_id"=>$turnaj_id), $r_kidy);

de_html($r_kidy);
cph_table_head(array($lang["rank"], $lang["debater"], $lang["club"], $lang["average"], $lang["samples"]),array("class"=>"visible"));
$count = 0; $rank = 1; $pts = -1;
foreach ($r_kidy as $kidy) {
	$count++;
	if ($pts != $kidy["avg"]) {
		$pts = $kidy["avg"];
		$rank = $count;
	}
	cph_table_row(array(
		$rank,
		cphs_link("./?page=clovek&clovek_id=" . $kidy["clovek_ID"], join_name($kidy["jmeno"],$kidy["nick"],$kidy["prijmeni"])),
		cphs_link("./?page=klub&klub_id=" . $kidy["klub_ID"], $kidy["klub"]),
		$kidy["avg"],
		$kidy["samples"]
	));
}
cph_table_end();

?>