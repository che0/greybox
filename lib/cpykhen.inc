<?php

include_once("cpdb.inc");
include_once("cphtml.inc");

class cpykhen {
	var $form;
	var $form_attr;
	var $select;
	var $select_args;
	var $init_done;
	
	function cpykhen() {
		$this->form = array();
		$this->form_attr = array("method"=>"post");
		$this->init_done = false;
	}
	
	function load_this($values) {
		$this->init();
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->load_this($values);
		}
	}
	
	function load_new() {
		$this->init();
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->load_new($values);
		}
	}
	
	function init() {
		if (! $this->init_done) {
			$this->init_force();
		}
	}
	
	function init_force() {
		foreach ($this->form  as $element=>$neco) {
			$this->form[$element]->init();
		}
		$this->init_done = true;
	}
	
	function render() {
		cph_tag_start("form",$this->form_attr);
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->render();
		}
		cph_tag_end("form");
		cph_nl();
	}
	
	function render_hidden($button_title = "__") {
	if ($button_title == "__") $button_title = $lang["add new"];
	$GLOBALS["hidden_forms"] = $GLOBALS["hidden_forms"] + 1;
	$form_id = "hidden_form_" . $GLOBALS["hidden_forms"];
	cph_tag_start("div",array("id"=>$form_id));
	cph_tag_start("div",array("id"=>$form_id . "_b","style"=>"visibility: hidden; position: absolute; z-index:1"));
	cph_tag_start("form");
	cph_tag_single("input", array ("type" => "button", "value" => $button_title, "onclick" => "c_hide('" . $form_id . "_b'); c_show('" . $form_id . "')"));
	cph_tag_end("form");
	cph_tag_end("div");
	
	$this->render();
	
	cph_tag_end("div");
	cph_tag("script",array("type"=>"text/javascript"),"c_show('" . $form_id . "_b'); c_hide('" . $form_id . "');");
	echo ;
	}
	
	
	function render_select() {
		cpdb_fetch_all($this->select, $this->select_args, $result);
		foreach ($result as $row) {
			$this->load_this($row);
			$this->render();
		}
	}
	
	function render_new() {
		$this->load_new();
		$this->render();
	}
}


class cfutral {
	var $value;
	var $field;
	var $attr;
	
	function init() {
	}
	
	function load_this($values) {
		$this->value = $values[$this->field];
		// echo "__" . $this->field . "=>" . $values[$this->field];
	}
	
	function load_new() {
		$this->value = NULL;
	}
	
	function render() {
	}
}

class cpf_text extends cfutral {
	var $tx;
	
	function cpf_text($content, $attr = array()) {
		$this->tx = $content;
		$this->attr = $attr;
	}
	
	function render() {
		if ($this->attr != array()) {
			cph_tag("span", $this->attr, $this->tx);
		} else {
			echo $this->tx;
		}
	}
}

class cpf_db_link extends cfutral {
	var $href;
	var $tx;
	
	function cpf_db_link($field, $href, $tx) {
		$this->field = $field;
		$this->href = $href;
		$this->tx = $tx;
	}
	
	function render() {
		cph_link(sprintf($this->href,$this->value),$this->tx);
	}
}

class cpf_nl extends cfutral {
	
	function render() {
		cph_br();
	}
}

class cpf_db_text extends cfutral {
	
	function cpf_db_text($field, $attr = array()) {
		$this->field = $field;
		$this->attr = $attr;
	}
	
	function render() {
		if ($this->attr != array()) {
			cph_tag("span", $this->attr, $this->value);
		} else {
			echo $this->value;
		}
	}
}

class cpf_db_listbox_array extends cfutral {
	var $domain;
	
	function cpf_db_listbox_array($name, $domain, $field = "__", $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		$this->domain = $domain;
		if ($field == "__") {
			$this->field = $name;	
		} else {
			$this->field = $field;
		}
	}
	
	function render() {
		cph_tag_start("select",$this->attr);
		foreach ($this->domain as $key => $caption) {
			if ($key == $this->value) {
				cph_tag("option",array("value" => $key, "selected" => "selected"), $caption);
			} else {
				cph_tag("option",array("value" => $key), $caption);
			}
		}
		cph_tag_end("select");
		cph_nl();
	}
}

class cpf_db_listbox extends cpf_db_listbox_array {
	var $select;
	var $select_args;
	
	function cpf_db_listbox($name, $domain, $field = "__", $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		$this->select = $domain[0];
		$this->select_args = $domain[1];
		if ($field == "__") {
			$this->field = $name;	
		} else {
			$this->field = $field;
		}
	}
	
	function init() {
		$this->domain = array();
		
		cpdb_fetch_all($this->select, $this->select_args, $my_res);
		foreach ($my_res as $option) {
			$this->domain[$option[0]] = $option[1];
		}
	}
}

class cpf_db_textbox extends cfutral {

	function cpf_db_textbox($name, $field = "__", $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		if ($field == "__") {
			$this->field = $name;	
		} else {
			$this->field = $field;
		}
	}
	
	function render() {
		cph_tag_single("input", array_merge(array("type"=>"text", "value"=>$this->value), $this->attr));
	}
}

class cpf_db_textarea extends cfutral {

	function cpf_db_textarea($name, $field = "__", $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		if ($field == "__") {
			$this->field = $name;	
		} else {
			$this->field = $field;
		}
	}
	
	function render() {
		cph_tag("textarea", $this->attr, $this->value);
	}
}

class cpf_hidden extends cfutral {
	var $tx;
	
	function cpf_hidden($name, $value) {
		$this->tx = $value;
		$this->attr = array_merge(array("name"=>$name),$this->attr);
	}
	
	function render() {
		cph_tag_single("input", array_merge(array("type"=>"hidden", "value"=>$this->tx), $this->attr));
	}
}

class cpf_db_hidden extends cfutral {
	
	function cpf_db_hidden($name, $field = NULL) {
		if ($field == NULL) {
			$this->field = $name;
		} else {
			$this->field = $field;
		}
		$this->attr = array_merge(array("name"=>$name),$this->attr);
	}
	
	function render() {
		cph_tag_single("input", array_merge(array("type"=>"hidden", "value"=>$this->value), $this->attr));
	}
}

class cpf_button extends cfutral {
	
	function cpf_button($name,$caption) {
		$this->attr = array("name"=>$name, "value"=>$caption, "type"=>"submit");
	}
	
	function render() {
		cph_tag_single("input", $this->attr);
	}
}

?>
