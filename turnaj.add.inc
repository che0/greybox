g<?php
/*
 * greybox
 * $Id: turnaj.add.inc,v 1.6 2005/03/29 10:09:50 che0 Exp $
 */

include_once("i_turnaj.inc");

if ($GLOBALS["cps_souteze"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

pg_head();

$turnaj_f = get_pykhen_turnaj();
$turnaj_f->form[] = new cpf_button("add",$lang["add"]);
$turnaj_f->render_new();

?> 
