<?php

include_once("cpdb.inc");
include_once("cphtml.inc");

// robotized form
class cpykhen {
	var $form; // this contains objects that represent form values
	var $form_attr; // array of attributes of form tag - like array("action"=>"index.php")
	var $select; // SQL select, that can be used to get data in
	var $select_args; // argument array for ^
	
	var $init_done; // internal, sat if the object is initialized
	
	// constructor
	function cpykhen() {
		$this->form = array();
		$this->form_attr = array("method"=>"post");
		$this->init_done = false;
	}
	
	// loads array of values into the object. if something is not in the array,
	// is is also cleared in the form field
	// this method also initalizes the form if necessary
	function load_this($values) {
		$this->init();
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->load_this($values);
		}
	}
	
	// loads array of values into the object. if something is not in the array,
	// the form field is left as it is
	// this method also initalizes the form if necessary
	function append_this($values) {
		$this->init();
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->append_this($values);
		}
	}
	
	// loads null values
	// this method also initalizes the form if necessary
	function load_new() {
		$this->init();
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->load_new();
		}
	}
	
	// this method initalizes the form if necessary
	function init() {
		if (! $this->init_done) {
			$this->init_force();
		}
	}
	
	// this method also always initalizes the form
	function init_force() {
		foreach ($this->form  as $element=>$neco) {
			$this->form[$element]->init();
		}
		$this->init_done = true;
	}
	
	// renders the form
	function render() {
		if (!isset($this->form_attr["action"])) $this->form_attr["action"] = "";
		cph_tag_start("form",$this->form_attr);
		cph_tag_start("div",array("class"=>"in_form"));
		foreach ($this->form as $element=>$neco) {
			$this->form[$element]->render();
		}
		cph_tag_end("div");
		cph_tag_end("form");
		cph_nl();
	}
	
	// renders form, but hides it under a button
	function render_hidden($button_title = "__") {
		if ($button_title == "__")	$button_title = $lang["add new"] ? $lang["add new"] : 'add new';
		$GLOBALS["hidden_forms"] = $GLOBALS["hidden_forms"] + 1;
		$form_id = "hidden_form_" . $GLOBALS["hidden_forms"];
		cph_tag_start("div",array("id" => $form_id));
		cph_tag_start("div",array("id" => $form_id . "_b",
			"style" => "visibility: hidden; position: absolute; z-index:1"));
		cph_tag_start("form",array("action" => ""));cph_tag_start("div");
		cph_tag_single("input", array ("type" => "button", 
			"value" => $button_title, 
			"onclick" => "c_hide('" . $form_id . "_b'); c_show('" . $form_id . "')"));
		cph_tag_end("div");cph_tag_end("form");
		cph_tag_end("div");
	
		$this->render();
	
		cph_tag_end("div");
		cph_tag("script",array("type"=>"text/javascript"),"c_show('" . $form_id . "_b'); c_hide('" . $form_id . "');");
		echo ;
	}
	
	// executes select query and render one form for each row
	function render_select() {
		cpdb_fetch_all($this->select, $this->select_args, $result);
		foreach ($result as $row) {
			$this->load_this($row);
			$this->render();
		}
	}
	
	// loads new values and renders the form
	function render_new() {
		$this->load_new();
		$this->render();
	}
}


// univerzal parent of all the form fields
// basically covers all basic things a form field should be
// able to do
class cfutral {
	var $value;
	var $field;
	var $attr;
	
	function init() {
	}
	
	function append_this($values) {
		if (isset($values[$this->field])) $this->value = $values[$this->field];
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

// just stupid static text
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

// a link
class cpf_db_link extends cfutral {
	var $href;
	var $tx;
	
	function cpf_db_link($field, $href, $tx) {
		$this->field = $field;
		$this->href = $href;
		$this->tx = $tx;
	}
	
	function render() {
		cph_link(sprintf($this->href,$this->value),htmlspecialchars($this->tx));
	}
}

// new line
class cpf_nl extends cfutral {
	
	function render() {
		cph_br();
	}
}

// clever text from the database
class cpf_db_text extends cfutral {
	
	function cpf_db_text($field, $attr = array()) {
		$this->field = $field;
		$this->attr = $attr;
	}
	
	function render() {
		if ($this->attr != array()) {
			cph_tag("span", $this->attr, htmlspecialchars($this->value));
		} else {
			echo htmlspecialchars($this->value);
		}
	}
}

// listbox, that select values from a given array
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
				// pg_achtung(sprintf("%s: match %s == %s",$this->field, $key,$this->value));
				cph_tag("option",array("value" => $key, "selected" => "selected"), htmlspecialchars($caption));
			} else {
				cph_tag("option",array("value" => $key), htmlspecialchars($caption));
			}
		}
		cph_tag_end("select");
		cph_nl();
	}
}

// listbox, that gets values from a database query
// $domain = array("select blah blah", array(":argument"=>$value))
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

// one line textbox
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

// password field
class cpf_db_password extends cfutral {

	function cpf_db_password($name, $field = "__", $attr = array()) {
		$this->attr = array_merge(array("name"=>$name), $attr);
		if ($field == "__") {
			$this->field = $name;	
		} else {
			$this->field = $field;
		}
	}
	
	function render() {
		cph_tag_single("input", array_merge(array("type"=>"password", "value"=>$this->value), $this->attr));
	}
}

// textarea -- multi line text
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
		cph_tag("textarea", $this->attr, htmlspecialchars($this->value));
	}
}

// hidden static form field.
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

// hidden form field
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

// a button
class cpf_button extends cfutral {
	
	function cpf_button($name,$caption) {
		$this->attr = array("name"=>$name, "value"=>$caption, "type"=>"submit");
	}
	
	function render() {
		cph_tag_single("input", $this->attr);
	}
}

?>
