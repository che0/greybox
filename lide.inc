<?php
/*
 * greybox
 * $Id: lide.inc,v 1.14 2005/02/13 12:13:57 che0 Exp $
 */

$kdo = isset($_REQUEST["kdo"]) ? $_REQUEST["kdo"] : "vsichni";

$kdo_domain = array("vsichni"=>$lang["everyone"], "debateri"=>$lang["debaters"], "rozhodci"=>$lang["judges"], "spravci"=>$lang["administrators"], "ibody"=>$lang["points overview"], "kidy"=>$lang["kidy"], "stats"=>$lang["stats"]);
$GLOBALS["page"]->title = $kdo_domain[$kdo];

pg_head();

cph_switch($lang["list"],"./?page=lide&kdo=%s", $kdo_domain, $kdo);

switch ($kdo) {
	case 'vsichni':
	// show list of people
		if ($GLOBALS["cps_lidi"] >= 1) {
			// if user can add new people, this is the page he can do it
			cph_p(cphs_link("./?page=clovek.add&klub_id=" . $_SESSION["user_klub_ID"], $lang['add person']));
		}
		
		cpdb_fetch_all("
			select
				clovek.clovek_ID,
				clovek.jmeno,
				clovek.nick,
				clovek.prijmeni,
				clovek.klub_ID,
				clovek.clen,
				klub.kratky_nazev as klub,
				sum(clovek_debata_ibody.ibody) as ibody
			from 
				clovek
				left join klub on clovek.klub_ID = klub.klub_ID
				left join clovek_debata_ibody
					on clovek.clovek_ID = clovek_debata_ibody.clovek_ID
					and clovek_debata_ibody.rocnik = :rocnik
			group by
				clovek.clovek_ID, clovek.jmeno, clovek.nick,
				clovek.prijmeni, clovek.klub_ID, klub.kratky_nazev
			order by
				clovek.prijmeni, clovek.jmeno, clovek.nick
		", array(":rocnik"=>$GLOBALS["current_season"]), $lide);
		
		de_html($lide);
		cph_table_head(
			array($lang["name"], $lang["club"], $lang["full member"], 	$lang["ipoints"]),
			array("class"=>"visible")
		);
		
		foreach ($lide as $clovek) {
			cph_table_row(array(
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"],join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],$clovek["klub"]),
				$clovek["clen"] ? $lang["yes"] : $lang["no"],
				$clovek["ibody"]
			),
				$clovek["clen"] ? array() : array("class"=>"inactive")
			);
		}
		
		cph_table_end();
	break;
	
	case 'ibody':
	// show individual points rankings
		if (! cpdb_fetch_all("
			select
				clovek.clovek_ID,
				clovek.jmeno,
				clovek.nick,
				clovek.prijmeni,
				clovek.klub_ID,
				klub.kratky_nazev as klub,
				sum(clovek_debata_ibody.ibody) as ibody
			from 
				clovek
				left join klub on clovek.klub_ID = klub.klub_ID
				left join clovek_debata_ibody
					on clovek.clovek_ID = clovek_debata_ibody.clovek_ID
			where
				clovek.clen = 1
				and clovek_debata_ibody.rocnik = :rocnik
			group by
				clovek.clovek_ID, clovek.jmeno, clovek.nick,
				clovek.prijmeni, clovek.klub_ID, klub.kratky_nazev
			having
				ibody > 0
			order by
				ibody desc, clovek.prijmeni, clovek.jmeno, clovek.nick
		", array(":rocnik"=>$GLOBALS["current_season"]), $lide)) {
		
			pg_achtung($lang["no record"]);
			return;
		}
		
		
		cph_p($lang["ranks only for full members"], array("class"=>"note"));
		
		de_html($lide);
		cph_table_head(
			array($lang["rank"], $lang["name"], $lang["club"], $lang["ipoints"]),
			array("class"=>"visible")
		);
		
		$count = 0; $rank = 1; $pts = -1;
		foreach ($lide as $clovek) {
			$count++;
			if ($pts != $clovek["ibody"]) {
				$pts = $clovek["ibody"];
				$rank = $count;
			}
			cph_table_row(array(
				$rank,
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"],join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],$clovek["klub"]),
				$clovek["ibody"]
			));
		}
		
		cph_table_end();
	break;
	
	case 'kidy':
		// show kidy rankings
		
		if (! cpdb_fetch_all("
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
				left join soutez on debata.soutez_ID = soutez.soutez_ID
			where
				clovek.debater = 1
				and clovek.clen = 1
				and soutez.rocnik = :rocnik
			group by
				clovek_ID, jmeno, nick, prijmeni, klub, klub_ID
			having
				samples > 0
			order by
				avg desc
			", array(":rocnik"=>$GLOBALS["current_season"]), $r_kidy)) {
			
			pg_achtung($lang["no record"]);
			return;
		}
		
		cph_p($lang["ranks only for full members"], array("class"=>"note"));
		
		de_html($r_kidy);
		cph_table_head(array($lang["rank"], $lang["rank"], $lang["debater"], $lang["club"], $lang["average"], $lang["samples"]),array("class"=>"visible"));
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
				cphs_link("./?page=klub&klub_id=" . $kidy["klub_ID"], $kidy["klub"]),
				$kidy["avg"],
				$kidy["samples"]
			),
				$fullrank ? array() : array("class"=>"inactive") 
			);
		}
		cph_table_end();

	break;
	
	case 'debateri':
		// show list of debaters
		
		if (! cpdb_fetch_all("
			select
				clovek.clovek_ID,
				clovek.jmeno,
				clovek.nick,
				clovek.prijmeni,
				clovek.klub_ID,
				klub.kratky_nazev as klub
			from 
				clovek
				left join klub using (klub_ID)
			where
				clovek.debater = 1
			order by
				clovek.prijmeni, clovek.jmeno, clovek.nick
		", array(), $lide)) {
			
			pg_achtung($lang["no record"]);
			return;
		}
		
		de_html($lide);
		cph_table_head(
			array($lang["name"], $lang["club"], $lang["team"], $lang["ipoints"]),
			array("class"=>"visible")
		);
		
		foreach ($lide as $clovek) {
			cpdb_fetch_all("
				select
					tym.tym_ID, tym.nazev
				from
					clovek_tym left join tym using (tym_ID)
				where
					clovek_tym.clovek_ID = :clovek_id
					and clovek_tym.aktivni = 1
			", array(":clovek_id"=>$clovek["clovek_ID"]), $tymy);
			de_html($tymy);
			$prvni = true; $f_tymy = "";
			foreach ($tymy as $tym) {
				if ($prvni) {
					$prvni = false;
				} else {
					$f_tymy .= ", ";
				}
				$f_tymy .= cphs_link("./?page=tym&tym_id=" . $tym["tym_ID"], $tym["nazev"]);
			}
			
			cph_table_row(array(
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"],join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],$clovek["klub"]),
				$f_tymy,
				cpdb_fetch_one_value("select sum(ibody) from clovek_debata_ibody where clovek_ID = :clovek_id and rocnik = :rocnik and role = 'debater'", array(":clovek_id"=>$clovek["clovek_ID"], ":rocnik"=>$GLOBALS["current_season"]))
			));
		}
		
		cph_table_end();
	break;

	
	case 'rozhodci':
		cpdb_fetch_all("
			select
				clovek.clovek_ID,
				clovek.jmeno,
				clovek.nick,
				clovek.prijmeni,
				clovek.klub_ID,
				rozhodci.misto,
				rozhodci.jazyk,
				klub.kratky_nazev as klub
			from 
				clovek
				left join klub on klub.klub_ID = clovek.klub_ID
				left join rozhodci on rozhodci.clovek_ID = clovek.clovek_ID
			where
				rozhodci.rocnik = :rocnik
			order by
				clovek.prijmeni, clovek.jmeno, clovek.nick
		", array(":rocnik"=>$GLOBALS["current_season"]), $lide);
		
		de_html($lide);
		cph_table_head(
			array($lang["name"], $lang["club"], $lang["languages"], $lang["place"], $lang["ipoints"]),
			array("class"=>"visible")
		);
		
		foreach ($lide as $clovek) {
			cph_table_row(array(
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"],join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],$clovek["klub"]),
				$clovek["jazyk"],
				$clovek["misto"],
				cpdb_fetch_one_value("select sum(ibody) from clovek_debata_ibody where clovek_ID = :clovek_id and rocnik = :rocnik and role = 'rozhodci'", array(":clovek_id"=>$clovek["clovek_ID"], ":rocnik"=>$GLOBALS["current_season"]))
			));
		}
		cph_table_end();
	break;
	
	case 'spravci':
		// show list of admins
		
		include_once("i_domeny.inc");
		
		cpdb_fetch_all("
			select
				clovek.clovek_ID,
				clovek.jmeno,
				clovek.nick,
				clovek.prijmeni,
				clovek.klub_ID,
				klub.kratky_nazev as klub,
				clovek.prava_lidi,
				clovek.prava_kluby,
				clovek.prava_souteze,
				clovek.prava_debaty
			from 
				clovek
				left join klub using (klub_ID)
			where
				clovek.prava_lidi +	clovek.prava_kluby + clovek.prava_souteze + clovek.prava_debaty > 0
			order by
				clovek.prijmeni, clovek.jmeno, clovek.nick
		", array(), $lide);
		
		de_html($lide);
		cph_table_head(
			array($lang["administrator"], $lang["people"], $lang["clubs"], $lang["competitions"], $lang["debates"]),
			array("class"=>"visible")
		);
		
		foreach ($lide as $clovek) {
			if ($clovek["prava_lidi"] == 1) {
				$prava_lidi = cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"], $clovek["klub"]);
			} else {
				$prava_lidi = $domain_prava_lidi[$clovek["prava_lidi"]];
			}
			if ($clovek["prava_kluby"] == 1) {
				$prava_kluby = cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"], $clovek["klub"]);
			} else {
				$prava_kluby = $domain_prava_kluby[$clovek["prava_kluby"]];
			}
			
			cph_table_row(array(
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"],join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				$prava_lidi,
				$prava_kluby,
				$domain_prava_souteze[$clovek["prava_souteze"]],
				$domain_prava_debaty[$clovek["prava_debaty"]]
			));
		}
		
		cph_table_end();
	break;
	
	case 'stats':
		// show people stats
		
		cph_tag_start("table", array("class"=>"visible"));
		cph_table_row(array(
			$lang["total number of people in the database"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek")
		));
		cph_table_row(array(
			$lang["active debaters"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where debater = 1")
		));
		cph_table_row(array(
			$lang["active judges"],
			cpdb_fetch_one_value("select count(distinct clovek_ID) from rozhodci where rocnik = :rocnik", array(":rocnik"=>$GLOBALS["current_season"]))
		));
		cph_table_row(array(
			$lang["full members"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where clen = 1")
		));
		cph_table_end();
		
		cph_h2($lang["full members by age"]);
		cph_tag_start("table", array("class"=>"visible"));
		cph_table_row(array(
			$lang["<15"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where clen = 1 and narozen > date_sub(now(), interval 15 year)")
		));
		cph_table_row(array(
			$lang["15-18"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where clen = 1 and narozen <= date_sub(now(), interval 15 year) and narozen > date_sub(now(), interval 18 year)")
		));
		cph_table_row(array(
			$lang["18-26"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where clen = 1 and narozen <= date_sub(now(), interval 18 year) and narozen > date_sub(now(), interval 26 year)")
		));
		cph_table_row(array(
			$lang["26+"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where clen = 1 and narozen <= date_sub(now(), interval 26 year)")
		));
		cph_table_row(array(
			$lang["unset"],
			cpdb_fetch_one_value("select count(clovek_ID) from clovek where clen = 1 and narozen is null")
		));
		cph_table_end();
	break;
}

?>
