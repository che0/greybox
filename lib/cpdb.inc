<?php

function cpdb_exec($query, $args = array()) {
	$repl_from = $repl_to = array();
	foreach ($args as $key => $value) {
		$repl_from[] = $key;
		$repl_to[] = "'" . addslashes($value) . "'";
	}
	return mysql_query(str_replace($str_from, $str_to, $query));
}

function cpdb_fetch_one_value ($query, $args = array()) {
	$res = cpdb_exec($query, $args);
	if ($res) {
		if ($row = mysql_fetch_row($res)) {
			return $row[0];
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}	
}

function cpdb_fetch_all ($query, $args, & $result) {
	$result = array();
	$numres = 0;
	
	$my_res = cpdb_exec($query, $args);
	if ($my_res) {
		while ($result[] = mysql_fetch_array($my_res)) {
			$numres++;
		}
	}
	
	return $numres;
}

function cpdb_transaction() {
	return mysql_query("start transaction");
}

function cpdb_commit() {
	return mysql_query("commit");
}

function cpdb_rollback() {
	return mysql_query("rollback");
}	

?>
