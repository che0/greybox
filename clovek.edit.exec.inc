<?php
/*
 * greybox
 * $Id: clovek.edit.exec.inc,v 1.12 2005/03/13 17:18:10 che0 Exp $
 */

include_once("lib/points.inc");

$clovek_id = $_REQUEST["clovek_id"];
$klub_id = $_REQUEST["klub_id"];
$jmeno = $_REQUEST["jmeno"];
$nick = $_REQUEST["nick"];
$prijmeni = $_REQUEST["prijmeni"];
$komentar = $_REQUEST["komentar"];
$narozen = $_REQUEST["narozen"];
$debater = $_REQUEST["debater"];
$clen = $_REQUEST["clen"];

cpdb_fetch_one_row("select klub_ID, clen from clovek where clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id), $cur);


if (!($GLOBALS["cps_lidi"] >= 2
	|| ($GLOBALS["cps_lidi"] == 1 && (
		($klub_id == $_SESSION["user_klub_ID"])
		|| ($cur["klub_ID"] == $_SESSION["user_klub_ID"])
	)))) {
		pg_achtung($lang["access denied"]);
		return;
}

if ($jmeno . $prijmeni == "" && !isset($_REQUEST["delete"]) && !isset($_REQUEST["access"])) {
	pg_achtung($lang["a person has to have at least a name"]);
	return;
}

if ($klub_id == 0) $klub_id = NULL;

if (isset($_REQUEST["add"])) {
	// add new
	
	if (cpdb_exec(
		"insert into
			clovek (klub_ID, jmeno, prijmeni, nick, narozen, debater, komentar, clen)
		values
			(:klub_id, :jmeno, :prijmeni, :nick, :narozen, :debater, :komentar, :clen)
		",
		array(
			":klub_id"=>$GLOBALS["cps_lidi"] >= 2 ? $klub_id : $_SESSION["user_klub_ID"],
			":jmeno"=>$jmeno,
			":prijmeni"=>$prijmeni,
			":nick"=>$nick,
			":narozen"=>deformat_date($narozen),
			":debater"=>$debater,
			":clen"=> $GLOBALS["cps_lidi"] >= 3 ? $clen : 0,
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
			clen = :clen,
			komentar = :komentar
		where
			clovek_ID = :clovek_id
		" . ($GLOBALS["cps_lidi"] < 2 ? "and klub_ID = :user_klub_id" : ""),
		array(
			":clovek_id"=>$clovek_id,
			":klub_id"=>$GLOBALS["cps_lidi"] >= 2 ? $klub_id  : $cur["klub_ID"],
			":jmeno"=>$jmeno,
			":prijmeni"=>$prijmeni,
			":nick"=>$nick,
			":narozen"=>deformat_date($narozen),
			":debater"=>$debater,
			":clen"=>$GLOBALS["cps_lidi"] >= 3 ? $clen : $cur["clen"],
			":komentar"=>$komentar,
			":user_klub_id"=>$_SESSION["user_klub_ID"]
		)
	)) {
		pg_achtung($lang["person update ok"]);
		
		if ($klub_id != $cur["klub_ID"]) {
			// recount ipoints if club changed
			ipts_recount_clovek($clovek_id);
		}
		
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("
		delete from
			clovek
		where
			clovek_ID = :clovek_id
		" . ($GLOBALS["cps_lidi"] < 2 ? "and klub_ID = :user_klub_id" : ""),
		array(
			":clovek_id"=>$clovek_id,
			":user_klub_id"=>$_SESSION["user_klub_ID"]
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
