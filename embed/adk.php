<?php

$GLOBALS["embed"] = true;

include_once("greybox/lib/config.inc");
include_once("greybox/lib/cphtml.inc");
include_once("greybox/lib/dblink.inc");
include_once("greybox/lib/cpdb.inc");
include_once("greybox/lib/misc.inc");

function sconv($tx) {
	return iconv("UTF-8", "Windows-1250", $tx);
}

cph_tag_start("div",array("id"=>"greybox_block"));
cph_tag("h2",array("id"=>"greybox_title"),cphs_link("greybox/","greybox"));

cpdb_fetch_all("select soutez_ID, nazev from soutez where zamceno = 0", array(), $souteze);
de_html($souteze);

// souteze
foreach ($souteze as $soutez) {
	cph_tag("h3",array(),cphs_link("greybox/?page=soutez&soutez_id=". $soutez["soutez_ID"], sconv($soutez["nazev"])));
	
	if (cpdb_fetch_all("
		select 
			sum(body) as bodu, tym.tym_ID, tym.nazev
		from
			debata
			left join debata_tym on debata.debata_ID = debata_tym.debata_ID
			left join tym on debata_tym.tym_ID = tym.tym_ID
		where
			debata.soutez_ID = :soutez_id
		group by
			tym.tym_ID, tym.nazev
		order by
			bodu desc
		limit 7
		",
		array(":soutez_id"=>$soutez["soutez_ID"]),$ranks)) {
		
		de_html($ranks);
	
		cph_table_head(array("#",sconv("tým"),"bdy"));
		$count = 0; $rank = 1; $pts = -1;
		foreach ($ranks as $team_rank) {
			$count++;
			if ($pts != $team_rank["bodu"]) {
				$pts = $team_rank["bodu"];
				$rank = $count;
			}
			cph_table_row(array(
				$rank,
				cphs_link("greybox/?page=tym&tym_id=" . $team_rank["tym_ID"], sconv($team_rank["nazev"])),
				cphs_link("greybox/?page=tym.soutez&tym_id=" . $team_rank["tym_ID"] . "&soutez_id=" . $soutez["soutez_ID"], $team_rank["bodu"])
			));
		}
		cph_table_end();
	} else {
		cph_p(sconv("zatím žádné výsledky"));
	}
}

// ligy
cpdb_fetch_all("select liga_ID, nazev from liga where rocnik = :rocnik", array(":rocnik"=>$GLOBALS["cp_config"]["default_season"]), $ligy);
de_html($ligy);

foreach ($ligy as $liga) {
	cph_tag("h3",array(),cphs_link("greybox/?page=liga&liga_id=". $liga["liga_ID"], sconv($liga["nazev"])));

	if (cpdb_fetch_all("
		select
			tym.tym_ID,
			tym.nazev,
			liga_tym.liga_vytezek,
			(
				select
					count(debata.debata_ID)
				from
					debata_tym
					inner join debata on debata_tym.debata_ID = debata.debata_ID
					inner join turnaj on debata.turnaj_ID = turnaj.turnaj_ID
				where
					debata_tym.tym_ID = tym.tym_ID -- vazba na vnejsi dotaz
					and turnaj.liga_ID = :liga_id
			) as pocet_debat
		from
			liga_tym
			left join tym on tym.tym_ID = liga_tym.tym_ID
		where
			liga_tym.liga_ID = :liga_id
		order by
			liga_vytezek desc
		limit 8
		", array(":liga_id"=>$liga["liga_ID"]), $ranks)) {
		
		cph_h2($lang["teams"]);
		de_html($ranks);
		cph_table_head(array("#", sconv("tým"), "tab"));
		$countF = 0; $rankF = 1; $ptsF = -1;
		foreach ($ranks as $team_rank) {
			$fullrank = ($team_rank["pocet_debat"] >= $GLOBALS["cp_config"]["min_league_debates"]);
			if ($fullrank) {
				$countF++;
				if ($ptsF != $team_rank["liga_vytezek"]) {
				$ptsF = $team_rank["liga_vytezek"];
				$rankF = $countF;
				}
			}
			cph_table_row(array(
				$fullrank ? $rankF : "",
				cphs_link("greybox/?page=tym&tym_id=" . $team_rank["tym_ID"], sconv($team_rank["nazev"])),
				cphs_link(sprintf("greybox/?page=tym.liga&tym_id=%s&liga_id=%s",$team_rank["tym_ID"],$liga["liga_ID"]), $team_rank["liga_vytezek"])
			),
				$fullrank ? array() : array("class"=>"inactive")
			);
		}
		cph_table_end();
	} else {
		cph_p(sconv("zatím žádné výsledky"));
	}
}


cph_tag_end("div");

?>
