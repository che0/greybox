<?php
/*
 * greybox
 * $Id: clovek.accr.edit.exec.inc,v 1.3 2005/01/09 00:47:31 che0 Exp $
 */

$rozhodci_id = $_REQUEST["rozhodci_id"];
$clovek_id = $_REQUEST["clovek_id"];
$jazyk = $_REQUEST["jazyk"];
$misto = $_REQUEST["misto"];
$rocnik = $_REQUEST["rocnik"];

if (!(($GLOBALS["cps_lidi"] >= 2) || ($GLOBALS["cps_lidi"] == 1 && cpdb_fetch_one_value("select klub_ID from clovek where clovek_ID = :clovek_id",array(":clovek_id"=>$clovek_id)) == $_SESSION["user_klub_ID"]))) {
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
	
	if (cpdb_exec("update rozhodci set jazyk = :jazyk, misto = :misto, rocnik = :rocnik where rozhodci_id = :rozhodci_id and clovek_id = :clovek_id", array(":rocnik"=>$rocnik, ":jazyk"=>$jazyk, ":misto"=>$misto, ":rozhodci_id"=>$rozhodci_id, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["accrediation update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["delete"])) {
	// delete
	
	if (cpdb_exec("delete from rozhodci where rozhodci_ID = :rozhodci_id and clovek_id = :clovek_id", array(":rozhodci_id"=>$rozhodci_id, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["accrediation update ok"]);
		include("clovek.inc");
	}
}

?>
