<?php
// this will define functions for getting full name, recounting points, etc

function set_title($title)
{
	global $template, $setup;
	$template->editvar('page_title',$setup['global_title'].' '.$title);
	$template->editvar('pure_title',$title);
}

function body_message($text)
{
	global $body_messages;
	$body_messages .= "<p class=\"body_message\">" . $text . "</p>\n";
}

function print_one_message($text)
{
	printf('<p class="body_message">%s</p>',$text);
}

function print_body_messages()
{
	global $body_messages;
	echo $body_messages;
	$body_messages = '';
}

function join_name($first, $nick, $last)
{
	if ($nick != "") {
		return $first . ' "' . $nick . '" ' . $last;
	} else {
		return $first . ' ' . $last;
	}
}

function print_comment($comment)
{
	if ($comment != "") {
		// can later make ot process bbcode
		echo '<div class="comment">' . htmlspecialchars($comment) . '</div>';
	}
}

function get_current_season()
{
	// can be later improved with season selection
	global $setup;
	return $setup['current_season'];
}

function season_to_str($season)
{
	return ($season + 2000) . "/" . ($season + 2001);
}

function format_date($date)
{
	return $date;
}

function form_textbox($title, $name, $size, $default, $desc)
{
	printf('<p>');
	if ($title) {
		printf('%s: ',$title);
	}
	printf('<input type="text" name="%s" size="%s" value="%s">', $name, $size, $default);
	if ($desc) {
		printf(' <span class="desc">%s</span>',$desc);
	}
	printf("</p>\n");
}

function form_password($title, $name, $size, $default, $desc)
{
	printf('<p>');
	if ($title) {
		printf('%s: ',$title);
	}
	printf('<input type="password" name="%s" size="%s" value="%s">', $name, $size, $default);
	if ($desc) {
		printf(' <span class="desc">%s</span>',$desc);
	}
	printf("</p>\n");
}

function form_pulldown($title, $name, $choices, $default, $desc)
{
	printf('<p>');
	if ($title) {
		printf('%s: ',$title);
	}
	printf('<select size="1" name="%s">',$name);
	foreach ($choices as $key => $value) {
		if ($key == $default) {
			printf('<option value="%s" selected>%s</option>',$key,$value);
		} else {
			printf('<option value="%s">%s</option>',$key,$value);
		}
	}
	printf('</select>');
	if ($desc) {
		printf(' <span class="desc">%s</span>',$desc);
	}
	printf("</p>\n");
}

function form_textarea($title, $name, $default, $desc)
{
	printf('<p>');
	if ($title) {
		printf('%s:<br>',$title);
	}
	printf('<textarea name="%s" rows="7" cols="70">%s</textarea>',$name,$default);
	if ($desc) {
		printf('<br><span class="desc">%s</span>',$desc);
	}
	printf("</p>\n");
}

function form_hidden($name, $value)
{
	printf('<input type="hidden" name="%s" value="%s">' . "\n",$name,$value);
}

function form_submit($title)
{
	printf('<p><input type="submit" name="submit" value="%s"></p>' . "\n", $title);
}

function form_nothing($title, $value)
{
	printf('<p>');
	if ($title) {
		printf('%s: ',$title);
	}
	printf("%s</p>\n",$value);
}

function get_text_field ($text)
{
	if ($text) {
		return sprintf('\'%s\'',$text);
	} else {
		return "NULL";
	}
}

function get_numeric_field ($numeric)
{
	return strval($numeric);
}

function header_redirect($page)
{
	global $template;
	$template->editvar('page_headers',sprintf('<meta http-equiv="refresh" content="1;url=%s">',$page));
}

?>
