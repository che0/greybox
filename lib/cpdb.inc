<?php
/*
 * greybox
 * $Id: cpdb.inc,v 1.13 2006/04/04 21:49:57 che0 Exp $
 */

function cpdb_achtung($achtung) {
	$achtung = "Database error: $achtung";
	if (isset($GLOBALS["page"])) {
		pg_achtung($achtung);
	} else {
		echo "<br />" . $achtung;
	}
}

function cpdb_bind($query, $args = array()) {
	$repl_from = $repl_to = array();
	foreach ($args as $key => $value) {
		$repl_from[] = $key;
		
		if (!is_int($value)) {
			if ($value == "" || is_null($value)) {
				$repl_to[] = "NULL";
			} else {
				$repl_to[] = "'" . mysql_real_escape_string($value) . "'";
			}
		} else {
			$repl_to[] = $value;
		}
	}
	
	// cpdb_achtung(str_replace($repl_from, $repl_to, $query));
	return str_replace($repl_from, $repl_to, $query);
}

function cpdb_fetch_one_value ($query, $args = array()) {
	$res = mysql_query(cpdb_bind($query, $args), $GLOBALS["dblink"]);
	if ($res) {
		if ($row = mysql_fetch_row($res)) {
			return $row[0];
		} else {
			return false;
		}
	} else {
		cpdb_achtung(mysql_error());
		return false;
	}	
}

function cpdb_fetch_all ($query, $args, & $result, $primary_key = "__") {
	$result = array();
	$numres = 0;
	
	$my_res = mysql_query(cpdb_bind($query, $args), $GLOBALS["dblink"]);
	if ($my_res) {
		while ($tres = mysql_fetch_array($my_res)) {
			if ($primary_key == "__") {
				// normal behaviour
				$result[] = $tres;
			} else {
				// create associative array of rows
				$result[$tres[$primary_key]] = $tres;
			}
			$numres++;
		}
	} else {
		cpdb_achtung(mysql_error());
	}
	
	return $numres;
}

function cpdb_fetch_one_row ($query, $args, & $row) {
	$my_res = mysql_query(cpdb_bind($query, $args), $GLOBALS["dblink"]);
	if (! $my_res) {
		cpdb_achtung(mysql_error());
		return false;
	}
	if ($row = mysql_fetch_array($my_res)) {
		if (mysql_fetch_array($my_res)) {
			cpdb_achtung("Internal database fetch problem: unexpected rows");
		}
		return true;
	} else {
		$row = NULL;
		return false;
	}
	
}

function cpdb_exec($query, $args) {
	$my_res = mysql_query(cpdb_bind($query, $args), $GLOBALS["dblink"]);
	if (!! $my_res) {
		return true;
	} else {
		cpdb_achtung(mysql_error());
		return false;
	}
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

function cpdb_insert_id() {
	return mysql_insert_id();
}

?>
