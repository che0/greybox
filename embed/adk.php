<?php

include_once("../lib/config.inc");
include_once("../lib/cphtml.inc");
include_once("../lib/dblink.inc");
include_once("../lib/cpdb.inc");
include_once("../lib/misc.inc");

function sconv($tx) {
	return iconv("UTF-8", "Windows-1250", $tx);
}

cph_tag_start("div",array("id"=>"greybox_block"));
cph_tag("h2",array("id"=>"greybox_title"),cphs_link("greybox/","greybox"));

cpdb_fetch_all("select soutez_ID, nazev from soutez where zamceno = 0", array(), $souteze);
de_html($souteze);

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
	
		cph_table_head(array("#",sconv("tým"),"bdy"),array("class"=>"visible"));
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

cph_tag_end("div");

?>
