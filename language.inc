<?php
/*
 * greybox
 * $Id: language.inc,v 1.4 2005/01/09 00:47:32 che0 Exp $
 */

$_SESSION['language'] = $_REQUEST['jazyk'];
$GLOBALS["page"]->headers[] = '<meta http-equiv="refresh" content="1;url=' . htmlspecialchars(urldecode($_REQUEST['von'])) . '" />';

pg_achtung($lang['language changed']);
?>
