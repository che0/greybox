<?php
/*
 * greybox
 * $Id: clovek.accr.edit.exec.inc,v 1.5 2005/01/31 16:23:07 che0 Exp $
 */

$clovek_id = $_REQUEST["clovek_id"];
$rocnik_old = $_REQUEST["rocnik_old"];
$jazyk = $_REQUEST["jazyk"];
$misto = $_REQUEST["misto"];
$rocnik = $_REQUEST["rocnik"];

if (! ($GLOBALS["cps_lidi"] >= 2)) {
	pg_achtung($lang["access denied"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into rozhodci (clovek_ID, jazyk, misto, rocnik) values (:clovek_id, :jazyk, :misto, :rocnik)", array(":rocnik"=>$rocnik, ":jazyk"=>$jazyk, ":misto"=>$misto, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["accrediation update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update rozhodci set jazyk = :jazyk, misto = :misto, rocnik = :rocnik where clovek_id = :clovek_id and rocnik = :rocnik_old", array(":clovek_id"=>$clovek_id, ":rocnik_old"=>$rocnik_old, ":rocnik"=>$rocnik, ":jazyk"=>$jazyk, ":misto"=>$misto))) {
		pg_achtung($lang["accrediation update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from rozhodci where clovek_id = :clovek_id and rocnik = :rocnik_old", array(":clovek_id"=>$clovek_id, ":rocnik_old"=>$rocnik_old))) {
		pg_achtung($lang["accrediation update ok"]);
		include("clovek.inc");
	}
}

?>
