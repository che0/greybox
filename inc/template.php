<?php

/*******************************************************************
	template class, taken from ftpseek application
	http://sourceforge.net/projects/ftpseek/
*/

class template {
	
	var $variables, $templates, $skin;

	function template ($skin)
	{
		$this->skin = $skin;
		$this->variables = array();
	}

	function load ($filename)
	{
		// Get template file
		$file = fopen('skins/'.$this->skin.'/'.$filename.'.tpl', 'r');
		$template_file = fread($file, filesize('skins/'.$this->skin.'/'.$filename.'.tpl'));
		fclose ($file);

		// ok, for every part
		while (preg_match_all("/<!-- BEGIN (.*?) -->/", $template_file, $matches))
		{
			$template_name = $matches[1][0];

			// this adds this part to $this->template['template_name']
			preg_match_all('/<!-- BEGIN '.$template_name.' -->
(.*)<!-- END '.$template_name.' -->/s', $template_file, $matches);
			// ^ warning: line feed in the string
			$this->templates[$template_name] = $matches[1][0];

			// this takes this template part out of $template_file
			$template_file = preg_replace('/<!-- BEGIN '.$template_name.' -->
.*<!-- END '.$template_name.' -->/s', '//', $template_file);
			// ^ warning: line feed in the string
		}
	}

	function editvar ($key, $value)
	{
		$this->variables['{'.$key.'}'] = $value;
	}

	function make ($name)
	{
		$template = str_replace(array_keys($this->variables), array_values($this->variables), $this->templates[$name]);
		return $template;
	}

}
?>
