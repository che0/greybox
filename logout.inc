<?php
/*
 * greybox
 * $Id: logout.inc,v 1.3 2005/01/09 00:47:32 che0 Exp $
 */

session_destroy();

$GLOBALS["page"]->login_block = $lang['logging out'];
$GLOBALS["page"]->headers[] = '<meta http-equiv="refresh" content="1;url=index.php" />';

pg_achtung($lang['logout ok']);

?>
