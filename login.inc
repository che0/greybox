<?php

$GLOBALS["page"]->login_block = $lang['on login page'];

if ($_GET['von'] != '') {
	$login_von = htmlspecialchars(urldecode($_GET['von']));
} else {
	$login_von = "./";
}

if ($_POST['action'] == 'login') {
	
	if (
		cpdb_fetch_one_row(
			"select clovek.* from login left join clovek using (clovek_ID)
			where login.username = :username and login.password = md5(:password)",
			array(":username"=>$_POST['username'],":password"=>$_POST['password']),
			$result2)
	) {
		pg_achtung($lang['login ok']);
		$GLOBALS["page"]->headers[] = '<meta http-equiv="refresh" content="1;url=' . htmlspecialchars(urldecode($_POST['von'])) . '">';

		// now set all the session variables
		$_SESSION['is_logged_in'] = true;
		$_SESSION['user_name'] = join_name($result2['jmeno'],$result2['nick'],$result2['prijmeni']);
		$_SESSION['user_clovek_ID'] = $result2['clovek_ID'];
		$_SESSION['user_klub_ID'] = $result2['klub_ID'];
		$_SESSION['user_prava_lidi'] = $result2['prava_lidi'];
		$_SESSION['user_prava_kluby'] = $result2['prava_kluby'];
		$_SESSION['user_prava_souteze'] = $result2['prava_souteze'];
		$_SESSION['user_prava_debaty'] = $result2['prava_debaty'];
		$_SESSION['user_prava_teze'] = $result2['prava_souteze'] | $result2['prava_debaty'];
	} else {
		pg_achtung($lang['access denied']);
		$login_default_name = $_POST['username'];
	}
}

pg_head();
pg_flush_achtungs();

if (! $_SESSION['is_logged_in']) printf('
<p><form id="loginform" action="./?page=login" method="post">
%s: <input type="text" name="username" value="%s"><br>
%s: <input type="password" name="password"><br>
<input type="hidden" name="action" value="login">
<input type="hidden" name="von" value="%s">
<input type="submit" value="ok">
</form></p>',$lang['username'], $login_default_name, $lang['password'], $login_von);

?>
