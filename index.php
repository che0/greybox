<?php
/*
 * greybox
 * $Id: index.php,v 1.32 2005/01/09 00:47:32 che0 Exp $
 */

include_once("lib/de_magic_quotes.inc");
include_once("lib/config.inc");
include_once("lib/cpykhen.inc");
include_once("lib/dblink.inc");
include_once("lib/page.inc");
include_once("lib/session.inc");
include_once("lib/misc.inc");

// link := array(link, title, section)
$GLOBALS["page"]->links[] = array("./",$lang["homepage"],"home");
$GLOBALS["page"]->links[] = array("./?page=lide",$lang["people"],"clovek");
$GLOBALS["page"]->links[] = array("./?page=kluby",$lang["clubs"],"klub");
$GLOBALS["page"]->links[] = array("./?page=tymy",$lang["teams"],"tym");
$GLOBALS["page"]->links[] = array("./?page=souteze",$lang["competitions"],"soutez");
$GLOBALS["page"]->links[] = array("./?page=turnaje",$lang["tournaments"],"turnaj");
$GLOBALS["page"]->links[] = array("./?page=debaty",$lang["debates"],"debata");
$GLOBALS["page"]->links[] = array("./?page=teze",$lang["resolutions"],"teze");

// page := array(keyword, title, section)
$GLOBALS["page"]->default_page = array("default",null, "home");

$GLOBALS["page"]->pages = array(
	array("language", $lang["language"],"home"),
	array("season", $lang["season"],"home"),
	array("login", $lang["login"],"home"),
	array("logout", $lang["login"],"home"),
	array("lide", $lang["people"],"clovek"),
	array("clovek", $lang["people"],"clovek"),
	array("clovek.add", $lang["add person"],"clovek"),
	array("clovek.edit.exec", $lang["edit person"],"clovek"),
	array("clovek.pw.edit.exec", $lang["edit person"],"clovek"),
	array("clovek.accr.edit.exec", $lang["edit person"],"clovek"),
	array("clovek.cont.edit.exec", $lang["edit person"],"clovek"),
	array("clovek.self.exec", $lang["edit person"],"clovek"),
	array("klub", $lang["clubs"],"klub"),
	array("klub.add", $lang["add club"], "klub"),
	array("klub.clovek.edit.exec", $lang["edit club"], "klub"),
	array("klub.edit.exec", $lang["edit club"], "klub"),
	array("kluby", $lang["clubs"], "klub"),
	array("teze", $lang["resolutions"], "teze"),
	array("teze.add", $lang["add resolution"], "teze"),
	array("teze.detaily", $lang["resolution details"], "teze"),
	array("teze.edit.exec", $lang["edit resolution"], "teze"),
	array("teze.add.exec", $lang["add resolution"], "teze"),
	array("souteze", $lang["competitions"], "soutez"),
	array("soutez", $lang["competition"], "soutez"),
	array("soutez.kidy", $lang["competition"]."/".$lang["kidy"], "soutez"),
	array("soutez.edit.exec", $lang["edit competition"], "soutez"),
	array("soutez.add", $lang["add competition"], "soutez"),
	array("soutez.teze.edit.exec", $lang["edit official resolution"], "soutez"),
	array("tym.soutez", $lang["team in the competition"], "soutez"),
	array("turnaj", $lang["tournament"], "turnaj"),
	array("turnaj.kidy", $lang["tournament"]."/".$lang["kidy"], "turnaj"),
	array("turnaj.add", $lang["add tournament"], "turnaj"),
	array("turnaj.edit.exec", $lang["edit tournament"], "turnaj"),
	array("turnaje", $lang["tournaments"], "turnaj"),
	array("turnaj.clovek.edit.exec", $lang["edit tournament"], "turnaj"),
	array("tymy", $lang["teams"], "tym"),
	array("tym", $lang["team"], "tym"),
	array("tym.add", $lang["add team"], "tym"),
	array("tym.edit.exec", $lang["edit team"], "tym"),
	array("tym.clovek.edit.exec", $lang["edit team"], "tym"),
	array("debata", $lang["debate"], "debata"),
	array("debata.add", $lang["add debate"], "debata"),
	array("debata.edit", $lang["edit debate"], "debata"),
	array("debata.clovek.edit.exec", $lang["edit debate"], "debata"),
	array("debata.edit.exec", $lang["edit debate"], "debata"),
	array("debaty", $lang["debates"], "debata"),
);

$GLOBALS["page"]->render();

?>
