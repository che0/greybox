<?php

session_destroy();

$GLOBALS["page"]->login_block = $lang['logging out'];
$GLOBALS["page"]->headers[] = '<meta http-equiv="refresh" content="1;url=index.php" />';

pg_achtung($lang['logout ok']);

?>
