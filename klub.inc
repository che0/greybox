<?php

$klub_id = $_REQUEST["klub_id"];

cpdb_fetch_one_row("select * from klub where klub_ID = :klub_id",array(":klub_id"=>$klub_id), $klub);

$can_edit = (($GLOBALS["cps_kluby"] >= 2)
	|| (($GLOBALS["cps_kluby"] == 1) && ($klub_id == $_SESSION['user_klub_ID'])));

$GLOBALS["page"]->title = $klub['nazev'];
pg_head();

if ($can_edit) {
	$klub_f = new cpykhen;
	$klub_f->form_attr["action"] = "./?page=klub.edit.exec";
	$klub_f->form[] = new cpf_db_hidden("klub_id","klub_ID");
	$klub_f->form[] = new cpf_text($lang["club name"] . ": ");
	$klub_f->form[] = new cpf_db_textbox("nazev");
	$klub_f->form[] = new cpf_nl;
	$klub_f->form[] = new cpf_text($lang["short name"] . ": ");
	$klub_f->form[] = new cpf_db_textbox("kratky_nazev");
	$klub_f->form[] = new cpf_nl;
	$klub_f->form[] = new cpf_text($lang["place"] . ": ");
	$klub_f->form[] = new cpf_db_textbox("misto");
	$klub_f->form[] = new cpf_nl;
	$klub_f->form[] = new cpf_text($lang["comment"] . ": ");
	$klub_f->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
	$klub_f->form[] = new cpf_nl;
	$klub_f->form[] = new cpf_button("save",$lang["save"]);
	$klub_f->form[] = new cpf_button("delete",$lang["delete"]);
	$klub_f->load_this($klub);
	$klub_f->render();
}

// teams
cph_h2($lang["teams"]);

if ($can_edit) cph_p(cphs_link("./?page=tym.add&klub_id=" . $klub_id,$lang['add team']));

cpdb_fetch_all("select tym_ID, tym.nazev from tym where klub_ID = :klub_id",array(":klub_id"=>$klub_id),$tymy);

cph_table_head(array($lang["team"],$lang["debaters"]),array("class"=>"visible"));
foreach ($tymy as $tym) {
	$debaters = cpdb_fetch_one_value("
		select
			count(clovek.clovek_ID)
		from
			clovek_tym
			left join clovek using(clovek_ID)
		where
			clovek_tym.aktivni = 1
			and clovek.debater = 1
			and clovek_tym.tym_ID = :tym_id
		", array(":tym_id"=>$tym["tym_ID"]));
	cph_table_row(array(
		cphs_link("./?page=tym&tym_id=" . $tym["tym_ID"], $tym["nazev"]),
		$debaters
	), ($debaters >= 3) ? array() : array("class"=>"inactive"));
}
cph_table_end();

// members
cph_h2($lang['members']);

if ($can_edit) {
	cph_p(cphs_link("./?page=clovek.add&klub_id=" . $klub_id ,$lang['add person']));
}

cpdb_fetch_all("
	select
		clovek.clovek_ID as a_clovek_ID,
		clovek.jmeno as a_jmeno,
		clovek.nick as a_nick,
		clovek.prijmeni as a_prijmeni,
		clovek.debater as a_debater,
		tym.tym_ID as a_tym_ID,
		tym.nazev as a_tym
	from
		clovek
		left join clovek_tym on clovek.clovek_ID = clovek_tym.clovek_ID
		left join tym on clovek_tym.tym_ID <=> tym.tym_ID
	where
		clovek.klub_ID = :klub_id
	order by
		a_prijmeni,
		a_jmeno,
		a_nick
	",array(":klub_id"=>$klub_id), $clenove);

cph_table_head(array($lang['name'], $lang['team'], $lang['active debater_short']),array("class"=>"visible"));

while ($clen = current($clenove)) {
	// first result fetch
	$cl_clovek_ID = $clen['a_clovek_ID'];
	$cl_name = cphs_link("./?page=clovek&id=" . $clen['a_clovek_ID'], join_name($clen['a_jmeno'], $clen['a_nick'], $clen['a_prijmeni']));
	if ($clen['a_tym_ID']) {
		$cl_team = cphs_link("./?page=tym&tym_id=" . $clen['a_tym_ID'], $clen['a_tym']);
	} else {
		$cl_team = '';
	}
	$cl_debater = $clen['a_debater'];

	while ($clen = next($clenove)) {
		// check for added club, either add club, or print & fetch
		if ($clen['a_clovek_ID'] == $cl_clovek_ID) {
			$cl_team .= ", " . cphs_link("./?page=tym&tym_id=" . $clen['a_tym_ID'], $clen['a_tym']);
		} else {
			cph_table_row(array($cl_name, $cl_team, $cl_debater ? $lang['active'] : $lang['no']), $cl_debater ? array() : array("class"=>"inactive"));
			
			$cl_clovek_ID = $clen['a_clovek_ID'];
			$cl_name = cphs_link("./?page=clovek&id=" . $clen['a_clovek_ID'], join_name($clen['a_jmeno'], $clen['a_nick'], $clen['a_prijmeni']));
			if ($clen['a_tym_ID']) {
				$cl_team = cphs_link("./?page=tym&tym_id=" . $clen['a_tym_ID'], $clen['a_tym']);
			} else {
				$cl_team = '';
			}
			$cl_debater = $clen['a_debater'];
		}
	}
	// print last one
	echo $clen['a_debater'];
	cph_table_row(array($cl_name, $cl_team, $cl_debater ? $lang['active'] : $lang['no']), $cl_debater ? array() : array("class"=>"inactive"));
	next($clenove);
}

cph_table_end();

// judges
// FIXME

// coaches
// FIXME


?>
