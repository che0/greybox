<?php

include_once("lib/de_magic_quotes.inc");
include_once("lib/config.inc");
include_once("lib/cpykhen.inc");
include_once("lib/dblink.inc");
include_once("lib/page.inc");
include_once("lib/session.inc");

include_once('inc/functions.php');

$GLOBALS["page"]->links[] = array("./",$lang["link_homepage"]);
$GLOBALS["page"]->links[] = array("./?page=clovek&akce=vsichni",$lang["link_everyone"]);
$GLOBALS["page"]->links[] = array("./?page=klub",$lang["clubs"]);
$GLOBALS["page"]->links[] = array("./?page=teze",$lang["resolutions"]);

$GLOBALS["page"]->default_page = "default";

$GLOBALS["page"]->pages = array(
	array("login", $lang["login"]),
	array("logout", $lang["login"]),
	array("clovek", $lang["people"]),
	array("klub", $lang["clubs"]),
	array("teze", $lang["resolutions"]),
	array("teze.pridat", $lang["add resolution"]),
	array("teze.detaily", $lang["resolution details"])
);

$GLOBALS["page"]->render();

?>