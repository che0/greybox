<?php
// clovek.php
require('inc/head.php');

$template->load('clovek');
$template->editvar('name',$lang['name']);
$template->editvar('club',$lang['club']);
$template->editvar('team',$lang['team']);
$template->editvar('ipoints',$lang['ipoints']);
$template->editvar('line_empty',$lang['no records']);

switch ($_POST['akce']) {
	// nothing yet
}

switch ($_GET['akce']) {
	case 'uprav':
	case 'pridej':
		// edit someones record or add new one

		break;

	case 'vsichni':
		// show list of people
		set_title($lang['people']);
		echo $template->make('head');
		
		$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.klub_ID as a_klub_ID, tym.tym_ID as a_tym_ID, tym.nazev as a_tym, klub.kratky_nazev as a_klub, sum(ibody_debater + ibody_rozhodci + ibody_trener + ibody_organizator) as ibody';
		$query .= ' from (((clovek left join klub on clovek.klub_ID <=> klub.klub_ID) left join clovek_tym on clovek.clovek_ID <=> clovek_tym.clovek_ID) left join tym on clovek_tym.tym_ID <=> tym.tym_ID) left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID';
		$query .= ' where (clovek_tym.aktivni is null or clovek_tym.aktivni = 1) and (clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = ' . get_current_season() . ')';
		$query .= ' group by clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, tym.tym_ID, tym.nazev, klub.kratky_nazev';
		$query .= ' order by clovek.prijmeni, clovek.jmeno, clovek.nick';
		$result = mysql_query($query);
		echo mysql_error();

		echo $template->make('clovek_list_all_head');
		
		if ($result2 = mysql_fetch_array($result)) {
			$id = $result2['a_clovek_ID'];
			$name = sprintf('<a href="clovek.php?id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni']));
			if ($result2['a_klub_ID']) { $klub = sprintf('<a href="klub.php?id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
			if ($result2['a_tym_ID']) { $tym = sprintf('<a href="tym.php?id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']); } else { $tym = ''; }
			$ibody = $result2['ibody'];
			while($result2 = mysql_fetch_array($result)) {
				if ($result2['a_clovek_ID'] == $id) {
					// second team for an already read person
					$tym .= sprintf(', <a href="tym.php?id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']);
				} else {
					$template->editvar('line_name',$name);
					$template->editvar('line_club',$klub);
					$template->editvar('line_team',$tym);
					$template->editvar('line_ipoints',$ibody);
					echo $template->make('clovek_list_all_line');

					$id = $result2['a_clovek_ID'];
					$name = sprintf('<a href="clovek.php?id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni']));
					if ($result2['a_klub_ID']) { $klub = sprintf('<a href="klub.php?id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
					if ($result2['a_tym_ID']) { $tym = sprintf('<a href="tym.php?id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']); } else { $tym = ''; }
					$ibody = $result2['ibody'];
				}
			}
			// send the last cached line
			$template->editvar('line_name',$name);
			$template->editvar('line_club',$klub);
			$template->editvar('line_team',$tym);
			$template->editvar('line_ipoints',$ibody);
			echo $template->make('clovek_list_all_line');
		} else {
			echo $template->make('clovek_list_all_empty');
		}
		echo $template->make('clovek_list_all_tail');
				
		break;
	
	case 'debateri':
		break;

	case 'rozhodci':
		break;

	default:
		if ($_GET['id'] != "") {
			// one person's data
		}
		break;
}

echo $template->make('tail');
require('inc/tail.php');
?>
