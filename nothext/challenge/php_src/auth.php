<?php
  define('SECRET', '5ba26');
  define('FLAG', 'FLAG:914240af8d93');

  $accts = array(
    'administrator' => 'bmLhVHjius',
    'guest'         => 'guest',
  );

  function is_valid($username, $password)
  {
    global $accts;

    return array_key_exists($username, $accts) && $accts[$username] === $password;
  }
?>
