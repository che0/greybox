<?php

include_once("lib/de_magic_quotes.inc");
include_once("lib/config.inc");
include_once("lib/cpykhen.inc");
include_once("lib/dblink.inc");
include_once("lib/page.inc");
include_once("lib/session.inc");
include_once("lib/misc.inc");

$GLOBALS["page"]->links[] = array("./",$lang["homepage"]);
$GLOBALS["page"]->links[] = array("./?page=lide",$lang["people"]);
$GLOBALS["page"]->links[] = array("./?page=kluby",$lang["clubs"]);
$GLOBALS["page"]->links[] = array("./?page=tymy",$lang["teams"]);
$GLOBALS["page"]->links[] = array("./?page=souteze",$lang["competitions"]);
$GLOBALS["page"]->links[] = array("./?page=turnaje",$lang["tournaments"]);
$GLOBALS["page"]->links[] = array("./?page=debaty",$lang["debates"]);
$GLOBALS["page"]->links[] = array("./?page=teze",$lang["resolutions"]);

$GLOBALS["page"]->default_page = "default";

$GLOBALS["page"]->pages = array(
	array("login", $lang["login"]),
	array("logout", $lang["login"]),
	array("lide", $lang["people"]),
	array("clovek", $lang["people"]),
	array("clovek.add", $lang["add person"]),
	array("clovek.edit.exec", $lang["edit person"]),
	array("clovek.pw.edit.exec", $lang["edit person"]),
	array("clovek.accr.edit.exec", $lang["edit person"]),
	array("clovek.cont.edit.exec", $lang["edit person"]),
	array("clovek.self.exec", $lang["edit person"]),
	array("klub", $lang["clubs"]),
	array("klub.add", $lang["add club"]),
	array("klub.clovek.edit.exec", $lang["edit club"]),
	array("klub.edit.exec", $lang["edit club"]),
	array("kluby", $lang["clubs"]),
	array("teze", $lang["resolutions"]),
	array("teze.add", $lang["add resolution"]),
	array("teze.detaily", $lang["resolution details"]),
	array("teze.edit.exec", $lang["edit resolution"]),
	array("teze.add.exec", $lang["add resolution"]),
	array("souteze", $lang["competitions"]),
	array("soutez", $lang["competition"]),
	array("soutez.kidy", $lang["competition"]."/".$lang["kidy"]),
	array("soutez.edit.exec", $lang["edit competition"]),
	array("soutez.add", $lang["add competition"]),
	array("soutez.teze.edit.exec", $lang["edit official resolution"]),
	array("tym.soutez", $lang["team in the competition"]),
	array("turnaj", $lang["tournament"]),
	array("turnaj.add", $lang["add tournament"]),
	array("turnaj.edit.exec", $lang["edit tournament"]),
	array("turnaje", $lang["tournaments"]),
	array("turnaj.clovek.edit.exec", $lang["edit tournament"]),
	array("tymy", $lang["teams"]),
	array("tym", $lang["team"]),
	array("tym.add", $lang["add team"]),
	array("tym.edit.exec", $lang["edit team"]),
	array("tym.clovek.edit.exec", $lang["edit team"]),
	array("debata", $lang["debate"]),
	array("debata.add", $lang["add debate"]),
	array("debata.edit", $lang["edit debate"]),
	array("debata.clovek.edit.exec", $lang["edit debate"]),
	array("debata.edit.exec", $lang["edit debate"]),
	array("debaty", $lang["debates"]),
);

$GLOBALS["page"]->render();

?>
