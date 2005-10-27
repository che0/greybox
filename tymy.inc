<?php
/*
 * greybox
 * $Id: tymy.inc,v 1.9 2005/10/27 12:45:44 che0 Exp $
 */

pg_head();

if ($GLOBALS["cps_kluby"] >= 2) cph_p(cphs_link("./?page=tym.add" . $klub_id,$lang['add team']));

cpdb_fetch_all("select tym_ID, tym.nazev as tym, klub.klub_ID, klub.nazev as klub from tym left join klub using (klub_ID) order by tym.nazev",array(":klub_id"=>$klub_id),$tymy);

de_html($tymy);

cpdb_fetch_all("
	select
		clovek_tym.tym_ID,
		count(clovek.clovek_ID) as pocet_clenu,
		sum(clovek_tym.aktivni) as pocet_aktivnich
	from
		clovek_tym
		left join clovek on clovek_tym.clovek_ID = clovek.clovek_ID
	group by
		clovek_tym.tym_ID
	",
	array(), $tym_debateri, "tym_ID");

cpdb_fetch_all("
	select
		tym_ID,
		count(*) as pocet_debat
	from
		debata_tym
	group by
		tym_ID
	",
	array(), $tym_debaty, "tym_ID");

cpdb_fetch_all("
	select
		tym_ID,
		count(*) as pocet_debat
	from
		debata_tym
		left join debata on debata_tym.debata_ID = debata.debata_ID
		left join soutez on debata.soutez_ID = soutez.soutez_ID
	where
		soutez.rocnik = :rocnik
	group by
		tym_ID
	",
	array(":rocnik"=>$GLOBALS["current_season"]), $tym_debaty_letos, "tym_ID");


cph_table_head(array($lang["team"],$lang["club"],$lang["members"],$lang["debates"]));
foreach ($tymy as $tym) {
	cph_table_row(array(
		cphs_link("./?page=tym&tym_id=" . $tym["tym_ID"], $tym["tym"]),
		cphs_link("./?page=klub&klub_id=" . $tym["klub_ID"], $tym["klub"]),
		(int)$tym_debateri[$tym["tym_ID"]]["pocet_aktivnich"] . " (" . (int)$tym_debateri[$tym["tym_ID"]]["pocet_clenu"] . ")",
		(int)$tym_debaty_letos[$tym["tym_ID"]]["pocet_debat"] . " (" . (int)$tym_debaty[$tym["tym_ID"]]["pocet_debat"] . ")"
	), 
	(((int)$tym_debateri[$tym["tym_ID"]]["pocet_aktivnich"] >= 3)
		&& ((int)$tym_debaty_letos[$tym["tym_ID"]]["pocet_debat"] >= 1))
		? array() : array("class"=>"inactive"));
}
cph_table_end();

?>
