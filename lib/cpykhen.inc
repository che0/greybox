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
		$this->form_attr = array();
		$this->init_done = false;
	}
	
	function load_this($values) {
		foreach ($this->form as $element) {
			$element->load_this($values);
		}
	}
	
	function load_new() {
		foreach ($this->form as $element) {
			$element->load_new($values);
		}
	}
	
	function init() {
		if (! $this->init_done) {
			foreach ($this->form  as $element) {
				$element->init();
			}
			$this->init_done = true;
		}
	}
	
	function render() {
		cph_tag_start("form",$this->form_attr);
		foreach ($this->form as $element) {
			$element->render();
		}
		cph_tag_end("form");
		cph_nl();
	}
	
	function render_select() {
		$this->init();
		cpdb_fetch_all($this->select, $this->select_args, $result);
		foreach ($result as $row) {
			$this->load_this($row);
			$this->render();
		}
	}
	
	function render_new() {
		$this->init();
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
		if ($attr != array()) {
			cph_tag("span", $attr, $tx);
		} else {
			echo $tx;
		}
	}
}

class cpf_db_text extends cpf_text {
	
	function cpf_db_text($field, $attr = array()) {
		$this->field = $field;
		$this->attr = $attr;
	}
	
	function render() {
		if ($attr != array()) {
			cph_tag("span", $attr, $value);
		} else {
			echo $value;
		}
	}
}

class cpf_db_listbox_array extends cfutral {
	var $domain;
	
	function cpf_db_listbox_array($name, $domain, $field, $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		$this->domain = $domain;
		$this->field = $field;
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
	var $select_attr;
	
	function cpf_db_listbox($name, $domain, $field, $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		$this->select = $domain[0];
		$this->select_attr = $domain[1];
		$this->field = $field;
	}
	
	function init() {
		$this->domain = array();
		
		cpdb_fetch_all($this->select, $this->select_attr, $my_res);
		foreach ($my_res as $option) {
			$this->domain[$option[0]] = $option[1];
		}
	}
}

class cpf_db_textbox extends cfutral {

	function cpf_db_textbox($name, $field, $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		$this->field = $field;
	}
	
	function render() {
		cph_tag_single("input", array_merge(array("type"=>"text", "value"=>$this->value), $this->attr));
	}
}

class cpf_db_textarea extends cfutral {

	function cpf_db_textbox($name, $field, $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		$this->field = $field;
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
		cph_tag_single("input", array_merge(array("type"=>"hidden", "value"=>$this->tx), $this->attr));
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
