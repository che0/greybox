<?php

function cphs_tag_start($name, $attr = array()) {
	$t = "<" . $name;
	foreach ($attr as $key => $value) {
		$t .= " " . $key . "=\"" . htmlspecialchars($value) . "\"";
	}
	$t .= ">";
	return $t;
}

function cph_tag_start($name, $attr = array()) {
	echo cphs_tag_start($name, $attr);
}

function cphs_tag_end($name) {
	return "</" . $name . ">";
}

function cph_tag_end($name) {
	echo cphs_tag_end($name);
}


function cphs_tag_single($name, $attr = array()) {
	$t =  "<" . $name;
	foreach ($attr as $key => $value) {
		$t .= " " . $key . "=\"" . htmlspecialchars($value);
	}
	$t .= " />";
	return $t;
}

function cph_tag_single($name, $attr = array()) {
	echo cphs_tag_single($name, $attr);
}

function cphs_tag($name, $attr, $content) {
	$t = cphs_tag_start($name, $attr);
	$t .= $content;
	$t.= cphs_tag_end($name);
	return $t;
}

function cph_tag($name, $attr, $content) {
	echo cphs_tag($name, $attr, $content);
}

function cph_nl() {
	echo "\n";
}

function cphs_br() {
	return cphs_tag_single("br") . "\"";
}

function cph_br() {
	echo "<br />\n";
}


function cph_table_head($cols, $attr = array()) {
	cph_tag_start("table", $attr);
	cph_tag_start("tr");
	foreach ($cols as $col) {
		cph_tag("th", array(), $col);
	}
	cph_tah_end("tr");
}

function cph_table_row($fields) {
	cph_tag_start("tr");
	foreach ($fields as $field) {
		cph_tag("td", array(), $field);
	}
	cph_tah_end("tr");
}

function cph_table_end() {
	cph_tag_end("table");
}

function cphs_link($href, $text,$attr = array()) {
	return cphs_tag("a",array_merge(array("href"=>$href), $attr), $text);
}

function cph_link($href, $text,$attr = array()) {
	echo cphs_link($href, $text,$attr = array());
}

function cphs_p($text, $attr = array()) {
	return cphs_tag("p", $attr, $text);
}

function cph_p($text, $attr = array()) {
	echo cphs_p($text, $attr = array());
}
?>
