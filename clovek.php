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
$template->editvar('debates',$lang['debates']);
$template->editvar('date',$lang['date']);
$template->editvar('affirmative',$lang['affirmative']);
$template->editvar('negative',$lang['negative']);
$template->editvar('result',$lang['result']);
$template->editvar('season',$lang['season']);
$template->editvar('accreditations',$lang['accreditations']);
$template->editvar('tournaments',$lang['tournaments']);
$template->editvar('date_from',$lang['date_from']);
$template->editvar('date_to',$lang['date_to']);
$template->editvar('title',$lang['title']);
$template->editvar('judging',$lang['judging']);
$template->editvar('coarching',$lang['coarching']);
$template->editvar('total',$lang['total']);
$template->editvar('ipoints_long',$lang['ipoints_long']);

if ($_SESSION['is_logged_in']) {
	$prava_lidi = $_SESSION['user_prava_lidi'];
} else {
	$prava_lidi = -1;
}

switch ($_GET['akce']) {
	case 'uprav':
		if (($prava_lidi < 1) && ($_SESSION['user_clovek_ID'] != $_GET['id'])) {
			// kick out people with access level lower than 1, unless the are editing themselves
			set_title($lang['access denied']);
			echo $template->make('head');
			print_one_message($lang['access denied']);
			break;
		}
		$result = mysql_query(sprintf('select * from clovek where clovek_ID = %s',$_GET['id']));
		if (! $result2 = mysql_fetch_array($result)) {
			// no found person, go out
			set_title($lang['no records']);
			echo $template->make('head');
			print_one_message($lang['no records']);
			break;
		}
		if (($prava_lidi == 1) && ($_SESSION['user_klub_ID'] != $result2['klub_ID'])) {
			// people with access level 1 can edit only member of their club
			set_title($lang['access denied']);
			echo $template->make('head');
			print_one_message($lang['access denied']);
			break;
		}
		set_title(sprintf("%s %s",$lang['edit'],join_name($result2['jmeno'],$result2['nick'],$result2['prijmeni'])));
		echo $template->make('head');
		echo $template->make('clovek_edit');

		echo '<form name="clovek.pridej" action="clovek.php?akce=uprav.commit" method="post">' . "\n";
		if ($prava_lidi > 0) {
			// normal user can't change his name
			form_textbox($lang['name'],'name', 30, $result2['jmeno'], $lang['name_desc']);
			form_textbox($lang['surname'],'surname', 30, $result2['prijmeni'], $lang['surname_desc']);
		} else {
			form_nothing($lang['name'], $result2['jmeno']);
			form_nothing($lang['surname'], $result2['prijmeni']);
		}
		form_textbox($lang['nick'],'nick', 30, $result2['nick'], $lang['nick_desc']);
		
		$result = mysql_query('select klub_ID, kratky_nazev from klub order by kratky_nazev');
		unset($clubs);
		$clubs[NULL] = $lang['no club'];
		while ($result3 = mysql_fetch_array($result)) {
			$clubs[$result3['klub_ID']]=$result3['kratky_nazev'];
		}
		if ($prava_lidi >= 2) {
			// only big boss can move people between clubs
			form_pulldown($lang['club'],'club',$clubs,$result2['klub_ID'],$lang['club_desc']);
		} else {
			form_nothing($lang['club'],$clubs[$result2['klub_ID']]);
		}
		
		if (($prava_lidi >= 1) || (! $result2['narozen'])) {
			// i can only insert my birth date, changing needs higher rights
			form_textbox($lang['born'],'born',10,$result2['narozen'],$lang['born_desc']);
		} else {
			form_nothing($lang['born'],$result2['narozen']);
		}
		
		form_textarea($lang['comment'],'comment',$result2['komentar'],$lang['comment_desc']);
		
		if ($prava_lidi > 0) {
			// only editor can switch 'active debater' status
			form_pulldown($lang['active debater'],'debater',array(1 => $lang['yes'], 0 => $lang['no']),$result2['debater'],$lang['active debater_desc']);
		} else {
			form_nothing($lang['active debater'],$result2['debater'] ? $lang['yes'] : $lang['no']);
		}
		printf('<input type="submit" value="%s" name="Submit"></form>' . "\n",$lang['submit']);
		
		// todo:
		// kontakty
		// akreditace
		// heslo/login/prava
		
	break;

	case 'uprav.commit':
		// todo

	break;
	
	case 'pridej':
		if ($prava_lidi < 1) {
			set_title($lang['access denied']);
			echo $template->make('head');
			print_one_message($lang['access denied']);
			break;
		}
		set_title($lang['add person']);
		echo $template->make('head');
		echo $template->make('clovek_new');

		echo '<form name="clovek.pridej" action="clovek.php?akce=pridej.commit" method="post">' . "\n";
		form_textbox($lang['name'],'name', 30, '', $lang['name_desc']);
		form_textbox($lang['surname'],'surname', 30, '', $lang['surname_desc']);
		form_textbox($lang['nick'],'surname', 30, '', $lang['nick_desc']);
		$result = mysql_query('select klub_ID, kratky_nazev from klub order by kratky_nazev');
		unset($clubs);
		$clubs[NULL] = $lang['no club'];
		while ($result3 = mysql_fetch_array($result)) {
			$clubs[$result3['klub_ID']]=$result3['kratky_nazev'];
		}
		if ($prava_lidi >= 2) {
			form_pulldown($lang['club'],'club',$clubs,$_SESSION['user_klub_ID'],$lang['club_desc']);
		} else {
			form_nothing($lang['club'],$clubs[$_SESSION['user_klub_ID']]);
		}
		form_textbox($lang['born'],'born',10,'1999-12-31',$lang['born_desc']);
		form_textarea($lang['comment'],'comment','',$lang['comment_desc']);
		form_pulldown($lang['active debater'],'debater',array(1 => $lang['yes'], 0 => $lang['no']),1,$lang['active debater_desc']);
		printf('<input type="submit" value="%s" name="Submit"></form>' . "\n",$lang['submit']);
	break;

	case 'pridej.commit':
		if ($prava_lidi < 1) {
			set_title($lang['access denied']);
			echo $template->make('head');
			print_one_message($lang['access denied']);
			break;
		}
		set_title($lang['add person']);
		
		$query = "insert into clovek (name, surname, nick, klub_ID, narozen, debater, komentar) values ";
		$query .= sprintf('(%s, %s, %s, %s, %s, %s, %s);',
			get_text_field($_POST['name']),
			get_text_field($_POST['surname']),
			get_text_field($_POST['nick']),
			($prava_lidi >= 2) ? get_numeric_field($_POST['klub_ID']) : $_SESSION['user_klub_ID'],
			get_text_field($_POST['narozen']),
			get_numeric_field($_POST['debater']),
			get_text_field($_POST['komentar'])
		);

		if (mysql_query($query)) {
			body_message($lang['add ok']);
			$template->editvar('page_headers',sprintf('<meta http-equiv="refresh" content="1;url=clovek.php?id=%s">',mysql_insert_id()));
		} else {
			body_message(sprintf('%s<br>%s',$lang['add failed'],mysql_error()));
		}
		
		echo $template->make('head');
		print_body_messages();


	break;

	case 'vsichni':
		// show list of people
		set_title($lang['people']);
		echo $template->make('head');

		if ($prava_lidi >= 1) {
			// if user can add new people, this is the page he can do it
			printf('<p><a href="clovek.php?akce=pridej">%s</a></p>' . "\n", $lang['add person']);
		}
		
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
			if (($_GET['id'] = $_SESSION['user_clovek_ID']) == "") {
				echo $template->make('head');
				break;
			}
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
			$got_club = 0;
			if ($result2['klub_ID']) {
				$query = sprintf('select nazev from klub where klub_ID = %s', $result2['klub_ID']);
				$result = mysql_query($query);
				if ($result3 = mysql_fetch_array($result)) {
					if (!$got_club) {
						echo $template->make('clovek_detail_kluby_head');
						$got_club = 1;
					}
					$template->editvar('line_role',$lang['member']);
					$template->editvar('line_club',sprintf('<a href="klub.php?id=%s">%s</a>', $result2['klub_ID'], $result3['nazev']));
					$template->editvar('line_season',season_to_str(get_current_season()));
					echo $template->make('clovek_detail_kluby_line');
				}
			}
			// club positions
			$query = sprintf('select klub.nazev as a_nazev, klub.klub_ID as a_klub_ID, clovek_klub.rocnik as a_rocnik, clovek_klub.role as a_role from clovek_klub left join klub using (klub_ID) where clovek_klub.clovek_ID = %s order by clovek_klub.rocnik desc',$result2['clovek_ID']);
			$result = mysql_query($query);
			while ($result3 = mysql_fetch_array($result)) {
				if (!$got_club) {
					echo $template->make('clovek_detail_kluby_head');
					$got_club = 1;
				}
				switch ($result3['a_role']) {
					case 't':
						$role = $lang['coach'];
					break;
				}
				$template->editvar('line_role',$role);
				$template->editvar('line_club',sprintf('<a href="klub.php?id=%s">%s</a>', $result3['a_klub_ID'], $result3['a_nazev']));
				$template->editvar('line_season',season_to_str($result3['a_rocnik']));
				echo $template->make('clovek_detail_kluby_line');

			}
			if ($got_club) {
				echo $template->make('clovek_detail_kluby_tail');
			}
			
			// contacts
			$query = sprintf('select druh, tx from kontakt where clovek_ID = %s and viditelnost <= %s order by druh',$result2['clovek_ID'],$adjacency);
			$result = mysql_query($query);

			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_kontakty_head');
			}
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
			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_kontakty_tail');
			}

			// accreditations
			$query = sprintf('select rozhodci.rocnik as a_rocnik, rozhodci.jazyk as a_jazyk, rozhodci.misto as a_misto, sum(clovek_debata_ibody.ibody_rozhodci) as a_ibody from rozhodci left join clovek_debata_ibody on rozhodci.rocnik = clovek_debata_ibody.rocnik and rozhodci.clovek_ID = clovek_debata_ibody.clovek_ID where rozhodci.clovek_ID = %s group by a_rocnik, a_misto, a_jazyk order by a_rocnik desc',$result2['clovek_ID']);
			$result = mysql_query($query);

			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_akreditace_head');
			}
			while ($result3 = mysql_fetch_array($result)) {
				$template->editvar('line_season',season_to_str($result3['a_rocnik']));
				$template->editvar('line_languages',$result3['a_jazyk']);
				$template->editvar('line_place',$result3['a_misto']);
				$template->editvar('line_ipoints',$result3['a_ibody']);
				echo $template->make('clovek_detail_akreditace_line');
			}
			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_akreditace_tail');
			}

			// tournaments (org)
			// TODO

			// points overview
			// TODO

			// debates
			$query = 'select debata.debata_ID as a_debata_ID, debata.datum as a_datum, clovek_debata.role as a_role, tym_aff.nazev as a_aff_nazev, tym_aff.tym_ID as a_aff_ID, tym_neg.nazev as a_neg_nazev, tym_neg.tym_ID as a_neg_ID, debata.vitez as a_vitez, sum(clovek_debata_ibody.ibody_debater) as a_ibody';
			$query .= ' from clovek_debata left join debata using(debata_ID) left join debata_tym as dt_aff on debata.debata_ID = dt_aff.debata_ID left join tym as tym_aff on dt_aff.tym_ID = tym_aff.tym_ID left join debata_tym as dt_neg on debata.debata_ID = dt_neg.debata_ID left join tym as tym_neg on dt_neg.tym_ID = tym_neg.tym_ID left join clovek_debata_ibody on clovek_debata.clovek_ID = clovek_debata_ibody.clovek_ID and debata.debata_ID = clovek_debata_ibody.debata_ID';
			$query .= sprintf(' where clovek_debata.clovek_ID = %s and dt_aff.pozice = 1 and dt_neg.pozice = 0',$result2['clovek_ID']);
			$query .= ' group by a_debata_ID, a_datum, a_role, a_aff_nazev, a_aff_ID, a_neg_nazev, a_neg_ID, a_vitez';
			$query .= ' order by a_datum desc';
			$result = mysql_query($query);

			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_debaty_head');
			}
			while ($result3 = mysql_fetch_array($result)) {
				$template->editvar('line_date',sprintf('<a href="debata.php?id=%s">%s</a>',$result3['a_debata_ID'],format_date($result3['a_datum'])));
				$template->editvar('line_affirmative', sprintf('<a href="tym.php?id=%s">%s</a>',$result3['a_aff_ID'],$result3['a_aff_nazev']));
				$template->editvar('line_negative', sprintf('<a href="tym.php?id=%s">%s</a>',$result3['a_neg_ID'],$result3['a_neg_nazev']));
				$template->editvar('line_result',$result3['a_vitez'] ? $lang['affirmative'] : $lang['negative']);
				$template->editvar('line_ipoints',$result3['a_ibody']);
				echo $template->make('clovek_detail_debaty_line');
			}
			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_debaty_tail');
			}
			
			// end of 'we found the man'
			
		} else {
			body_message($lang['no records']);
			set_title($lang['no records']);

			echo $template->make('head');
			print_body_messages();
		}	
		// end of
	break;
}

echo $template->make('tail');
require('inc/tail.php');
?>
