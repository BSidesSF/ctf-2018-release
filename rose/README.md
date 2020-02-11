# _"A Rose By Any Other Name"_

## The Challenge
Inform CTF players that the following service is running on the internal network:
`http://secretshost.internal/`

Inform players HTML for this internal service is the following:
```html
<!DOCTYPE html>
<html>
	<head>
		<title>Please wait while you are redirected...</title>
	</head>
	<body onload="redirect_user()">
		<h1>Please wait while you are redirected!</h1>
		<!--
		The key is stored at /key.txt

		Send the admin a link, use XSS to get the key!

		Please note the admin uses only the Chrome web browser.
		-->
<script>
// For inspiration https://www.youtube.com/watch?v=zeIjmvZZ_SQ
function redirect_user() {
	var redirect_url = window.location.hash.substr(1);
	// Fixed that stupid XSS hackers! Suck it!
	if( !redirect_url.startsWith( "javascript" ) && redirect_url.indexOf( "https://www.google.com/" ) !== -1 && /^[a-zA-Z0-9\.\:/\=]+$/.test( redirect_url ) ) {
		window.location = redirect_url;
	} else {
		alert( "No." );
		window.location = "https://lmgtfy.com/?q=how+do+i+hack+gibson";
	}
}
</script>
	</body>
</html>
```

## The Service
Players can submit their exploit attempts via a URL submission form. This service can be found at `http://<HOST>:3000/`. The players must craft an exploit which bypasses the newly-added XSS mitigations. Using this exploit, players must steal the key located at `/key.txt` and exfiltrate it.

The service will automatically spawn a new Chrome instance to visit all URLs that have been submitted. This is a headless Chrome with all the regular web protections you'd expect (SOP enabled, etc).

## The Solution
The solution for this challenge can be found in `solution-exploit.html`. This exploit must be hosted on a server and then the URL of the exploit submitted via the form.