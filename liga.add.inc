<?php
/*
 * greybox
 * $Id: liga.add.inc,v 1.1 2005/03/22 18:18:22 che0 Exp $
 */

include_once("i_liga.inc");
 
if ($GLOBALS["cps_souteze"] < 2) {
	pg_achtung($lang["access denied"]);
	return;
}

pg_head();

$liga_f = get_pykhen_liga();
$liga_f->form[] = new cpf_button("add",$lang["add"]);
$liga_f->render_new();

?> 
