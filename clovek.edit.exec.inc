<?php

$clovek_id = $_REQUEST["clovek_id"];
$klub_id = $_REQUEST["klub_id"];
$cur_klub_id = $_REQUEST["cur_klub_id"];
$jmeno = $_REQUEST["jmeno"];
$nick = $_REQUEST["nick"];
$prijmeni = $_REQUEST["prijmeni"];
$komentar = $_REQUEST["komentar"];
$narozen = $_REQUEST["narozen"];
$debater = $_REQUEST["debater"];

if (!($GLOBALS["cps_lidi"] >= 2
	|| ($GLOBALS["cps_lidi"] == 1 && (
		($klub_id == $_SESSION["user_klub_ID"])
		|| ($cur_klub_id == $_SESSION["user_klub_ID"])
	)))) {
		pg_achtung($lang["access denied"]);
		return;
}

if ($klub_id == 0) $klub_id = ""; // "" gets converted to NULL by cpdb_exec

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec(
		"insert into
			clovek (klub_ID, jmeno, prijmeni, nick, narozen, debater, komentar)
		values
			(:klub_id, :jmeno, :prijmeni, :nick, :narozen, :debater, :komentar)
		",
		array(
			":klub_id"=>$klub_id,
			":jmeno"=>$jmeno,
			":prijmeni"=>$prijmeni,
			":nick"=>$nick,
			":narozen"=>deformat_date($narozen),
			":debater"=>$debater,
			":komentar"=>$komentar
		)
	)) {
		pg_achtung($lang["person insert ok"]);
		if ($klub_id) {
			include("klub.inc");
		} else {
			$_REQUEST["clovek_id"] = cpdb_insert_id();
			include("clovek.inc");
		}
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec(
		"update clovek set
			klub_ID = :klub_id,
			jmeno = :jmeno,
			prijmeni = :prijmeni,
			nick = :nick,
			narozen = :narozen,
			debater = :debater,
			komentar = :komentar
		where
			clovek_ID = :clovek_id
		" . ($GLOBALS["cps_kluby"] < 2 ? "and klub_ID = :cur_klub_id" : ""),
		array(
			":clovek_id"=>$clovek_id,
			":klub_id"=>$klub_id,
			":cur_klub_id"=>$cur_klub_id,
			":jmeno"=>$jmeno,
			":prijmeni"=>$prijmeni,
			":nick"=>$nick,
			":narozen"=>deformat_date($narozen),
			":debater"=>$debater,
			":komentar"=>$komentar
		)
	)) {
		pg_achtung($lang["person update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("
		delete from
			clovek
		where
			clovek_ID = :clovek_id
		" . ($GLOBALS["cps_kluby"] < 2 ? "and klub_ID = :cur_klub_id" : ""),
		array(
			":tym_id"=>$tym_id,
			":cur_klub_id"=>$cur_klub_id
		)
	)) {
		pg_achtung($lang["person delete ok"]);
		include("klub.inc");
	}
} elseif (isset($_REQUEST["access"])) {
	// update permissions
	
	if ($GLOBALS["cps_lidi"] < 3) {
		pg_achtung($lang["access denied"]);
		return;	
	}
	
	if (cpdb_exec(
		"update clovek set
			prava_lidi = :prava_lidi,
			prava_kluby = :prava_kluby,
			prava_souteze = :prava_souteze,
			prava_debaty = :prava_debaty
		where
			clovek_ID = :clovek_id
		",
		array(
			":clovek_id"=>$clovek_id,
			":prava_lidi"=>$_REQUEST["prava_lidi"],
			":prava_kluby"=>$_REQUEST["prava_kluby"],
			":prava_souteze"=>$_REQUEST["prava_souteze"],
			":prava_debaty"=>$_REQUEST["prava_debaty"]
		)
	)) {
		pg_achtung($lang["permissions update ok"]);
		include("clovek.inc");
	}
}

?> 
