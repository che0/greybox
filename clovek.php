<?php
// clovek.php
require('inc/head.php');

$template->load('clovek');
$template->editvar('name',$lang['name']);
$template->editvar('club',$lang['club']);
$template->editvar('team',$lang['team']);
$template->editvar('languages',$lang['languages']);
$template->editvar('place',$lang['place']);
$template->editvar('ipoints',$lang['ipoints']);
$template->editvar('line_empty',$lang['no records']);
$template->editvar('num_rows',$lang['number of records']);
$template->editvar('list',$lang['list']);
$template->editvar('everyone',$lang['everyone']);
$template->editvar('judges',$lang['judges']);
$template->editvar('debaters',$lang['debaters']);
$template->editvar('contacts',$lang['contacts']);

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
		
		$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.klub_ID as a_klub_ID, tym.tym_ID as a_tym_ID, tym.nazev as a_tym, klub.kratky_nazev as a_klub, sum(ibody_debater) + sum(ibody_rozhodci) + sum(ibody_trener) + sum(ibody_organizator) as ibody';
		$query .= ' from (((clovek left join klub on clovek.klub_ID <=> klub.klub_ID) left join clovek_tym on clovek.clovek_ID <=> clovek_tym.clovek_ID) left join tym on clovek_tym.tym_ID <=> tym.tym_ID) left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID';
		$query .= sprintf(' where (clovek_tym.aktivni is null or clovek_tym.aktivni = 1) and (clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = %s)', get_current_season());
		$query .= ' group by clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, tym.tym_ID, tym.nazev, klub.kratky_nazev';
		$query .= ' order by clovek.prijmeni, clovek.jmeno, clovek.nick';
		$result = mysql_query($query);

		echo $template->make('clovek_list_all_head');
		
		$num_rows = 0;
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
					$num_rows++;

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
			$num_rows++;
		} else {
			echo $template->make('clovek_list_all_empty');
		}
		$template->editvar('line_total',$num_rows);
		echo $template->make('clovek_list_all_tail');
				
		break;
	
	case 'debateri':
		// show list of debaters
		set_title($lang['debaters']);
		echo $template->make('head');
		
		$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.klub_ID as a_klub_ID, tym.tym_ID as a_tym_ID, tym.nazev as a_tym, klub.kratky_nazev as a_klub, sum(ibody_debater) as ibody';
		$query .= ' from (((clovek left join klub on clovek.klub_ID <=> klub.klub_ID) left join clovek_tym on clovek.clovek_ID <=> clovek_tym.clovek_ID) left join tym on clovek_tym.tym_ID <=> tym.tym_ID) left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID';
		$query .= sprintf(' where clovek.debater = 1 and (clovek_tym.aktivni is null or clovek_tym.aktivni = 1) and (clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = %s)', get_current_season());
		$query .= ' group by clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, tym.tym_ID, tym.nazev, klub.kratky_nazev';
		$query .= ' order by clovek.prijmeni, clovek.jmeno, clovek.nick';
		$result = mysql_query($query);

		echo $template->make('clovek_list_debaters_head');
		
		$num_rows = 0;
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
					$num_rows++;

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
			$num_rows++;
		} else {
			echo $template->make('clovek_list_all_empty');
		}
		$template->editvar('line_total',$num_rows);
		echo $template->make('clovek_list_all_tail');
		break;

	case 'rozhodci':
		set_title($lang['judges']);
		echo $template->make('head');
		
		$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.klub_ID as a_klub_ID, rozhodci.jazyk as a_jazyk, rozhodci.misto as a_misto, klub.kratky_nazev as a_klub, sum(ibody_rozhodci) as ibody';
		$query .= ' from ((clovek left join klub on clovek.klub_ID <=> klub.klub_ID) left join rozhodci on clovek.clovek_ID <=> rozhodci.clovek_ID) left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID';
		$query .= sprintf(' where (rozhodci.rocnik = %s) and (clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = %s)',get_current_season(),get_current_season());
		$query .= ' group by clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, rozhodci.jazyk, klub.kratky_nazev';
		$query .= ' order by clovek.prijmeni, clovek.jmeno, clovek.nick';
		$result = mysql_query($query);
		echo mysql_error();

		echo $template->make('clovek_list_judges_head');
		
		$one_line = 0;
		while ($result2 = mysql_fetch_array($result)) {
			$one_line = 1;
			$template->editvar('line_name', sprintf('<a href="clovek.php?id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni'])));
			if ($result2['a_klub_ID']) { $klub = sprintf('<a href="klub.php?id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
			$template->editvar('line_club',$klub);
			$template->editvar('line_languages',$result2['a_jazyk']);
			$template->editvar('line_place',$result2['a_misto']);
			$template->editvar('line_ipoints', $result2['ibody']);
			echo $template->make('clovek_list_judges_line');
		}
		if ($one_line == 0) echo $template->make('clovek_list_judges_empty');
		$template->editvar('line_total',mysql_num_rows($result));
		echo $template->make('clovek_list_judges_tail');
		break;

	default:
		if ($_GET['id'] == "") {
			$_GET['id'] = $_SESSION['user_clovek_ID'];
		}
		$query = 'select * from clovek where clovek_ID = ' . $_GET['id'];
		$result = mysql_query($query);
		if ($result2 = mysql_fetch_array($result)) {
			$can_see_birth = $_SESSION['user_prava_lidi'] >= 2 || ($_SESSION['user_prava_souteze'] >= 1) || ($_SESSION['user_prava_lidi'] == 1 && $_SESSION['user_klub_ID'] === $result2['a_klub_ID']) || $_GET['id'] == $_SESSION['user_id'];
			if ($_SESSION['is_logged_in']) {
				if (($_SESSION['user_klub_ID'] == $result['klub_ID']) || ($_SESSION['user_prava_lidi'] >= 2) || ($_SESSION['user_prava_souteze'] >= 1)) {
					$adjacency = 2;
				} else {
					$adjacency = 1;
				}
			} else {
				$adjacency = 0;
			}
			
			set_title(join_name($result2['jmeno'], $result2['nick'], $result2['prijmeni']));

			echo $template->make('head');
			print_comment($result2['komentar']);

			// club
			echo sprintf("<h2>%s</h2>",$lang['club']);
			$got_club = 0;
			if ($result2['klub_ID']) {
				$query = sprintf('select nazev from klub where klub_ID = %s', $result2['klub_ID']);
				$result = mysql_query($query);
				if ($result3 = mysql_fetch_array($result)) {
					$got_club = 1;
					printf('<p class="klub">%s: <a href="klub.php?id=%s">%s</a></p>',$lang['member'], $result2['klub_ID'], $result3['nazev']);
				}
			}
			// club positions
			$query = sprintf('select klub.nazev as a_nazev, klub.klub_ID as a_klub_ID, clovek_klub.rocnik as a_rocnik, clovek_klub.role as a_role from clovek_klub left join klub using (klub_ID) where clovek_klub.clovek_ID = %s order by clovek_klub.rocnik desc',$result2['clovek_ID']);
			$result = mysql_query($query);
			while ($result3 = mysql_fetch_array($result)) {
				$got_club = 1;
				switch ($result3['a_role']) {
					case 't':
						$role = $lang['coach'];
						break;
				}
				printf('<p class="klub">%s: <a href="klub.php?id=%s">%s</a> (%s)</p>', $role, $result3['a_klub_ID'], $result3['a_nazev'], season_to_str($result3['a_rocnik']));
			}
			if (! $got_club) { printf ('<p class="klub">%s</p>',$lang['no records']); }
			
			// contacts
			$query = sprintf('select druh, tx from kontakt where clovek_ID = %s and viditelnost <= %s order by druh',$result2['clovek_ID'],$adjacency);
			$result = mysql_query($query);

			echo $template->make('clovek_detail_kontakty_head');
			while ($result3 = mysql_fetch_array($result)) {
				switch ($result3['druh']) {
					case 'telefon':
						$template->editvar('line_type',$lang['phone']);
						$template->editvar('line_tx',$result3['tx']);
						break;
					case 'email':
						$template->editvar('line_type',$lang['email']);
						$template->editvar('line_tx',sprintf('<a href="mailto:%s">%s</a>',$result3['tx'],$result3['tx']));
						break;
					case 'adresa':
						$template->editvar('line_type',$lang['postal']);
						$template->editvar('line_tx',$result3['tx']);
						break;
					case 'icq':
						$template->editvar('line_type',$lang['icq']);
						$template->editvar('line_tx',sprintf('<a href="http://web.icq.com/wwp?Uin=%s">%s</a>',$result3['tx'],$result3['tx']));
						break;
					case 'jabber':
						$template->editvar('line_type',$lang['jabber']);
						$template->editvar('line_tx',$result3['tx']);
						break;
					case 'web':
						$template->editvar('line_type',$lang['web']);
						$template->editvar('line_tx',sprintf('<a href="%s">%s</a>',$result3['tx'],$result3['tx']));
						break;
				}
				echo $template->make('clovek_detail_kontakty_line');
			}
			if (mysql_num_rows($result) == 0) {
				echo $template->make('clovek_detail_kontakty_empty');
			}
			echo $template->make('clovek_detail_kontakty_tail');
			
			
		} else {
			body_message($lang['no records']);
			set_title($lang['no records']);

			echo $template->make('head');
			print_body_messages();
		}	
		break;
}

echo $template->make('tail');
require('inc/tail.php');
?>
