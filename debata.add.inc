<?php
/*
 * greybox
 * $Id: debata.add.inc,v 1.4 2005/01/09 00:47:32 che0 Exp $
 */
if ($GLOBALS["cps_debaty"] < 1) {
	pg_achtung($lang["access denied"]);
	return;
}

pg_head();

// build teams domain
$sdomain_tym = array(
	"select
		tym.tym_ID,
		concat(tym.nazev, ' (', klub.kratky_nazev, ')') as tym
	from
		tym
		left join klub using (klub_ID)
	order by
		tym.nazev
	", array()
);

// selects for debater domains
$select_debaters = "
		(
		select
			clovek.clovek_ID,
			concat_ws(' ', clovek.jmeno, clovek.nick, clovek.prijmeni) as clovek,
			concat_ws(' ', clovek.prijmeni,clovek.jmeno,clovek.nick) as s_clovek
		from
			clovek_tym
			left join clovek using (clovek_ID)
		where
			clovek_tym.aktivni = 1
			and clovek.debater = 1
			%s
		)
		union
		(
		select
			0 as clovek_ID,
			:null as clovek,
			'' as s_clovek
		)
		order by
			s_clovek
	";

// build affirmative debaters domain
if (isset($_SESSION["dbt_cache"]["aff_ID"])) {
	// choose affirmatives from the team
	$sdomain_aff = array(sprintf($select_debaters,"and clovek_tym.tym_ID = :tym_id"), array(":tym_id"=>$_SESSION["dbt_cache"]["aff_ID"], ":null"=>$lang["N/A"]));
} else {
	// choose from all debaters
	$sdomain_aff = array(sprintf($select_debaters,""), array(":null"=>$lang["N/A"]));
}

// build negative debaters domain
if (isset($_SESSION["dbt_cache"]["neg_ID"])) {
	// choose negatives from the team
	$sdomain_neg = array(sprintf($select_debaters,"and clovek_tym.tym_ID = :tym_id"), array(":tym_id"=>$_SESSION["dbt_cache"]["neg_ID"], ":null"=>$lang["N/A"]));
} else {
	// choose from all debaters
	$sdomain_neg = array(sprintf($select_debaters,""), array(":null"=>$lang["N/A"]));
}

// build resolutions domain
$sdomain_teze = array(
	sprintf("select teze_ID, tx as tx from teze %s order by tx", isset($_SESSION["dbt_cache"]["jazyk"]) ? "where jazyk = :jazyk" : ""),
	array(":jazyk"=>$_SESSION["dbt_cache"]["jazyk"])
);

// build competitions domain
$sdomain_souteze = array(
	"select soutez_ID, nazev from soutez where rocnik = :rocnik",
	array(":rocnik"=>$GLOBALS["current_season"])
);

// build tournaments domain
$sdomain_turnaje = array(
	sprintf("
		(select turnaj_ID, nazev, nazev as nazev_s from turnaj %s)
		union
		(select 0, :null, '' as nazev_s)
		order by nazev_s
	", isset($_SESSION["dbt_cache"]["soutez_ID"]) ? "where soutez_ID = :soutez_id" : ""),
	array(":soutez_id"=>$_SESSION["dbt_cache"]["soutez_ID"], ":null"=>$lang["N/A"])
);

// pridani result = [AN][+-]
$dres = $_SESSION["dbt_cache"]["vitez"] ? "A" : "N";
$dres .= $_SESSION["dbt_cache"]["presvedcive"] ? "+" : "-";
$_SESSION["dbt_cache"]["vysledek"] = $dres;

// pykhen na zakladni vlastnosti
$dpyk = new cpykhen;
$dpyk->form_attr["action"] = "./?page=debata.edit.exec";
$dpyk->form[] = new cpf_db_hidden("debata_id","debata_ID");
$dpyk->form[] = new cpf_text($lang["competition"] . ": ");
$dpyk->form[] = new cpf_db_listbox("soutez_id", $sdomain_souteze, "soutez_ID");
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["tournament"] . ": ");
$dpyk->form[] = new cpf_db_listbox("turnaj_id", $sdomain_turnaje, "turnaj_ID");
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["date"] . ": ");
$dpyk->form[] = new cpf_db_textbox("datum","datum",array("size"=>"11"));
$dpyk->form[] = new cpf_text(" " . $lang["yyyy-mm-dd"],array("class"=>"desc"));
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["place"] . ": ");
$dpyk->form[] = new cpf_db_textbox("misto");
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["resolution"] . ": ");
$dpyk->form[] = new cpf_db_listbox("teze_id", $sdomain_teze, "teze_ID");

