<?php
/*
 * greybox
 * $Id: season.inc,v 1.3 2005/01/09 00:47:32 che0 Exp $
 */

if (isset($_REQUEST["set_season"])) {
	$_SESSION["current_season"] = $_REQUEST["set_season"];
	$GLOBALS["page"]->headers[] = '<meta http-equiv="refresh" content="1;url=./?page=souteze" />';

	pg_achtung($lang["season changed"]);
} else {
	pg_head();
	cph_tag_start("p");
	foreach ($GLOBALS["season_domain"] as $key => $value) {
		cph_link("./?page=season&set_season=" . $key, $value);
		cph_br();
	}
	cph_tag_end("p");
}
?>
