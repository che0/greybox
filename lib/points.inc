<?php

function ipts_recount_clovek_role($clovek_id, $role) {
	// vycisti vsechny ibody dane role pro cloveka a spocita je znovu
	// FIXME
}

function ipts_recount_clovek($clovek_id) {
	// vycisti a prepocita vsechny ibody pro clovek
	// FIXME
}

function ipts_recount_debate($debata_id) {
	// vycisti a prepocita vsechny ibody pro debatu
	// FIXME
}

function ipts_recount() {
	// vycisti a prepocita vsechny ibody
	// FIXME
}

function recount_debate($debata_id) {
	// prepocita vysledky a body v debate
	// FIXME
	// if (cpdb_fetch_all("select soutez_ID from soutez_teze where teze.teze_ID = soutez_teze.teze_ID and soutez_ID = :soutez_id and teze_ID = :teze_id",array(":soutez_id"=>$debata["soutez_ID"], ":teze_ID"=>$debata["teze_ID"]), $neco) == 1) { teze je oficialni }
}

function recount_competition($soutez_id) {
	// prepocita vysledky vsech debat v soutezi
	// FIXME
}

function recount_all_debates() {
	// prepocita vysledky a body u vsech debat
	// FIXME
}

?>