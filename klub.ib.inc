<?php
/*
 * greybox
 * $Id: klub.ib.inc,v 1.5 2006/08/23 17:07:33 che0 Exp $
 */

$klub_id = $_REQUEST["klub_id"];
$klub_nazev = cpdb_fetch_one_value("select nazev from klub where klub_ID = :klub_id", array(":klub_id"=>$klub_id));

if ($klub_nazev === false) {
	pg_achtung($lang["no record"]);
	return;
}

$GLOBALS["page"]->title = htmlspecialchars($klub_nazev) . " / " . $lang["points overview"];

pg_head();

cph_p(
	$lang["club"] . ": " . cphs_link("./?page=klub&klub_id=" . $klub_id, $klub_nazev)
	. cphs_br() . $lang["ipoints"] . ": " . cphs_link("./?page=lide&kdo=ibody", $lang["global rankings"])
);

// show individual points rankings

if (! cpdb_fetch_all("
	select
		clovek_ID,
		jmeno,
		nick,
		prijmeni,
		(
			ifnull((select sum(ibody) from clovek_debata_ibody where clovek_ID = clovek.clovek_ID and rocnik = :rocnik),
			0)
			+ ifnull((select sum(ibody) from clovek_ibody where clovek_ID = clovek.clovek_ID and rocnik = :rocnik), 0)
		) as ibody
	from
		clovek
	where
		clen = 1
		and clovek.klub_ID = :klub_id
	group by
		clovek_ID, jmeno, nick,
		prijmeni
	having
		ibody > 0
	order by
		ibody desc, prijmeni, jmeno, nick
", array(":rocnik"=>$GLOBALS["current_season"], ":klub_id"=>$klub_id), $lide)) {
	pg_achtung($lang["no record"]);
	return;
}

cph_p($lang["ranks only for full members"], array("class"=>"note"));

de_html($lide);
cph_table_head(array($lang["rank"], $lang["name"], $lang["ipoints"]));

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
		$clovek["ibody"]
	));
}

cph_table_end();

?>
