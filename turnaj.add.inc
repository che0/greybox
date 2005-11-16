<?php
/*
 * greybox
 * $Id: turnaj.add.inc,v 1.7 2005/11/16 07:45:08 che0 Exp $
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
