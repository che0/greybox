<?php

$kdo = isset($_REQUEST["kdo"]) ? $_REQUEST["kdo"] : "vsichni";

$kdo_domain = array("vsichni"=>$lang["everyone"], "debateri"=>$lang["debaters"], "rozhodci"=>$lang["judges"]);
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
				klub.kratky_nazev as klub
			from 
				clovek
				left join klub using (klub_ID)
			order by
				clovek.prijmeni, clovek.jmeno, clovek.nick
		", array(), $lide);
		
		de_html($lide);
		cph_table_head(
			array($lang["name"], $lang["club"], $lang["ipoints"]),
			array("class"=>"visible")
		);
		
		foreach ($lide as $clovek) {
			cph_table_row(array(
				cphs_link("./?page=clovek&clovek_id=" . $clovek["clovek_ID"],join_name($clovek["jmeno"],$clovek["nick"],$clovek["prijmeni"])),
				cphs_link("./?page=klub&klub_id=" . $clovek["klub_ID"],$clovek["klub"]),
				cpdb_fetch_one_value("select sum(ibody) from clovek_debata_ibody where clovek_ID = :clovek_id and rocnik = :rocnik", array(":clovek_id"=>$clovek["clovek_ID"], ":rocnik"=>$GLOBALS["current_season"]))
			));
		}
		
		cph_table_end();
	break;
	
	
	case 'debateri':
		// show list of debaters
		
		cpdb_fetch_all("
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
		", array(), $lide);
		
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
}

?>