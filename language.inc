<?php

$_SESSION['language'] = $_REQUEST['lang'];
$GLOBALS["page"]->headers[] = '<meta http-equiv="refresh" content="1;url=' . htmlspecialchars(urldecode($_REQUEST['von'])) . '">';

pg_achtung($lang['language changed']);
?>
