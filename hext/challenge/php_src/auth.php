<?php
  define('SECRET', '26cb1');
  define('FLAG', 'FLAG:795bd587e66329');

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
