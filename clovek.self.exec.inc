<?php

$clovek_id = $_REQUEST["clovek_id"];
$nick = $_REQUEST["nick"];
$komentar = $_REQUEST["komentar"];
$narozen = $_REQUEST["narozen"];

if ($_SESSION["user_clovek_ID"] != $clovek_id) {
		pg_achtung($lang["access denied"]);
		return;
}

if (isset($_REQUEST["save"])) {
	// update
	
	if (cpdb_exec("
		update clovek set
			nick = :nick,
			komentar = :komentar
		where
			clovek_ID = :clovek_id
		",
		array(
			":clovek_id"=>$clovek_id,
			":nick"=>$nick,
			":komentar"=>$komentar
		)
	)) {
		pg_achtung($lang["person update ok"]);
		include("clovek.inc");
	}
	
} elseif (isset($_REQUEST["born"])) {
	// delete
	
	if (cpdb_exec("
		update clovek set
			narozen = :narozen
		where
			clovek_ID = :clovek_id
			and isnull(narozen)
		",
		array(
			":clovek_id"=>$clovek_id,
			":narozen"=>deformat_date($narozen),
		)
	)) {
		pg_achtung($lang["person update ok"]);
		include("clovek.inc");
	}
}

?> 
