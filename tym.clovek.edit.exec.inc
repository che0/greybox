<?php
/*
 * greybox
 * $Id: tym.clovek.edit.exec.inc,v 1.4 2005/01/09 00:47:32 che0 Exp $
 */

$ct_id = $_REQUEST["ct_id"];
$tym_id = $_REQUEST["tym_id"];
$clovek_id = $_REQUEST["clovek_id"];
$aktivni = $_REQUEST["aktivni"];

if (!(
	$GLOBALS["cps_kluby"] >= 2
	|| ($GLOBALS["cps_kluby"] == 1 && cpdb_fetch_one_value("select klub_ID from tym where tym_ID = :tym_id", array(":tym_id"=>$tym_id)) == $_SESSION["user_klub_ID"])
	|| (cpdb_fetch_one_value("select aktivni from clovek_tym where clovek_ID = :clovek_id and tym_ID = :tym_id", array(":clovek_id"=>$_SESSION["user_clovek_ID"], ":tym_id"=>$tym_id)) == 1)
	)) {
		pg_achtung($lang["access denied"]);
		return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_tym (clovek_ID, tym_ID, aktivni) values (:clovek_id, :tym_id, :aktivni)", array(":clovek_id"=>$clovek_id, ":tym_id"=>$tym_id, ":aktivni"=>$aktivni))) {
		pg_achtung($lang["team update ok"]);
		include("tym.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_tym set aktivni = :aktivni where clovek_ID = :clovek_id and ct_ID = :ct_id", array(":clovek_id"=>$clovek_id, ":tym_id"=>$tym_id, ":aktivni"=>$aktivni, ":ct_id"=>$ct_id))) {
		pg_achtung($lang["team update ok"]);
		include("tym.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_fetch_one_value(
		"select
			count(debata.debata_ID)
		from
			debata
			left join debata_tym on debata.debata_ID = debata_tym.debata_ID
			left join clovek_debata on debata.debata_ID = clovek_debata.debata_ID
		where
			clovek_debata.clovek_ID = :clovek_id
			and debata_tym.tym_ID = :tym_id
			and (
				(clovek_debata.role in ('a1','a2','a3') and debata_tym.pozice = 1)
				or
				(clovek_debata.role in ('n1','n2','n3') and debata_tym.pozice = 0)
			)
		", array(":clovek_id"=>$clovek_id, ":tym_id"=>$tym_id)) == 0)
	{
		// the person does not have any debates with that team
		if (cpdb_exec("delete from clovek_tym where ct_ID = :ct_id and clovek_ID = :clovek_id and tym_ID = :tym_id", array(":ct_id"=>$ct_id, ":clovek_id"=>$clovek_id, ":tym_id"=>$tym_id))) {
			pg_achtung($lang["team update ok"]);
			include("tym.inc");
		}
	} else {
		// the person did debate with that team
		pg_achtung($lang["refusing to delete member with recorded debates"]);
		include("tym.inc");
	}
}

?> 
