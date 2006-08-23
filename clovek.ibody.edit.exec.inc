<?php
/*
 * greybox
 * $Id: clovek.ibody.edit.exec.inc,v 1.1 2006/08/23 17:07:33 che0 Exp $
 */

$clovek_ibody_id = $_REQUEST["clovek_ibody_id"];
$clovek_id = $_REQUEST["clovek_id"];
$rocnik = $_REQUEST["rocnik"];
$ibody = $_REQUEST["ibody"];
$tx = $_REQUEST["tx"];


if (! ($GLOBALS["cps_lidi"] >= 2)) {
	pg_achtung($lang["access denied"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into clovek_ibody (clovek_ID, rocnik, ibody, tx) values (:clovek_id, :rocnik, :ibody, :tx)", array(":clovek_id"=>$clovek_id, ":rocnik"=>$rocnik, ":ibody"=>$ibody, ":tx"=>$tx))) {
		pg_achtung($lang["ipoints update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update clovek_ibody set rocnik = :rocnik, ibody = :ibody, tx = :tx where clovek_ibody_ID = :clovek_ibody_id and clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id, ":clovek_ibody_id"=>$clovek_ibody_id, ":rocnik"=>$rocnik, ":ibody"=>$ibody, ":tx"=>$tx))) {
		pg_achtung($lang["ipoints update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from clovek_ibody where clovek_ibody_ID = :clovek_ibody_id and clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id, ":clovek_ibody_id"=>$clovek_ibody_id))) {
		pg_achtung($lang["ipoints update ok"]);
		include("clovek.inc");
	}
}

?>
