<?php
/*
 * greybox
 * $Id: soutez.kidy.inc,v 1.13 2005/03/23 20:05:48 che0 Exp $
 */

$soutez_id = $_REQUEST["soutez_id"];
$ukaz = isset($_REQUEST["ukaz"]) ? $_REQUEST["ukaz"] : "avg";

$soutez_nazev = cpdb_fetch_one_value("select nazev from soutez where soutez_ID = :soutez_id", array(":soutez_id"=>$soutez_id));

$GLOBALS["page"]->title = htmlspecialchars($soutez_nazev) . " / " . $lang["kidy"];
pg_head();

$j = ($ukaz == "judges") ? "_rozhodci" : "";
cph_p(
	$lang["competition"] . ": " . cphs_link("./?page=soutez&soutez_id=" . $soutez_id, $soutez_nazev)
	. cphs_br() . $lang["kidy"] . ": " . cphs_link("./?page=lide&kdo=kidy" . $j, $lang["global rankings"])
);

cph_switch($lang["list"], "./?page=soutez.kidy&soutez_id=" . $soutez_id . "&ukaz=%s", array("avg"=>$lang["average"], "judges"=>$lang["judges"]." - ".$lang["include panels"], "judges_single"=>$lang["judges"]." - ".$lang["single judges"]), $ukaz);

switch ($ukaz) {
	case "judges":
	case "judges_single":
		// show average point given by the judges
		
		switch ($ukaz) {
			case "judges_single":
				$filtr_sql = "
					and (
						select count(*) 
						from clovek_debata cd_r2
						where
							cd_r2.debata_ID = debata.debata_ID
							and cd_r2.role = 'r'
					) = 1
				";
			break;
			
			case "judges":
				$filtr_sql = "";
			break;
		}
		
		cpdb_fetch_all(sprintf("
			select
				clovek.clovek_ID,
				jmeno,
				nick,
				prijmeni,
				avg(cd_hodnoceni.kidy) as avg,
				count(cd_hodnoceni.kidy) as samples,
				std(cd_hodnoceni.kidy) as stddev
			from
				rozhodci
				left join clovek
					on rozhodci.clovek_ID = clovek.clovek_ID
				left join clovek_debata cd_rozhoduje
					on clovek.clovek_ID = cd_rozhoduje.clovek_ID and cd_rozhoduje.role = 'r'
				left join debata
					on cd_rozhoduje.debata_ID = debata.debata_ID
				left join clovek_debata cd_hodnoceni
					on cd_rozhoduje.debata_ID = cd_hodnoceni.debata_ID and find_in_set(cd_hodnoceni.role,'a1,a2,a3,n1,n2,n3')
			where
				debata.soutez_ID = :soutez_id
				%s
			group by
				clovek.clovek_ID, jmeno, nick, prijmeni
			having
				samples > 0
			order by
				avg desc
			", $filtr_sql), array(":soutez_id"=>$soutez_id), $r_kidy);
		
		de_html($r_kidy);
		cph_table_head(array($lang["name"], $lang["average"], $lang["samples"], $lang["stddev"]));
		foreach ($r_kidy as $kidy) {
			cph_table_row(array(
				cphs_link("./?page=clovek&clovek_id=" . $kidy["clovek_ID"], join_name($kidy["jmeno"],$kidy["nick"],$kidy["prijmeni"])),
				$kidy["avg"],
				$kidy["samples"],
				$kidy["stddev"]
			),
				$kidy["samples"] >= $GLOBALS["cp_config"]["min_kidy_judge"] ?
					array() : array("class"=>"inactive") 
			);
		}
		cph_table_end();

	break;
	
	case "avg";
		// show point of debaters
		cph_p($lang["ranks only for full members"], array("class"=>"note"));
		
		cpdb_fetch_all("
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
				and clovek.clen = 1
				and debata.soutez_ID = :soutez_id
			group by
				clovek_ID, jmeno, nick, prijmeni, klub, klub_ID
			having
				samples > 0
			order by
				avg desc
			", array(":soutez_id"=>$soutez_id), $r_kidy);
		
		de_html($r_kidy);
		cph_table_head(array($lang["rank"], $lang["rank"], $lang["debater"], $lang["club"], $lang["average"], $lang["samples"]));
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
				cphs_link("./?page=klub&klub_id=" . $kidy["klub_ID"], $kidy["klub"]),
				$kidy["avg"],
				$kidy["samples"]
			),
				$fullrank ? array() : array("class"=>"inactive") 
			);
		}
		cph_table_end();
	break;
}

?>
