<?php

// print out club list
pg_head();

if ($prava_kluby >= 2) {
	cph_p(cphs_link("./?page=klub&akce=pridej",$lang['add club']));
}

cph_table_head(array($lang["club"],$lang["place"],$lang["debaters"]),array("class"=>"visible"));
cpdb_fetch_all("select klub_ID, nazev, misto from klub order by nazev",array(),$kluby);
foreach ($kluby as $klub) {
	cph_table_row(array(
		cphs_link("./?page=klub&klub_id=" . $klub['klub_ID'],$klub['nazev']),
		$klub["misto"],
		cpdb_fetch_one_value("select count(clovek_ID) from clovek where debater = 1 and klub_ID = :klub_id", array(":klub_id"=>$klub["klub_ID"]))
	));
}
cph_table_end();

?>