<?php
/*
 * greybox
 * $Id: de_magic_quotes.inc,v 1.4 2005/01/09 00:47:38 che0 Exp $
 */

if (get_magic_quotes_gpc()) {
   function stripslashes_deep($value)
   {
       $value = is_array($value) ?
                   array_map('stripslashes_deep', $value) :
                   stripslashes($value);

       return $value;
   }

   $_REQUEST = array_map('stripslashes_deep', $_REQUEST);
   $_POST = array_map('stripslashes_deep', $_POST);
   $_GET = array_map('stripslashes_deep', $_GET);
   $_COOKIE = array_map('stripslashes_deep', $_COOKIE);
} 

?>
