<?php

function cph_tag_start($name, $attr = array()) {
	echo "<" . $name;
	foreach ($attr as $key => $value) {
		echo " " . $key . "=\"" . htmlspecialchars($value);
	}
	echo ">";
}

function cph_tag_end($name) {
	echo "</" . $name . ">";
}

function cph_tag_single($name, $attr = array()) {
	echo "<" . $name;
	foreach ($attr as $key => $value) {
		echo " " . $key . "=\"" . htmlspecialchars($value);
	}
	echo " />";
}

function cph_tag($name, $attr, $content) {
	cph_tag_start($name, $attr);
	echo htmlspecialchars($content);
	cph_tag_end($name);
}

function cph_nl() {
	echo "\n";
}

function cph_br() {
	cph_tag_single("br");
	cph_nl();
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

function cph_link($text, $href, $attr = array()) {
	cph_tag("a",array_merge(array("href"=>$href), $attr), $text);
}
?> 
