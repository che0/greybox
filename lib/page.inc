<?php

class pg_page {
	var $headers;
	var $title;
	var $language;
	var $links;
	var $lang_block;
	var $login_block;
	var $pages;
	var $default_page;
	
	var $was_head;
	var $achtungs;
	
	function pg_page() {
		$this->headers = array();
		$this->links = array();
		$this->pages = array();
		$this->achtungs = array();
		$this->was_head = false;
	}
	
	function render_head() {
		if ($this->was_head) {
			return false;
		}
		
		$headers = "";
		foreach($this->headers as $header) {
			$headers .= $header . "\n";
		}
		
		$links = "";
		foreach($this->links as $link) {
			$links .= sprintf("<p class=\"link\"><a href=\"%s\">%s</a></p>\n",htmlspecialchars($link[0]), htmlspecialchars($link[1]));
		}
		
		printf(
'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html><head>
<title>%s</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Language" content="%s">
%s<link rel="stylesheet" href="skins/default/global.css" media="screen" type="text/css">
</head>

<body>
<div id="main_title"><a href="./"><img src="skins/default/images/titul.png" width="288" height="44" alt="greybox" style="border: none"></a><span id="main_title_text">%s</span></div>
<div id="leftblock"><p class="language">%s</p>
<p class="login_info">%s</p>
%s</div><div id="mainbody">',
		(isset($this->title))
			? ($GLOBALS["cp_config"]["title"] . " ". $this->title)
			: ($GLOBALS["cp_config"]["title"]),
		$this->language,
		$headers,
		$this->title,
		$this->lang_block,
		$this->login_block,
		$links);
		
		$this->was_head = true;
		return true;

	}
	
	function render_tail() {
		print(
'</div>

</body>
</html>');
	}

	function render() {
		$page[0] = $this->default_page;
		foreach ($this->pages as $candidate) {
			if ($candidate[0] == $_REQUEST["page"]) {
				$page = $candidate;
			}
		}
		
		$this->title = $page[1];
		
		$this->render_this($page[0]);
	}
	
	function render_this($module) {
		
		// included globals
		global $lang;
		
		include($module . ".inc");
		
		if (! $this->was_head) {
			$this->render_head();
		}
		
		$this->render_tail();
	}
	
	function add_achtung($achtung) {
		$this->achtungs[] = $achtung;
	}
	
	function print_achtung($achtung) {
		cph_tag_start("div",array("class"=>"achtung"));
		echo $achtung;
		cph_tag_end("div");
		cph_nl();
	}
	
	function flush_achtungs() {
		foreach ($this->achtungs as $achtung) {
			$this->print_achtung($achtung);
		}
		$this->achtungs = array();
	}
}

function pg_achtung($achtung) {
	$GLOBALS["page"]->add_achtung($achtung);
}

function pg_achtung_now($achtung) {
	$GLOBALS["page"]->print_achtung($achtung);
}

function pg_flush_achtungs() {
	$GLOBALS["page"]->flush_achtungs();
}

function pg_head() {
	$GLOBALS["page"]->render_head();
}


// create the page global
$GLOBALS["page"] = new pg_page;

?>
