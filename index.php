<?php

include_once("lib/de_magic_quotes.inc");
include_once("lib/config.inc");
include_once("lib/cpykhen.inc");
include_once("lib/dblink.inc");
include_once("lib/page.inc");

$_GLOBALS["page"] = new pg_page();
include_once("lib/session.inc");


include_once('inc/functions.php');

/*
<p class="link"><a href="./">{link_homepage}</a></p>
<p class="link"><a href="clovek.php?akce=vsichni">{link_everyone}</a></p>
<p class="link"><a href="klub.php">{link_clubs}</a></p>
*/


$fw_stranky = array(
	//array(modul, titulek)
);



?>