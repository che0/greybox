<?php

include_once("lib/de_magic_quotes.inc");
include_once("lib/config.inc");
include_once("lib/cpykhen.inc");
include_once("lib/dblink.inc");
include_once("lib/page.inc");
include_once("lib/session.inc");
include_once('lib/misc.inc');

$GLOBALS["page"]->links[] = array("./",$lang["link_homepage"]);
$GLOBALS["page"]->links[] = array("./?page=clovek&akce=vsichni",$lang["link_everyone"]);
$GLOBALS["page"]->links[] = array("./?page=klub",$lang["clubs"]);
$GLOBALS["page"]->links[] = array("./?page=teze",$lang["resolutions"]);
$GLOBALS["page"]->links[] = array("./?page=souteze",$lang["competitions"]);
$GLOBALS["page"]->links[] = array("./?page=turnaje",$lang["tournaments"]);

$GLOBALS["page"]->default_page = "default";

$GLOBALS["page"]->pages = array(
	array("login", $lang["login"]),
	array("logout", $lang["login"]),
	array("clovek", $lang["people"]),
	array("klub", $lang["clubs"]),
	array("teze", $lang["resolutions"]),
	array("teze.add", $lang["add resolution"]),
	array("teze.detaily", $lang["resolution details"]),
	array("teze.edit.exec", $lang["edit resolution"]),
	array("teze.add.exec", $lang["add resolution"]),
	array("souteze", $lang["competitions"]),
	array("soutez.detaily", $lang["competition"]),
	array("soutez.edit.exec", $lang["edit competition"]),
	array("soutez.add", $lang["add competition"]),
	array("soutez.teze.edit.exec", $lang["edit official resolution"]),
	array("tym.soutez", $lang["team in the competition"]),
	array("turnaj", $lang["tournament"]),
	array("turnaj.add", $lang["add tournament"]),
	array("turnaj.edit.exec", $lang["edit tournament"]),
	array("turnaje", $lang["tournaments"]),
	array("turnaj.clovek.edit.exec", $lang["edit tournament"]),
	array("tym", $lang["team"]),
	array("tym.add", $lang["add team"]),
	array("tym.edit.exec", $lang["edit team"]),
	array("tym.clovek.edit.exec", $lang["edit team"]),
);

$GLOBALS["page"]->render();

?>