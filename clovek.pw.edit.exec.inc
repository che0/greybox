<?php

if($_REQUEST["n1"] != $_REQUEST["n2"]) {
	pg_achtung($lang["passwords do not match"]);
	return;
}

if($_REQUEST["n1"] == "" && !isset($_REQUEST["delete"])) {
	pg_achtung($lang["refusing to set empty password"]);
	return;

}

$login_id = $_REQUEST["login_id"];
$clovek_id = $_REQUEST["clovek_id"];
$username = $_REQUEST["username"];
$pw_n = md5($_REQUEST["n1"]);
$pw_o = md5($_REQUEST["old"]);


if (isset($_REQUEST["password"])) {
	// check user is changing his own pw
	if ($clovek_id != $_SESSION["user_clovek_ID"]) {
		pg_achtung($lang["access denied"]);
		return;
	}
	
	// check old password
	if (cpdb_fetch_one_value("select password from login where clovek_ID = :clovek_id and password = :password", array(":clovek_id"=>$clovek_id, ":password"=>$pw_o)) != $pw_o) {
		pg_achtung($lang["bad old password"]);
		return;
	}
	
	// change it
	if (cpdb_exec("update login set password = :password where clovek_ID = :clovek_id",array(":clovek_id"=>$clovek_id, ":password"=>$pw_n))) {
		pg_achtung($lang["password changed"]);
		include("clovek.inc");
	}
} else {
	// ensure user has approprite privileges
	if ($GLOBALS["cps_lidi"] < 3) {
		pg_achtung($lang["access denied"]);
		return;
	}
	
	if (isset($_REQUEST["add"])) {
		if (cpdb_fetch_one_value("select count(*) from login where username = :username",array(":username"=>$username)) > 0) {
			pg_achtung($lang["duplicate username"]);
			return;
		}
		
		// add account
		if (cpdb_exec("insert into login (clovek_ID, username, password) values (:clovek_id, :username, :password)", array(":clovek_id"=>$clovek_id, ":username"=>$username, ":password"=>$pw_n))) {
			pg_achtung($lang["account insert ok"]);
			include("clovek.inc");
		}
		
	} elseif (isset($_REQUEST["save"])) {
		if (cpdb_fetch_one_value("select count(*) from login where username = :username and login_ID != :login_id",array(":username"=>$username, ":login_id"=>$login_id)) > 0) {
			pg_achtung($lang["duplicate username"]);
			return;
		}
		
		// change account
		if (cpdb_exec("update login set username = :username,  password = :password where login_ID = :login_id and clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id, ":username"=>$username, ":password"=>$pw_n, ":login_id"=>$login_id))) {
			pg_achtung($lang["account update ok"]);
			include("clovek.inc");
		}
		
	} elseif (isset($_REQUEST["delete"])) {
		// delete account
		if (cpdb_exec("delete from login where login_ID = :login_id and clovek_ID = :clovek_id", array(":clovek_id"=>$clovek_id, ":login_id"=>$login_id))) {
			pg_achtung($lang["account delete ok"]);
			include("clovek.inc");
		}
		
	}
}

?>
