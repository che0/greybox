<?php
// clovek.inc

// stare funkce
include_once("inc/functions.php");
include_once("inc/template.php");

$template = new template("default");
$template->load("clovek");
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
$template->editvar('coaching',$lang['coaching']);
$template->editvar('total',$lang['total']);
$template->editvar('ipoints_long',$lang['ipoints_long']);

if (!isset($_GET["id"])) $_GET["id"] = $_REQUEST["clovek_id"];

switch ($_GET['akce']) {
	case 'uprav':
		if (($prava_lidi < 1) && ($_SESSION['user_clovek_ID'] != $_GET['id'])) {
			// kick out people with access level lower than 1, unless the are editing themselves
			set_title($lang['access denied']);
			pg_head();
			print_one_message($lang['access denied']);
			break;
		}
		$result = mysql_query(sprintf('select * from clovek where clovek_ID = %s',$_GET['id']));
		if (! $result2 = mysql_fetch_array($result)) {
			// no found person, go out
			set_title($lang['no records']);
			pg_head();
			print_one_message($lang['no records']);
			break;
		}
		if (($prava_lidi == 1) && ($_SESSION['user_klub_ID'] != $result2['klub_ID'])) {
			// people with access level 1 can edit only member of their club
			set_title($lang['access denied']);
			pg_head();
			print_one_message($lang['access denied']);
			break;
		}
		set_title(sprintf("%s %s",$lang['edit'],join_name($result2['jmeno'],$result2['nick'],$result2['prijmeni'])));
		pg_head();

		echo '<form name="clovek.pridej" action="./?page=clovek&akce=uprav.commit" method="post">' . "\n";
		form_hidden('clovek_ID',$result2['clovek_ID']);
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
		$clubs[-1] = $lang['no club'];
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
		
		// kontakty
		$query = sprintf('select kontakt_ID, druh, tx, viditelnost from kontakt where clovek_ID = %s', $result2['clovek_ID']);
		$result = mysql_query($query);

		unset($viditelnost);
		$viditelnost[0] = $lang['public'];
		$viditelnost[1] = $lang['registered'];
		$viditelnost[2] = $lang['club only'];
		
		unset($druh);
		$druh['DELETE'] = $lang['delete'];
		$druh['telefon']= $lang['phone'];
		$druh['email'] = $lang['email'];
		$druh['adresa'] = $lang['postal'];
		$druh['icq'] = $lang['icq'];
		$druh['jabber'] = $lang['jabber'];
		$druh['web'] = $lang['web'];
		
		printf('<h2>%s</h2>',$lang['contacts']);
		echo '<table class="visible">' . "\n";
		while ($result3 = mysql_fetch_array($result)) {
			echo '<form name="clovek_kontakt_'.$result3['kontakt_ID'].'" action="./?page=clovek&akce=uprav.commit.kontakty" method="post">' . "\n";
			form_hidden('clovek_ID',$result2['clovek_ID']);	
			form_hidden('kontakt_ID',$result3['kontakt_ID']);
			echo '<tr><td>';
			form_pulldown('','type',$druh,$result3['druh'],'');
			echo '</td><td>';
			form_textbox('','tx',30,$result3['tx'],'');
			echo '</td><td>';
			form_pulldown('','visibility',$viditelnost,$result3['viditelnost'],'');
			echo '</td><td>' . "\n";
			printf('<input type="submit" value="%s" name="Submit">' . "\n",$lang['submit']);
			echo '</td></tr></form>' . "\n";
		}
		unset($druh['DELETE']);

		echo '<form name="clovek_kontakt_add" action="./?page=clovek&akce=uprav.commit.kontakty" method="post">' . "\n";
		form_hidden('clovek_ID',$result2['clovek_ID']);	
		form_hidden('kontakt_ID','NEW');
		echo '<tr><td>';
		form_pulldown('','type',$druh,'email','');
		echo '</td><td>';
		form_textbox('','tx',30,'','');
		echo '</td><td>';
		form_pulldown('','visibility',$viditelnost,0,'');
		echo '</td><td>' . "\n";
		printf('<input type="submit" value="%s" name="Submit">' . "\n",$lang['add']);
		echo '</td></tr></form>' . "\n";
		
		echo '</table>' . "\n";

		// todo:
		// akreditace

		// delete
		if ($prava_lidi >= 1) {
			printf('<h2>%s</h2>',$lang['delete']);
			echo '<form name="clovek_delete" action="./?page=clovek&akce=uprav.commit.delete" method="post">';
			form_hidden('clovek_ID',$_GET['id']);
			form_pulldown($lang['delete'],'confirm',array('no' => $lang['no'], 'yes' => $lang['im sure']),'no',$lang['delete_desc']);
			printf('<input type="submit" value="%s" name="Submit">', $lang['delete']);
			echo '</form>' . "\n";
		}
		
		// heslo/login/prava
		if (($prava_lidi >= 3) || ($_SESSION['user_clovek_ID'] == $_GET['id'])) {
			printf('<h2>%s</h2>',$lang['access']);
			$query = sprintf('select username from login where clovek_ID = %s', $_GET['id']);
			$result = mysql_query($query);

			if ($result3 = mysql_fetch_array($result)) {
				if ($prava_lidi >= 3) {
					echo '<form name="clovek_access_edit" action="./?page=clovek&akce=uprav.commit.access.edit" method="post">' . "\n";
					form_hidden('clovek_ID',$_GET['id']);
					form_textbox($lang['username'],'username',30,$result3['username'],$lang['username_desc']);
					form_pulldown($lang['access people'], 'prava_lidi',
						array(
						0 => $lang['access people0'],
						1 => $lang['access people1'],
						2 => $lang['access people2'],
						3 => $lang['access people3']
						), $result2['prava_lidi'], $lang['access people_desc']);
					form_pulldown($lang['access clubs'], 'prava_kluby',
						array(
						0 => $lang['access clubs0'],
						1 => $lang['access clubs1'],
						2 => $lang['access clubs2']
						), $result2['prava_kluby'], $lang['access clubs_desc']);
					form_pulldown($lang['access competitions'], 'prava_souteze',
						array(
						0 => $lang['access competitions0'],
						1 => $lang['access competitions1'],
						2 => $lang['access competitions2']
						), $result2['prava_souteze'], $lang['access competitions_desc']);
					form_pulldown($lang['access debates'], 'prava_debaty',
						array(
						0 => $lang['access debates0'],
						1 => $lang['access debates1']
						), $result2['prava_debaty'], $lang['access debates_desc']);
					printf('<input type="submit" value="%s" name="Submit">', $lang['submit']);
					echo '</form>' . "\n";	

					echo '<form name="clovek_access_delete" action="./?page=clovek&akce=uprav.commit.access.delete" method="post">' . "\n";
					form_hidden('clovek_ID',$_GET['id']);
					form_pulldown($lang['delete account'], 'confirm', array('no' => $lang['no'], 'yes' => $lang['im sure']), 'no', $lang['delete account_desc']);
					printf('<input type="submit" value="%s" name="Submit">', $lang['delete']);
					echo '</form>' . "\n";	
				}

				echo '<form name="clovek_access_pw" action="./?page=clovek&akce=uprav.commit.access.pw" method="post">' . "\n";
				if ($prava_lidi < 3) form_nothing($lang['username'],$result3['username']);
				form_hidden('clovek_ID',$_GET['id']);
				form_password($lang['password'],'pw',30,'',$lang['password_desc']);
				form_password($lang['password'],'pw2',30,'',$lang['password_desc2']);
				printf('<input type="submit" value="%s" name="Submit">', $lang['change password']);
				echo '</form>' . "\n";
			
			} else if ($prava_lidi >= 3) {
				echo '<form name="clovek_access_add" action="./?page=clovek&akce=uprav.commit.access.add" method="post">' . "\n";
				form_hidden('clovek_ID',$_GET['id']);
				form_textbox($lang['username'],'username',30,'',$lang['username_desc']);
				form_password($lang['password'],'pw',30,'',$lang['password_desc']);
				form_password($lang['password'],'pw2',30,'',$lang['password_desc2']);
				printf('<input type="submit" value="%s" name="Submit">', $lang['add']);
				echo '</form>' . "\n";	
			}
		}
	break;

	case 'uprav.commit':
	case 'uprav.commit.kontakty':
	case 'uprav.commit.delete':
	case 'uprav.commit.access.edit':
	case 'uprav.commit.access.add':
	case 'uprav.commit.access.pw':		
	case 'uprav.commit.access.delete':	
		if (($prava_lidi < 1) && ($_SESSION['user_clovek_ID'] != $_POST['clovek_ID'])) {
			// kick out people with access level lower than 1, unless the are editing themselves
			set_title($lang['access denied']);
			pg_head();
			print_one_message($lang['access denied']);
			break;
		}
		$result = mysql_query(sprintf('select * from clovek where clovek_ID = %s',$_POST['clovek_ID']));
		if (! $result2 = mysql_fetch_array($result)) {
			// no found person, go out
			set_title($lang['no records']);
			pg_head();
			print_one_message($lang['no records']);
			break;
		}
		if (($prava_lidi == 1) && ($_SESSION['user_klub_ID'] != $result2['klub_ID'])) {
			// people with access level 1 can edit only member of their club
			set_title($lang['access denied']);
			pg_head();
			print_one_message($lang['access denied']);
			break;
		}
		set_title(sprintf("%s %s",$lang['edit'],join_name($result2['jmeno'],$result2['nick'],$result2['prijmeni'])));
		
		switch ($_GET["akce"]) {
			
		case 'uprav.commit.access.add':
			if ($prava_lidi >= 3) {
				if ($_POST['pw'] == $_POST['pw2']) {
					$query = sprintF('insert into login (clovek_ID, username, password) values (%s,%s,%s)',
						get_numeric_field($_POST['clovek_ID']),
						get_text_field($_POST['username']),
						get_text_field(md5($_POST['pw']))
						);
					if (mysql_query($query)) {
						body_message($lang['edit ok']);
						header_redirect(sprintf('./?page=clovek&akce=uprav&id=%s',$_POST['clovek_ID']));
					} else {
						body_message(sprintf('%s<br>%s',$lang['edit failed'],mysql_error()));
					}
				} else {
					body_message($lang['passwords do not match']);
				}
			} else {
				body_message($lang['access denied']);
			}
		break;

		case 'uprav.commit.access.delete':
			if ($prava_lidi >= 3) {
				if ($_POST['confirm'] == 'yes') {
					$query = sprintf('delete from login where clovek_ID = %s', $_POST['clovek_ID']);
					if (mysql_query($query)) {
						body_message($lang['delete ok']);
						header_redirect(sprintf('./?page=clovek&akce=uprav&id=%s',$_POST['clovek_ID']));
					} else {
						body_message(sprintf('%s<br>%s',$lang['delete failed'], mysql_error()));
					}
				} else {
					body_message($lang['no confirm']);
				}
			} else {
				body_message($lang['access denied']);
			}
		break;

		case 'uprav.commit.access.edit':
			if ($prava_lidi >= 3) {
				$query = 'update login set ';
				$query .= sprintf('username = %s ', get_text_field($_POST['username']));
				$query .= sprintf('where clovek_ID = %s', $_POST['clovek_ID']);

				$query2 = 'update clovek set ';
				$query2 .= sprintf('prava_lidi = %s, ', get_numeric_field($_POST['prava_lidi']));
				$query2 .= sprintf('prava_kluby = %s, ', get_numeric_field($_POST['prava_kluby']));
				$query2 .= sprintf('prava_souteze = %s, ', get_numeric_field($_POST['prava_souteze']));
				$query2 .= sprintf('prava_debaty = %s ', get_numeric_field($_POST['prava_debaty']));
				$query2 .= sprintf('where clovek_ID = %s', $_POST['clovek_ID']);

				if (mysql_query($query) && mysql_query($query2)) {
					body_message($lang['edit ok']);
					header_redirect(sprintf('./?page=clovek&akce=uprav&id=%s',$_POST['clovek_ID']));
				} else {
					body_message(sprintf('%s<br>%s',$lang['edit failed'],mysql_error()));
				}
			} else {
				body_message($lang['access denied']);
			}
		break;

		case 'uprav.commit.access.pw':
			if (($prava_lidi >= 3) || ($_POST['clovek_ID'] == $_SESSION['user_clovek_ID'])) {
				if ($_POST['pw'] == $_POST['pw2']) {
					$query = sprintf('update login set password = %s where clovek_ID = %s', get_text_field(md5($_POST['pw'])), $_POST['clovek_ID']);
					if (mysql_query($query)) {
						body_message($lang['password changed']);
						header_redirect(sprintf('./?page=clovek&akce=uprav&id=%s',$_POST['clovek_ID']));
					} else {
						body_message(sprintf('%s<br>%s',$lang['edit failed'],mysql_error()));
					}
				} else {
					body_message($lang['passwords do not match']);
				}
			} else {
				body_message($lang['access denied']);
			}
		break;	

		case 'uprav.commit.delete':
			function kill_da_man($da_man, $table)
			{
				global $lang;
				$kill_query = sprintf('delete from %s where clovek_ID = %s', $table, $da_man);
				if (mysql_query($kill_query)) {
					body_message(sprintf($lang['kill table ok'], $table));
				} else {
					body_message(sprintf($lang['kill table failed'] . '<br>%s', $table, mysql_error()));
				}
			}

			if ($_POST['confirm'] == 'yes') {
				kill_da_man($_POST['clovek_ID'],'clovek');
				kill_da_man($_POST['clovek_ID'],'kontakt');
				kill_da_man($_POST['clovek_ID'],'rozhodci');
				kill_da_man($_POST['clovek_ID'],'clovek_tym');
				kill_da_man($_POST['clovek_ID'],'clovek_klub');
				kill_da_man($_POST['clovek_ID'],'clovek_debata');
				kill_da_man($_POST['clovek_ID'],'clovek_debata_ibody');
				kill_da_man($_POST['clovek_ID'],'clovek_turnaj');
			} else {
				body_message($lang['no confirm']);
			}
		break;

		case 'uprav.commit':
			$query = 'update clovek set ';
		
			if ($prava_lidi > 0) {
				// normal user can't change his name
				$query .= sprintf('jmeno = %s, ', get_text_field($_POST['name']));
				$query .= sprintf('prijmeni = %s, ', get_text_field($_POST['surname']));
			}
			$query .= sprintf('nick = %s, ', get_text_field($_POST['nick']));
		
			$result = mysql_query('select klub_ID, kratky_nazev from klub order by kratky_nazev');

			if ($prava_lidi >= 2) {
				// only big boss can move people between clubs
				$query .= sprintf('klub_ID = %s, ', get_numeric_field($_POST['club']));
			}
		
			if (($prava_lidi >= 1) || (! $result2['narozen'])) {
				// i can only insert my birth date, changing needs higher rights
				$query .= sprintf('narozen = %s, ', get_text_field($_POST['born']));
			}
		
		
			if ($prava_lidi > 0) {
				// only editor can switch 'active debater' status. komentar has to be switched because of the comma
				$query .= sprintf('komentar = %s, ', get_text_field($_POST['comment']));
				$query .= sprintf('debater = %s ', get_numeric_field($_POST['debater']));
			} else {
				$query .= sprintf('komentar = %s ', get_text_field($_POST['comment']));
			}

			$query .= sprintf('where clovek_ID = %s',$_POST['clovek_ID']);

			if (mysql_query($query)) {
				body_message($lang['edit ok']);
				header_redirect(sprintf('./?page=clovek&id=%s',$_POST['clovek_ID']));
			} else {
				body_message(sprintf('%s<br>%s',$lang['edit failed'],mysql_error()));
			}
		break;
		
		case 'uprav.commit.kontakty':

			set_title(sprintf("%s %s",$lang['edit'],join_name($result2['jmeno'],$result2['nick'],$result2['prijmeni'])));

			if ($_POST['kontakt_ID'] == 'NEW') {
				// insert new one
				$query = sprintf('insert into kontakt (clovek_ID, viditelnost, druh, tx) values (%s, %s, %s, %s)',
					get_numeric_field($_POST['clovek_ID']),
					get_numeric_field($_POST['visibility']),
					get_text_field($_POST['type']),
					get_text_field($_POST['tx'])
				);
			} else {
				if ($_POST['type'] == 'DELETE') {
					// delete
					$query = sprintf('delete from kontakt where kontakt_ID = %s and clovek_ID = %s', $_POST['kontakt_ID'], $_POST['clovek_ID']);
				} else {
					// edit
					$query = sprintf('update kontakt set druh = %s, viditelnost = %s, tx = %s where kontakt_ID = %s and clovek_ID = %s',
						get_text_field($_POST['type']),
						get_numeric_field($_POST['visibility']),
						get_text_field($_POST['tx']),
					       	$_POST['kontakt_ID'], $_POST['clovek_ID']
					);
				}
			}
				
			if (mysql_query($query)) {
				body_message($lang['edit ok']);
				header_redirect(sprintf('./?page=clovek&akce=uprav&id=%s',$result2['clovek_ID']));
			} else {
				body_message(sprintf('%s<br>%s',$lang['edit failed'],mysql_error()));
			}
		break;
		}
		
		pg_head();
		print_body_messages();
		

	break;
	


	case 'pridej.commit':
		if ($prava_lidi < 1) {
			set_title($lang['access denied']);
			pg_head();
			print_one_message($lang['access denied']);
			break;
		}
		set_title($lang['add person']);
		
		$query = "insert into clovek (jmeno, prijmeni, nick, klub_ID, narozen, debater, komentar) values ";
		$query .= sprintf('(%s, %s, %s, %s, %s, %s, %s);',
			get_text_field($_POST['name']),
			get_text_field($_POST['surname']),
			get_text_field($_POST['nick']),
			($prava_lidi >= 2) ? get_numeric_field($_POST['club']) : $_SESSION['user_klub_ID'],
			get_text_field($_POST['narozen']),
			get_numeric_field($_POST['debater']),
			get_text_field($_POST['komentar'])
		);

		if (mysql_query($query)) {
			body_message($lang['add ok']);
			header_redirect(sprintf('./?page=clovek&id=%s',mysql_insert_id()));
		} else {
			body_message(sprintf('%s<br>%s',$lang['add failed'],mysql_error()));
		}
		
		pg_head();
		print_body_messages();


	break;

	case 'vsichni':
		// show list of people
		set_title($lang['people']);
		pg_head();

		if ($prava_lidi >= 1) {
			// if user can add new people, this is the page he can do it
			printf('<p><a href="./?page=clovek&akce=pridej">%s</a></p>' . "\n", $lang['add person']);
		}
		
		$query = sprintf('
			select
				clovek.clovek_ID as a_clovek_ID,
				clovek.jmeno as a_jmeno,
				clovek.nick as a_nick,
				clovek.prijmeni as a_prijmeni,
				clovek.klub_ID as a_klub_ID,
				tym.tym_ID as a_tym_ID,
				tym.nazev as a_tym,
				klub.kratky_nazev as a_klub,
				sum(clovek_debata_ibody.ibody) as a_ibody
			from 
				(((clovek
				left join klub on clovek.klub_ID <=> klub.klub_ID)
				left join clovek_tym on clovek.clovek_ID <=> clovek_tym.clovek_ID)
				left join tym on clovek_tym.tym_ID <=> tym.tym_ID)
				left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID
			where
				(clovek_tym.aktivni is null or clovek_tym.aktivni = 1)
				and (clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = %s)
			group by 
				clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, tym.tym_ID, tym.nazev, klub.kratky_nazev
			order by
				clovek.prijmeni, clovek.jmeno, clovek.nick
		',get_current_season());
		$result = mysql_query($query);

		printf('<p>%s: %s | <a href="./?page=clovek&akce=rozhodci">%s</a> | <a href="./?page=clovek&akce=debateri">%s</a></p>
<table class="visible">
<tr><th>%s</td><th>%s</th><th>%s</th><th>%s</th></tr>', $lang["list"], $lang["everyone"], $lang["judges"], $lang["debaters"], $lang["name"], $lang["club"], $lang["team"], $lang["ipoints"]);
		
		$num_rows = 0;
		if ($result2 = mysql_fetch_array($result)) {
			$id = $result2['a_clovek_ID'];
			$name = sprintf('<a href="./?page=clovek&id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni']));
			if ($result2['a_klub_ID']) { $klub = sprintf('<a href="./?page=klub&id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
			if ($result2['a_tym_ID']) { $tym = sprintf('<a href="./?page=tym&id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']); } else { $tym = ''; }
			$ibody = $result2['a_ibody'];
			while($result2 = mysql_fetch_array($result)) {
				if ($result2['a_clovek_ID'] == $id) {
					// second team for an already read person
					$tym .= sprintf(', <a href="./?page=tym&id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']);
				} else {
					printf('<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>',$name, $klub, $tym, $ibody);
					$num_rows++;

					$id = $result2['a_clovek_ID'];
					$name = sprintf('<a href="./?page=clovek&id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni']));
					if ($result2['a_klub_ID']) { $klub = sprintf('<a href="./?page=klub&id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
					if ($result2['a_tym_ID']) { $tym = sprintf('<a href="./?page=tym&id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']); } else { $tym = ''; }
					$ibody = $result2['a_ibody'];
				}
			}
			// send the last cached line
			printf('<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>',$name, $klub, $tym, $ibody);
			
			$num_rows++;
		} else {
			printf('<tr><td colspan="5">%s</td></tr>',$line_empty);
		}
		printf('<tr><td colspan="4">%s: %s</td></tr></table>', $lang["total"], $num_rows);
				
		break;
	
	case 'debateri':
		// show list of debaters
		set_title($lang['debaters']);
		pg_head();
		
		$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.klub_ID as a_klub_ID, tym.tym_ID as a_tym_ID, tym.nazev as a_tym, klub.kratky_nazev as a_klub, sum(clovek_debata_ibody.ibody) as a_ibody';
		$query .= ' from (((clovek left join klub on clovek.klub_ID <=> klub.klub_ID) left join clovek_tym on clovek.clovek_ID <=> clovek_tym.clovek_ID) left join tym on clovek_tym.tym_ID <=> tym.tym_ID) left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID';
		$query .= sprintf(' where (clovek_debata_ibody.role = \'debater\' or clovek_debata_ibody.role is null) and clovek.debater = 1 and (clovek_tym.aktivni is null or clovek_tym.aktivni = 1) and (clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = %s)', get_current_season());
		$query .= ' group by clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, tym.tym_ID, tym.nazev, klub.kratky_nazev';
		$query .= ' order by clovek.prijmeni, clovek.jmeno, clovek.nick';
		$result = mysql_query($query);

		echo $template->make('clovek_list_debaters_head');
		
		$num_rows = 0;
		if ($result2 = mysql_fetch_array($result)) {
			$id = $result2['a_clovek_ID'];
			$name = sprintf('<a href="./?page=clovek&id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni']));
			if ($result2['a_klub_ID']) { $klub = sprintf('<a href="./?page=klub&id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
			if ($result2['a_tym_ID']) { $tym = sprintf('<a href="./?page=tym&id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']); } else { $tym = ''; }
			$ibody = $result2['a_ibody'];
			while($result2 = mysql_fetch_array($result)) {
				if ($result2['a_clovek_ID'] == $id) {
					// second team for an already read person
					$tym .= sprintf(', <a href="./?page=tym&id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']);
				} else {
					$template->editvar('line_name',$name);
					$template->editvar('line_club',$klub);
					$template->editvar('line_team',$tym);
					$template->editvar('line_ipoints',$ibody);
					echo $template->make('clovek_list_all_line');
					$num_rows++;

					$id = $result2['a_clovek_ID'];
					$name = sprintf('<a href="./?page=clovek&id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni']));
					if ($result2['a_klub_ID']) { $klub = sprintf('<a href="./?page=klub&id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
					if ($result2['a_tym_ID']) { $tym = sprintf('<a href="./?page=tym&id=%s">%s</a>',$result2['a_tym_ID'],$result2['a_tym']); } else { $tym = ''; }
					$ibody = $result2['a_ibody'];
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
		pg_head();
		
		$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.klub_ID as a_klub_ID, rozhodci.jazyk as a_jazyk, rozhodci.misto as a_misto, klub.kratky_nazev as a_klub, sum(clovek_debata_ibody.ibody) as a_ibody';
		$query .= ' from ((clovek left join klub on clovek.klub_ID <=> klub.klub_ID) left join rozhodci on clovek.clovek_ID <=> rozhodci.clovek_ID) left join clovek_debata_ibody on clovek.clovek_ID <=> clovek_debata_ibody.clovek_ID';
		$query .= sprintf(' where (rozhodci.rocnik = %s) and (clovek_debata_ibody.role = \'rozhodci\' or clovek_debata_ibody.role is null) and(clovek_debata_ibody.rocnik is null or clovek_debata_ibody.rocnik = %s)',get_current_season(),get_current_season());
		$query .= ' group by clovek.clovek_ID, clovek.jmeno, clovek.nick, clovek.prijmeni, clovek.klub_ID, rozhodci.jazyk, klub.kratky_nazev';
		$query .= ' order by clovek.prijmeni, clovek.jmeno, clovek.nick';
		$result = mysql_query($query);
		echo mysql_error();

		echo $template->make('clovek_list_judges_head');
		
		$one_line = 0;
		while ($result2 = mysql_fetch_array($result)) {
			$one_line = 1;
			$template->editvar('line_name', sprintf('<a href="./?page=clovek&id=%s">%s</a>',$result2['a_clovek_ID'],join_name($result2['a_jmeno'],$result2['a_nick'],$result2['a_prijmeni'])));
			if ($result2['a_klub_ID']) { $klub = sprintf('<a href="./?page=klub&id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_klub']); } else { $klub = ''; }
			$template->editvar('line_club',$klub);
			$template->editvar('line_languages',$result2['a_jazyk']);
			$template->editvar('line_place',$result2['a_misto']);
			$template->editvar('line_ipoints', $result2['a_ibody']);
			echo $template->make('clovek_list_judges_line');
		}
		if ($one_line == 0) echo $template->make('clovek_list_judges_empty');
		$template->editvar('line_total',mysql_num_rows($result));
		echo $template->make('clovek_list_judges_tail');
	break;

	default:
		if ($_GET['id'] == "") {
			if (($_GET['id'] = $_SESSION['user_clovek_ID']) == "") {
				pg_head();
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

			pg_head();
			if (
					($prava_lidi > 1)
					|| (($prava_lidi == 1) && ($result2['klub_ID'] == $_SESSION['user_klub_ID']))
					|| ($_SESSION['user_clovek_ID'] == $_GET['id'])
			) {
				printf('<p><a href="./?page=clovek&akce=uprav&id=%s">%s</a></p>', $result2['clovek_ID'], $lang['edit']);
			}
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
					$template->editvar('line_club',sprintf('<a href="./?page=klub&id=%s">%s</a>', $result2['klub_ID'], $result3['nazev']));
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
				$template->editvar('line_club',sprintf('<a href="./?page=klub&id=%s">%s</a>', $result3['a_klub_ID'], $result3['a_nazev']));
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

			// teams
			// TODO

			// accreditations
			$query = sprintf('select rozhodci.rocnik as a_rocnik, rozhodci.jazyk as a_jazyk, rozhodci.misto as a_misto, sum(clovek_debata_ibody.ibody) as a_ibody from rozhodci left join clovek_debata_ibody on rozhodci.rocnik = clovek_debata_ibody.rocnik and rozhodci.clovek_ID = clovek_debata_ibody.clovek_ID where (clovek_debata_ibody.role = \'rozhodci\' or clovek_debata_ibody.role is null) and rozhodci.clovek_ID = %s group by a_rocnik, a_misto, a_jazyk order by a_rocnik desc',$result2['clovek_ID']);
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
			$query = 'select debata.debata_ID as a_debata_ID, debata.datum as a_datum, clovek_debata.role as a_role, tym_aff.nazev as a_aff_nazev, tym_aff.tym_ID as a_aff_ID, tym_neg.nazev as a_neg_nazev, tym_neg.tym_ID as a_neg_ID, debata.vitez as a_vitez, sum(clovek_debata_ibody.ibody) as a_ibody';
			$query .= ' from clovek_debata left join debata using(debata_ID) left join debata_tym as dt_aff on debata.debata_ID = dt_aff.debata_ID left join tym as tym_aff on dt_aff.tym_ID = tym_aff.tym_ID left join debata_tym as dt_neg on debata.debata_ID = dt_neg.debata_ID left join tym as tym_neg on dt_neg.tym_ID = tym_neg.tym_ID left join clovek_debata_ibody on clovek_debata.clovek_ID = clovek_debata_ibody.clovek_ID and debata.debata_ID = clovek_debata_ibody.debata_ID';
			$query .= sprintf(' where (clovek_debata_ibody.role = \'debater\' or clovek_debata_ibody.role is null) and clovek_debata.clovek_ID = %s and dt_aff.pozice = 1 and dt_neg.pozice = 0',$result2['clovek_ID']);
			$query .= ' group by a_debata_ID, a_datum, a_role, a_aff_nazev, a_aff_ID, a_neg_nazev, a_neg_ID, a_vitez';
			$query .= ' order by a_datum desc';
			$result = mysql_query($query);

			if (mysql_num_rows($result) > 0) {
				echo $template->make('clovek_detail_debaty_head');
			}
			while ($result3 = mysql_fetch_array($result)) {
				$template->editvar('line_date',sprintf('<a href="debata.php?id=%s">%s</a>',$result3['a_debata_ID'],format_date($result3['a_datum'])));
				$template->editvar('line_affirmative', sprintf('<a href="./?page=tym&id=%s">%s</a>',$result3['a_aff_ID'],$result3['a_aff_nazev']));
				$template->editvar('line_negative', sprintf('<a href="./?page=tym&id=%s">%s</a>',$result3['a_neg_ID'],$result3['a_neg_nazev']));
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

			pg_head();
			print_body_messages();
		}	
		// end of
	break;
}
?>
