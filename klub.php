<?php
require('inc/head.php');
$template->load('klub');
$template->editvar('clubs',$lang['clubs']);
$template->editvar('club',$lang['club']);
$template->editvar('place',$lang['place']);
$template->editvar('debaters',$lang['debaters']);
$template->editvar('members',$lang['members']);

switch ($_GET['akce']) {
	// akce rozdeleni
	case 'uprav':
	case 'uprav.tym.pridej':
	case 'uprav.commit':
		$got_header = 0;
		if ( ($prava_kluby >=2) || (($prava_kluby == 1) && ($_SESSION['user_klub_ID'] == $_GET['id'])) ) {
			$query = sprintf('select * from klub where klub_ID = %s', $_GET['id']);
			$result = mysql_query($query);
			
			if ($result2 = mysql_fetch_array($result)) {
				set_title($result2['nazev']);
				switch ($_GET['akce']) {
					case 'uprav':
						echo $template->make('head'); $got_header = 1;
						printf('<form name="klub_uprav" method="post" action="klub.php?akce=uprav.commit&id=%s">' . "\n", $_GET['id']);
						form_textbox($lang['club name'],'nazev', 40, $result2['nazev'], $lang['club name_desc']);
						form_textbox($lang['short name'],'kratky_nazev', 30, $result2['kratky_nazev'], $lang['short name_desc']);
						form_textbox($lang['place'],'misto', 30, $result2['misto'], $lang['club place_desc']);
						form_textarea($lang['comment'],'komentar', $result2['komentar'], $lang['comment_desc']);
						form_submit($lang['submit']);
						echo '</form>';

						if ($prava_kluby >=2) {
							printf('<h2>%s</h2>' . "\n", $lang['delete']);
							// TODO
							body_message('not implemented yet');
						}
					break;

					case 'uprav.commit':
						$query = sprintf('update klub set nazev = %s, kratky_nazev = %s, misto = %s, komentar = %s where klub_ID = %s',
							get_text_field($_POST['nazev']),
							get_text_field($_POST['kratky_nazev']),
							get_text_field($_POST['misto']),
							get_text_field($_POST['komentar']),
							$_GET['id']
						);
						
						if (mysql_query($query)) {
							body_message($lang['edit ok']);
							header_redirect(sprintf('klub.php?id=%s',$_GET['id']));
						} else {
							body_message(sprintf('%s<br>%s',$lang['edit failed'],mysql_error()));
						}
					break;

					case 'uprav.tym.pridej':
						// TODO
						body_message('not implemented yet');
					break;
				}
			} else {
				set_title($lang['no records']);
				body_message($lang['no records']);
			}
		} else {
			set_title($lang['access denied']);
			body_message($lang['access denied']);
		}
		if (! $got_header) echo $template->make('head');
		print_body_messages();
	break;

	case 'pridej':
		// add club
		if ($prava_kluby >= 2) {
			set_title($lang['add club']);
			echo $template->make('head');

			echo '<form name="klub_pridej" method="post" action="klub.php?akce=pridej.commit">' . "\n";
			form_textbox($lang['club name'],'nazev', 40, '', $lang['club name_desc']);
			form_textbox($lang['short name'],'kratky_nazev', 30, '', $lang['short name_desc']);
			form_textbox($lang['place'],'misto', 30, '', $lang['club place_desc']);
			form_textarea($lang['comment'],'komentar', '', $lang['comment_desc']);
			form_submit($lang['submit']);
			echo '</form>';
		} else {
			set_title($lang['access denied']);
			echo $template->make('head');
			print_one_message($lang['access denied']);
		}
	break;

	case "pridej.commit":
		if ($prava_kluby >= 2) {
			set_title($lang['add club']);
			$query = sprintf('insert into klub (nazev, kratky_nazev, misto, komentar) values (%s, %s, %s, %s)',
				get_text_field($_POST['nazev']),
				get_text_field($_POST['kratky_nazev']),
				get_text_field($_POST['misto']),
				get_text_field($_POST['komentar'])
			);
			if (mysql_query($query)) {
				body_message($lang['add ok']);
				header_redirect(sprintf('klub.php?id=%s',mysql_insert_id()));
			} else {
				body_message(sprintf('%s<br>%s',$lang['add failed'],mysql_error()));
			}	
		} else {
			set_title($lang['access denied']);
			body_message($lang['access denied']);
		}
		echo $template->make('head');
		print_body_messages();
	break;

	default:
		if ($_GET['id']) {
			// print out one club
			$query = sprintf('select nazev, kratky_nazev, misto, komentar from klub where klub_ID = %s',$_GET['id']);
			$result = mysql_query($query);
			if ($result2 = mysql_fetch_array($result)) {
				$can_edit = (($prava_kluby >= 2) || (($prava_kluby == 1) && ($_GET['id'] == $_SESSION['user_klub_ID'])));

				set_title($result2['nazev']);
				echo $template->make('head');
				printf('<p>(%s) %s</p>'."\n",$result2['kratky_nazev'],$result2['misto']);

			       	if ($can_edit) {
					printf('<p><a href="klub.php?akce=uprav&id=%s">%s</a></p>',$_GET['id'],$lang['edit']);
				}
				print_comment($result2['komentar']);

				// teams
				$query = 'select tym.tym_ID as a_tym_ID, tym.nazev as a_nazev, count(clovek_tym.clovek_ID) as a_clenove';
				$query .= ' from tym left join clovek_tym using (tym_ID)';
				$query .= ' where clovek_tym.aktivni = 1 or clovek_tym.aktivni is null';
				$query .= ' group by a_tym_ID, a_nazev';
				$query .= ' order by a_nazev';

				$result = mysql_query($query);

				$got_header = 0;
				if ($can_edit) {
					$got_header = 1;
					printf('<h2>%s</h2>',$lang['teams']);
					printf('<p><a href="klub.php?akce=uprav.pridej.tym&id=%s">%s</a></p>',$_GET['id'],$lang['add team']);
				}

				if (mysql_num_rows($result)) {
					if (! $got_header) printf('<h2>%s</h2>',$lang['teams']);
					printf('<table class="visible">');
					printf('<tr><th>%s</th><th>%s</th></tr>',$lang['team'],$lang['members']);

					while ($result3 = mysql_fetch_array($result)) {
						printf('<tr><td><a href="tym.php?id=%s">%s</td><td>%s</td></tr>'."\n",
							$result3['a_tym_ID'],
							$result3['a_nazev'],
							$result3['a_clenove']
						);
					}

					printf('</table>' . "\n");
				}

				// members
				printf('<h2>%s</h2>',$lang['members']);
				if ($can_edit) {
					printf('<p><a href="clovek.php?akce=pridej&klub=%s">%s</a></p>' . "\n", $_GET['id'] ,$lang['add person']);
				}
				$query = 'select clovek.clovek_ID as a_clovek_ID, clovek.jmeno as a_jmeno, clovek.nick as a_nick, clovek.prijmeni as a_prijmeni, clovek.debater as a_debater, tym.tym_ID as a_tym_ID, tym.nazev as a_tym';
				$query .= ' from clovek left join clovek_tym on clovek.clovek_ID = clovek_tym.clovek_ID left join tym on clovek_tym.tym_ID <=> tym.tym_ID';
				$query .= sprintf(' where clovek.klub_ID = %s',$_GET['id']);
				$query .= ' order by a_prijmeni, a_jmeno, a_nick';

				echo '<table class="visible">';
				printf ('<tr><th>%s</th><th>%s</th><th>%s</th></tr>' . "\n", $lang['name'], $lang['team'], $lang['active debater_short']);

				$result = mysql_query($query);

				if ($result3 = mysql_fetch_array($result)) {
					// first result fetch
					$r3_clovek_ID = $result3['a_clovek_ID'];
					$r3_name = sprintf('<a href="clovek.php?id=%s">%s</a>', $result3['a_clovek_ID'], join_name($result3['a_jmeno'], $result3['a_nick'], $result3['a_prijmeni']));
					$r3_team = sprintf('<a href="tym.php?id=%s">%s</a>', $result3['a_tym_ID'], $result3['a_tym']);
					$r3_debater = $result3['a_debater'] ? $lang['active'] : $lang['no'];

					while ($result3 = mysql_fetch_array($result)) {
						// check for added club, either add club, or print & fetch
						if ($result3['a_clovek_ID'] == $r3_clovek_ID) {
							$r3_team .= sprintf(', <a href="tym.php?id=%s">%s</a>', $result3['a_tym_ID'], $result3['a_tym']);
						} else {
							printf('<tr><td>%s</td><td>%s</td><td>%s</td>', $r3_name, $r3_team, $r3_debater);
							$r3_clovek_ID = $result3['a_clovek_ID'];
							$r3_name = sprintf('<a href="clovek.php?id=%s">%s</a>', $result3['a_clovek_ID'], join_name($result3['a_jmeno'], $result3['a_nick'], $result3['a_prijmeni']));
							$r3_team = sprintf('<a href="tym.php?id=%s">%s</a>', $result3['a_tym_ID'], $result3['a_tym']);
							$r3_debater = $result3['a_debater'] ? $lang['active'] : $lang['no'];
						}
					}
					// print last one
					printf('<tr><td>%s</td><td>%s</td><td>%s</td>', $r3_name, $r3_team, $r3_debater);
				} else {
					printf('<tr><td colspan="3">%s</td></tr>',$lang['no records']);
				}

				echo '</table>' . "\n";

				// todo
				// judges
				// coaches
				


				
			} else {
				set_title($lang['no records']);
				echo $template->make('head');
				print_one_message($lang['no records']);
			}
		} else {
			// print out club list
			set_title($lang['clubs']);
			echo $template->make('head');

			if ($prava_kluby >= 2) {
				printf('<p><a href="klub.php?akce=pridej">%s</a></p>',$lang['add club']);
			}
			echo $template->make('club_list_head');
			$query = 'select klub.klub_ID as a_klub_ID, klub.nazev as a_nazev, klub.misto as a_misto, count(clovek.clovek_ID) as a_debateri';
			$query .= ' from klub left join clovek using (klub_ID)';
			$query .= ' where clovek.debater = 1 or clovek.debater is null';
			$query .= ' group by a_klub_ID, a_nazev, a_misto';
			$query .= ' order by a_nazev';

			$result = mysql_query($query);
			while ($result2 = mysql_fetch_array($result)) {
				$template->editvar('line_club',sprintf('<a href="klub.php?id=%s">%s</a>',$result2['a_klub_ID'],$result2['a_nazev']));
				$template->editvar('line_place',$result2['a_misto']);
				$template->editvar('line_debaters',$result2['a_debateri']);
				echo $template->make('club_list_line');
			}

			echo $template->make('club_list_tail');

		}
	break;
}


echo $template->make('tail');

require('inc/tail.php');
?>