$dpyk->form[] = new cpf_text($lang["affirmative"],array("class"=>"p_h2"));
$dpyk->form[] = new cpf_text($lang["team"] . ": ");
$dpyk->form[] = new cpf_db_listbox("aff_id", $sdomain_tym, "aff_ID");
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["1st speaker"] . ": ");
$dpyk->form[] = new cpf_db_listbox("a1_id", $sdomain_aff, "a1_ID");
$dpyk->form[] = new cpf_text(" " . $lang["kidy"] . ": ");
$dpyk->form[] = new cpf_db_textbox("a1_kidy","a1_kidy",array("size"=>"3"));
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["2nd speaker"] . ": ");
$dpyk->form[] = new cpf_db_listbox("a2_id", $sdomain_aff, "a2_ID");
$dpyk->form[] = new cpf_text(" " . $lang["kidy"] . ": ");
$dpyk->form[] = new cpf_db_textbox("a2_kidy","a2_kidy",array("size"=>"3"));
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["3rd speaker"] . ": ");
$dpyk->form[] = new cpf_db_listbox("a3_id", $sdomain_aff, "a3_ID");
$dpyk->form[] = new cpf_text(" " . $lang["kidy"] . ": ");
$dpyk->form[] = new cpf_db_textbox("a3_kidy","a3_kidy",array("size"=>"3"));
$dpyk->form[] = new cpf_nl;

$dpyk->form[] = new cpf_text($lang["negative"],array("class"=>"p_h2"));
$dpyk->form[] = new cpf_text($lang["team"] . ": ");
$dpyk->form[] = new cpf_db_listbox("neg_id", $sdomain_tym, "neg_ID");
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["1st speaker"] . ": ");
$dpyk->form[] = new cpf_db_listbox("n1_id", $sdomain_neg, "n1_ID");
$dpyk->form[] = new cpf_text(" " . $lang["kidy"] . ": ");
$dpyk->form[] = new cpf_db_textbox("n1_kidy","n1_kidy",array("size"=>"3"));
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["2nd speaker"] . ": ");
$dpyk->form[] = new cpf_db_listbox("n2_id", $sdomain_neg, "n2_ID");
$dpyk->form[] = new cpf_text(" " . $lang["kidy"] . ": ");
$dpyk->form[] = new cpf_db_textbox("n2_kidy","n2_kidy",array("size"=>"3"));
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["3rd speaker"] . ": ");
$dpyk->form[] = new cpf_db_listbox("n3_id", $sdomain_neg, "n3_ID");
$dpyk->form[] = new cpf_text(" " . $lang["kidy"] . ": ");
$dpyk->form[] = new cpf_db_textbox("n3_kidy","n3_kidy",array("size"=>"3"));
$dpyk->form[] = new cpf_nl;

$dpyk->form[] = new cpf_text($lang["result"],array("class"=>"p_h2"));
$dpyk->form[] = new cpf_text($lang["result"] . ": ");
$dpyk->form[] = new cpf_db_listbox_array("vysledek", array(
	"A+" => $lang["aff"] . " " . $lang["3:0"],
	"A-" => $lang["aff"] . " " . $lang["2:1"],
	"N+" => $lang["neg"] . " " . $lang["3:0"],
	"N-" => $lang["neg"] . " " . $lang["2:1"]
));
$dpyk->form[] = new cpf_text(" " . $lang["applies only if there are no adjudicators filled in"], array("class"=>"desc"));
$dpyk->form[] = new cpf_nl;
$dpyk->form[] = new cpf_text($lang["comment"] . ": ");
$dpyk->form[] = new cpf_db_textarea("komentar","komentar",array("rows"=>4, "cols"=>60));
$dpyk->form[] = new cpf_nl;

$dpyk->form[] = new cpf_button("add",$lang["add"]);
$dpyk->load_this($_SESSION["dbt_cache"]);
$dpyk->render();


?>
