<?php
  # Note: This has to end with a '/'
  define('DIR', '/tmp/images/');

  function get_file($checksum)
  {
    foreach(scandir(DIR) as $file) {
      if(strlen($file) < 32)
        continue;

      list($cached_checksum, $filename) = explode(':', $file, 2);

      if($checksum == $cached_checksum) {
        return [$file, file_get_contents(DIR . $file)];
      }
    }

    return [null, null];
  }

  function save_file($filename, $original_name)
  {
    $dest_name = basename($original_name);
    $dest_name = md5_file($filename) . ':' . preg_replace('/[^a-zA-Z0-9.]/', '', $original_name);
    $dest_name = DIR . $dest_name;
    rename($filename, $dest_name);

    printf("Saving file as " . htmlentities($dest_name));

    return $dest_name;
  }

  function verify_image($filename, $original_filename)
  {
    $image = null;
    if(preg_match('/\.png$/i', $original_filename)) {
      $image = imagecreatefrompng($filename);
    } elseif(preg_match('/\.gif$/i', $original_filename)) {
      $image = imagecreatefromgif($filename);
    } elseif(preg_match('/\.jpg$/i', $original_filename) ||
             preg_match('/\.jpeg/i', $original_filename)) {
      $image = imagecreatefromjpeg($filename);
    } else {
      die("Unknown file type: " . htmlentities($original_filename) . "\n");
    }

    if(!$image) {
      return false;
    }

    return true;
  }

  if(array_key_exists('file', $_FILES)) {
    $file = $_FILES['file'];

    if(!verify_image($file['tmp_name'], $file['name'])) {
      die("Not a valid image!\n");
    }

    list($result_filename, $result_contents) = get_file(md5_file($file['tmp_name']));

    if(!$result_filename) {
      $result_filename = save_file($file['tmp_name'], $file['name']);
      print "<p>File saved into cache! Send it again to get it back!</p>";
    } else {
      header("Content-Type: image/jpg");
      header("Content-Disposition: attachment; filename='$result_filename'");
      print(file_get_contents(DIR . $result_filename));
    }
  } else {
?>
  <h1>Welcome to Pre-Image, your friendly neighbourhood image-caching service!</h1>

  <p>Simply upload your image here, and if it's already in our collection, we'll return it to you. Easy as that!</p>

  <form action='index.php' method='post' enctype='multipart/form-data'>
  <p>Upload file: <input type='file' name='file' /></p>

  <p><input type='submit' value='Check cache!' /></p>
  </form>

  <h1>Current cache</h1>
  <pre style='font-size: 1.4em'><ul><?php
    foreach(scandir(DIR) as $file) {
      if($file == '.' || $file == '..') {
        continue;
      }
      if($file == "0e444267211677510999396671309349:flag.jpg") {
        print "<li><strong>" . htmlentities($file) . "</strong></li>";
      } else {
        print "<li>$file</li>";
      }
    }
  ?></ul></pre>
  <?php
}
?>
