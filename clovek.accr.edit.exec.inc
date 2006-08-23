<?php
/*
 * greybox
 * $Id: clovek.accr.edit.exec.inc,v 1.6 2006/08/23 17:07:33 che0 Exp $
 */

$clovek_id = $_REQUEST["clovek_id"];
$rocnik_old = $_REQUEST["rocnik_old"];
$jazyk = $_REQUEST["jazyk"];
$misto = $_REQUEST["misto"];
$rocnik = $_REQUEST["rocnik"];
$format = $_REQUEST["format"];

if (! ($GLOBALS["cps_lidi"] >= 2)) {
	pg_achtung($lang["access denied"]);
	return;
}

if (isset($_REQUEST["add"])) {
	// add new
	if (cpdb_exec("insert into rozhodci (clovek_ID, jazyk, misto, rocnik, format) values (:clovek_id, :jazyk, :misto, :rocnik, :format)", array(":rocnik"=>$rocnik, ":jazyk"=>$jazyk, ":misto"=>$misto, ":format"=>$format, ":clovek_id"=>$clovek_id))) {
		pg_achtung($lang["accrediation update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("update rozhodci set jazyk = :jazyk, misto = :misto, rocnik = :rocnik, format = :format where clovek_id = :clovek_id and rocnik = :rocnik_old", array(":clovek_id"=>$clovek_id, ":rocnik_old"=>$rocnik_old, ":rocnik"=>$rocnik, ":jazyk"=>$jazyk, ":misto"=>$misto, ":format"=>$format))) {
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
