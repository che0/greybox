<?php

if ($GLOBALS["cps_debaty"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

include_once("i_domeny.inc");

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
		soutez.jazyk as jazyk,
		turnaj.turnaj_ID,
		turnaj.nazev as turnaj
	from
		debata
		left join debata_tym dt_aff on dt_aff.debata_ID = debata.debata_ID and dt_aff.pozice = 1
		left join debata_tym dt_neg on dt_neg.debata_ID = debata.debata_ID and dt_neg.pozice = 0
		left join tym tym_aff on dt_aff.tym_ID = tym_aff.tym_ID
		left join tym tym_neg on dt_neg.tym_ID = tym_neg.tym_ID
		left join teze on teze.teze_ID = debata.teze_ID
		left join soutez on soutez.soutez_ID = debata.soutez_ID
		left join turnaj on turnaj.turnaj_ID = debata.turnaj_ID
	where
		debata.debata_ID = :debata_id
	", array(":debata_id"=>$debata_id), $debata)) {
	
	pg_achtung($lang["no record"]);
	return;
}

$GLOBALS["page"]->title = format_date($debata["datum"]) . " " . $debata["aff_tym"] . "/" . $debata["neg_tym"];
pg_head();

// build teams domain
$sdomain_tym = array(
	"select
		tym.tym_ID,
		concat(tym.nazev, ' (', klub.kratky_nazev, ')') as tym
	from
		tym
		left join klub using (klub_ID)
	order by
		tym.nazev
	", array()
);

// selects for debater domains
$select_debaters_team = "
		(
		select
			clovek.clovek_ID,
			concat_ws(' ', clovek.jmeno, clovek.nick, clovek.prijmeni) as clovek,
			concat(clovek.prijmeni,clovek.jmeno,clovek.nick) as s_clovek
		from
			clovek_tym
			left join clovek using (clovek_ID)
		where
			clovek_tym.tym_ID = :tym_id
			and clovek_tym.aktivni = 1
			and clovek.debater = 1
		)
		union
		(
		select
			0 as clovek_ID,
			:null as clovek,
			'' as s_clovek
		)
		order by
			s_clovek
	";
	
$select_debaters = "
		(
		select
			clovek.clovek_ID,
			concat_ws(' ', clovek.jmeno, clovek.nick, clovek.prijmeni) as clovek,
			concat(clovek.prijmeni,clovek.jmeno,clovek.nick) as s_clovek
		from
			clovek_tym
			left join clovek using (clovek_ID)
		where
			clovek_tym.aktivni = 1
			and clovek.debater = 1
		)
		union
		(
		select
			0 as clovek_ID,
			:null as clovek,
			'' as s_clovek
		)
		order by
			s_clovek
	";

// build affirmative debaters domain
if (isset($debata["aff_ID"])) {
	// choose affirmatives from the team
	$sdomain_aff = array($select_debaters_team, array(":tym_id"=>$debata["aff_ID"], ":null"=>$lang["N/A"]));
} else {
	// choose from all debaters
	$sdomain_aff = array($select_debaters, array(":null"=>$lang["N/A"]));
}

// build negative debaters domain
if (isset($debata["neg_ID"])) {
	// choose negatives from the team
	$sdomain_neg = array($select_debaters_team, array(":tym_id"=>$debata["neg_ID"], ":null"=>$lang["N/A"]));
} else {
	// choose from all debaters
	$sdomain_neg = array($select_debaters, array(":null"=>$lang["N/A"]));
}

// build resolutions domain
$sdomain_teze = array(
	sprintf("select teze_ID, tx as tx from teze %s order by tx", isset($debata["jazyk"]) ? "where jazyk = :jazyk" : ""),
	array(":jazyk"=>$debata["jazyk"])
);

// build competitions domain
$sdomain_souteze = array(
	"select soutez_ID, nazev from souteze where rocnik = :rocnik",
	array(":rocnik"=>$GLOBALS["current_season"])
);

// build tournaments domain
$sdomain_turnaje = array(
	sprintf("select turnaj_ID, nazev from turnaj %s order by nazev", isset($debata["soutez_ID"]) ? "where soutez_ID = :soutez_id" : ""),
	array(":soutez_id"=>$debata["soutez_ID"])
);

// FIXME pykhen na zakladni vlastnosti

// FIXME pykhen na rozhodci

// FIXME pykhen na organizatory

?>