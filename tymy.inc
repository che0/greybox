<?php
pg_head();

if ($GLOBALS["cps_kluby"] >= 2) cph_p(cphs_link("./?page=tym.add" . $klub_id,$lang['add team']));

cpdb_fetch_all("select tym_ID, tym.nazev as tym, klub.klub_ID, klub.nazev as klub from tym left join klub using (klub_ID) order by tym.nazev",array(":klub_id"=>$klub_id),$tymy);
de_html($tymy);
cph_table_head(array($lang["team"],$lang["club"],$lang["debaters"]),array("class"=>"visible"));
foreach ($tymy as $tym) {
	$debaters = cpdb_fetch_one_value("
		select
			count(clovek.clovek_ID)
		from
			clovek_tym
			left join clovek using(clovek_ID)
		where
			clovek_tym.aktivni = 1
			and clovek.debater = 1
			and clovek_tym.tym_ID = :tym_id
		", array(":tym_id"=>$tym["tym_ID"]));
	cph_table_row(array(
		cphs_link("./?page=tym&tym_id=" . $tym["tym_ID"], $tym["tym"]),
		cphs_link("./?page=klub&klub_id=" . $tym["klub_ID"], $tym["klub"]),
		$debaters
	), ($debaters >= 3) ? array() : array("class"=>"inactive"));
}
cph_table_end();

?>