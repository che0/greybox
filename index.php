<?php

include_once("lib/de_magic_quotes.inc");
include_once("lib/config.inc");
include_once("lib/cpykhen.inc");
include_once("lib/dblink.inc");
include_once("lib/page.inc");
$GLOBALS["page"] = new pg_page();
include_once("lib/session.inc");

include_once('inc/functions.php');

$GLOBALS["page"]->links[] = array("./",$lang["link_homepage"]);
$GLOBALS["page"]->links[] = array("./",$lang["link_everyone"]);
$GLOBALS["page"]->links[] = array("./",$lang["clubs"]);

$GLOBALS["page"]->default_page = "default";

$GLOBALS["page"]->pages = array(
	//array(modul, titulek)
);

$GLOBALS["page"]->render();

?>