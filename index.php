<?php
require('inc/head.php');
$template->load('index');
$template->editvar('teams',$lang['teams']);
$template->editvar('user',$lang['user']);

echo $template->make('head');

if ($lang['release notes']) print_one_message($lang['release notes']);

if ($_SESSION['is_logged_in']) {
	$template->editvar('index_name',sprintf('<a href="clovek.php">%s</a>',$_SESSION['user_name']));
	
	$query = sprintf('select nazev from klub where klub_ID = %s', $_SESSION['user_klub_ID']);
	$result = mysql_query($query);

	if ($result2 = mysql_fetch_array($result)) {
		$template->editvar('index_club',sprintf(', <a href="klub.php?id=%s">%s</a>',$_SESSION['user_klub_ID'], $result2['nazev']));
	} else {
		$template->editvar('index_club','');
	}

	$query = sprintf('select tym.tym_ID as a_tym_ID, tym.nazev as a_tym from clovek_tym left join tym using (tym_ID) where clovek_tym.clovek_ID = %s and clovek_tym.aktivni = 1', $_SESSION['user_clovek_ID']);
	$result = mysql_query($query);

	$teams = '';
	$num_teams = 0;
	while ($result2 = mysql_fetch_array($result)) {
		if ($num_teams > 0) {
			$teams .= ', ';
		}
		$num_teams++;
		$teams .= sprintf('<a href="tym.php?id=%s">%s</a>', $result2['a_tym_ID'], $result2['a_tym']);
	}
	if ($num_teams == 0) {
		$teams = $lang['no records'];
	}
	$template->editvar('index_teams',$teams);
	
	echo $template->make('index');	
}

printf('<div id="footnote">%s</div>', $lang['tail_footnote']);

echo $template->make('tail');

require('inc/tail.php');
?>
