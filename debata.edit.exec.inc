<?php
include("lib/points.inc");

if ($GLOBALS["cps_debaty"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}


// FIXME do the edit, then recount points

?>