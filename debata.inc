<?php

$debata_id = $_REQUEST["debata_id"];

if (!cpdb_fetch_one_row("
	select
		tym_aff.tym_ID as aff_ID,
		tym_aff.nazev as aff_tym,
		dt_aff.body as aff_body,
		tym_neg.tym_ID as neg_ID,
		tym_neg.nazev as neg_tym,
		dt_neg.body as neg_body,
		teze.tx as teze,
		teze.teze_ID as teze_ID,
		debata.datum,
		debata.vitez,
		debata.presvedcive,
		debata.debata_ID,
		soutez.soutez_ID,
		soutez.nazev as soutez,
		turnaj.turnaj_ID,
		turnaj.nazev as turnaj
	from
		debata
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
		left join soutez on soutez.soutez_ID = debata.soutez_ID
		left join turnaj on turnaj.turnaj_ID = debata.turnaj_ID
	where
		dt_aff.pozice = 1
		and dt_neg.pozice = 0
		and debata.debata_ID = :debata_id
	", array(":debata_id"=>$debata_id), $debata)) {
	
	pg_achtung($lang["no record"]);
	return;
}

$GLOBALS["page"]->title = format_date($debata["datum"]) . " " . $debata["aff_tym"] . "/" . $debata["neg_tym"];
pg_head();

if (isset($debata["komentar"])) cph_p($debata["komentar"]);

$info = $lang["competition"] . ": " . cphs_link("./?page=soutez&soutez_id=" . $debata["soutez_ID"], $debata["soutez"]);
if (isset($debata["turnaj_ID"])) $info .= cphs_br() . $lang["tournament"] . ": " . cphs_link("./?page=turnaj&turnaj_id=" . $debata["turnaj_ID"], $debata["turnaj"]);
cph_p($info);

$info = $lang["date"] . ": " . format_date($debata["datum"]);
if (isset($debata["misto"])) $info .= cphs_br() . $lang["place"] . ": " . $debata["misto"];
$info .= cphs_br() . $lang["resolution"] . ": " . cphs_link("./?page=teze&teze_id=" . $debata["teze_ID"], $debata["teze"]);
cph_p($info);

cpdb_fetch_all("
	select
		clovek.clovek_ID,
		clovek.jmeno,
		clovek.nick,
		clovek.prijmeni,
		clovek_debata.role,
		clovek_debata.kidy,
		clovek_debata_ibody.ibody
	from
		clovek_debata
		left join clovek on clovek_debata.clovek_ID = clovek.clovek_ID
		left join clovek_debata_ibody on clovek_debata.clovek_ID = clovek_debata_ibody.clovek_ID
			and clovek_debata.debata_ID = clovek_debata_ibody.debata_ID
	where
		clovek_debata.debata_ID = :debata_id
		and find_in_set(clovek_debata.role,'a1,a2,a3,n1,n2,n3')
	", array(":debata_id"=>$debata_id), $recnik, "role");

$pers = $debata["presvedcive"] ? $lang["3:0"] : $lang["2:1"];
if ($debata["vitez"] == 1) {
	$a_sieg = array("class"=>"sieg");
	$n_sieg = array();
	$a_cell = $lang["won"] . " " . $pers;
	$n_cell = "";
} else {
	$a_sieg = array();
	$n_sieg = array("class"=>"sieg");
	$a_cell = "";
	$n_cell = $lang["won"] . " " . $pers;
}
$a_sieg2 = array_merge($a_sieg, array("colspan"=>"2"));
$n_sieg2 = array_merge($n_sieg, array("colspan"=>"2"));

cph_tag_start("table",array("class"=>"visible"));
cph_tag("tr",array(),
	cphs_tag("th", array(), "")
	. cphs_tag("th", $a_sieg2, $lang["affirmative"])
	. cphs_tag("th", $n_sieg2, $lang["negative"])
);
cph_tag("tr",array(),
	cphs_tag("th", array(), $lang["team"])
	. cphs_tag("td", $a_sieg2, cphs_link("./?page=tym&tym_id=" . $debata["aff_ID"], $debata["aff_tym"]))
	. cphs_tag("td", $n_sieg2, cphs_link("./?page=tym&tym_id=" . $debata["neg_ID"], $debata["neg_tym"]))
);
cph_tag("tr",array(),
	cphs_tag("th", array(), $lang["result"] . "/" . $lang["pts"])
	. cphs_tag("td", $a_sieg, $a_cell)
	. cphs_tag("td", $a_sieg, $debata["aff_body"])
	. cphs_tag("td", $n_sieg, $n_cell)
	. cphs_tag("td", $n_sieg, $debata["neg_body"])
);
cph_tag("tr",array("style"=>"padding-top: 5px"),
	cphs_tag("th", array(), $lang["1st speaker"] . "/" . $lang["kidy"])
	. cphs_tag("td", $a_sieg, cphs_link("./?page=clovek&clovek_id=" . $recnik["a1"]["clovek_ID"], join_name($recnik["a1"]["jmeno"],$recnik["a1"]["nick"],$recnik["a1"]["prijmeni"])))
	. cphs_tag("td", $a_sieg, $recnik["a1"]["kidy"])
	. cphs_tag("td", $n_sieg, cphs_link("./?page=clovek&clovek_id=" . $recnik["n1"]["clovek_ID"], join_name($recnik["n1"]["jmeno"],$recnik["n1"]["nick"],$recnik["n1"]["prijmeni"])))
	. cphs_tag("td", $n_sieg, $recnik["n1"]["kidy"])
);
cph_tag("tr",array(),
	cphs_tag("th", array(), $lang["2nd speaker"] . "/" . $lang["kidy"])
	. cphs_tag("td", $a_sieg, cphs_link("./?page=clovek&clovek_id=" . $recnik["a2"]["clovek_ID"], join_name($recnik["a2"]["jmeno"],$recnik["a2"]["nick"],$recnik["a2"]["prijmeni"])))
	. cphs_tag("td", $a_sieg, $recnik["a2"]["kidy"])
	. cphs_tag("td", $n_sieg, cphs_link("./?page=clovek&clovek_id=" . $recnik["n2"]["clovek_ID"], join_name($recnik["n2"]["jmeno"],$recnik["n2"]["nick"],$recnik["n2"]["prijmeni"])))
	. cphs_tag("td", $n_sieg, $recnik["n2"]["kidy"])
); cph_nl();
cph_tag("tr",array(),
	cphs_tag("th", array(), $lang["3rd speaker"] . "/" . $lang["kidy"])
	. cphs_tag("td", $a_sieg, cphs_link("./?page=clovek&clovek_id=" . $recnik["a3"]["clovek_ID"], join_name($recnik["a3"]["jmeno"],$recnik["a3"]["nick"],$recnik["a3"]["prijmeni"])))
	. cphs_tag("td", $a_sieg, $recnik["a3"]["kidy"])
	. cphs_tag("td", $n_sieg, cphs_link("./?page=clovek&clovek_id=" . $recnik["n3"]["clovek_ID"], join_name($recnik["n3"]["jmeno"],$recnik["n3"]["nick"],$recnik["n3"]["prijmeni"])))
	. cphs_tag("td", $n_sieg, $recnik["n2"]["kidy"])
); cph_nl();
cph_tag_end("table");

?>