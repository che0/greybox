<?php

class pg_page {
	var $headers;
	var $title;
	var $language;
	var $links;
	var $lang_block;
	var $login_block;
	
	function pg_page() {
		$this->headers = array();
		$this->links = array();
	}
	
	function print_head() {
		$headers = "";
		foreach($this->headers as $header) {
			$headers .= $header . "\n";
		}
		
		$links = "";
		foreach($this->links as $link) {
			$links .= sprintf("<p class=\"link\"><a href=\"%s\">%s</a></p>\n",htmlspecialchars($link["href"]), htmlspecialchars($link["text"]));
		}
		
		printf(
'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html><head>
<title>%s</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Language" content="%s">
{page_headers}
<link rel="stylesheet" href="skins/default/global.css" media="screen" type="text/css">
</head>

<body>
<div id="main_title"><a href="./"><img src="skins/default/images/titul.png" width="288" height="44" alt="{page_title}" style="border: none"></a><span id="main_title_text">%s</span></div>
<div id="leftblock"><p class="language">%s</p>
<p class="login_info">{login_info}</p>
%s</div><div id="mainbody">',
		$_GLOBALS["config"]["title"] . $this->title,
		$this->language,
		$headers,
		$this->title,
		$this->lang_block,
		$this->login_block,
		$links);

	}
	
	function print_tail() {
		print(
'</div>

</body>
</html>');
	
	}
}

?>