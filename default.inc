<?php
pg_head();

if ($lang['release notes']) cph_p($lang['release notes']);

if ($_SESSION['is_logged_in']) {
	cph_tag("h2",array(),cphs_link("./?page=clovek",$_SESSION['user_name']));
	
	$klub_nazev = cpdb_fetch_one_value("select nazev from klub where klub_ID = :klub_ID",array(":klub_ID"=>$_SESSION['user_klub_ID']));
	
	if ($klub_nazev) {
		cph_p(cphs_link(sprintf("./?page=klub&klub_id=%s",$_SESSION['user_klub_ID']),$klub_nazev));
	}

	cpdb_fetch_all(
		"select tym.tym_ID as a_tym_ID,	tym.nazev as a_tym
			from clovek_tym left join tym using (tym_ID)
			where clovek_tym.clovek_ID = :clovek_ID and clovek_tym.aktivni = 1",
		array(":clovek_ID"=>$_SESSION['user_clovek_ID']),
		$tymy);
	
	$prvni = true;
	foreach($tymy as $tym) {
		if ($prvni) {
			cph_tag_start("p");
			$prvni = false;
		} else {
			echo ", ";
		}
		cph_link(sprintf("./?page=tym&tym_id=%s",$tym["a_tym_ID"]),$tym["a_tym"]);
	}
	if (! $prvni) {
		cph_tag_end("p");
	}
}

cph_tag("div", array("id"=>"footnote"), $lang['tail_footnote']);

?>
