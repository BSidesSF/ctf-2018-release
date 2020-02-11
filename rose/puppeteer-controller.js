const puppeteer = require( "puppeteer" );
const express = require( "express" );
var bodyParser = require( "body-parser" );
var path = require( "path" );
var app = express();
var internal_app = express();

app.use(bodyParser.urlencoded({
	extended: true
})); 

function visit_url( target_url ) {
	var browser = false;
	return puppeteer.launch().then(function( _browser ) {
		browser = _browser;
	}).then(function() {
		return browser.newPage()
	}).then(function( new_page ) {
		console.log( "Navigating to URI..." );
		return new_page.goto( target_url );
	}).then(function( new_page_instance ) {
		return delay(
			( 1000 * 10 )
		);
	}).catch(function( exception ) {
		console.log( "An exception occured" );
		console.log( exception );
		browser.close();
	}).then(function() {
		console.log( "Closing browser..." );
		browser.close();
	})
}

function delay(t, v) {
   return new Promise(function(resolve) { 
       setTimeout(resolve.bind(null, v), t)
   });
}

var base_page = `
<!DOCTYPE html>
<html>
	<head>
		<title>Contact Us!</title>
	</head>
	<body>
		<center>
			<h1>Contact us!</h1>
			<p>
				Send us your website and we'll take a look.
			</p>
			<form action="/sendmessage" method="post">
				Website URL: <input type="text" placeholder="https://example.com" name="url" />
				<br />
				<input type="submit" value="Submit Website"/>
			</form>
		</center>
	</body>
</html>
`

app.get(
	"/",
	function( request, response ) {
		console.log( "Homepage visited!" );
		response.send(
			base_page
		)
	}
)

app.post(
	"/sendmessage",
	function( request, response ) {
		var input_url = request.body.url;
		input_url = input_url.toString();
		console.log( "Submitted URL: " + input_url );
		if( input_url.startsWith( "http://" ) || input_url.startsWith( "https://" ) ){
			visit_url(
				input_url
			).then(function() {
				response.send(
					"URL submitted!"
				)
			});
		} else {
			response.send(
				"Invalid URL!"
			);
		}
	}
)

// This is the external submit URL page
app.listen(
	3000,
	function() {
		console.log( "Vulnerable Admin is now running on port 3000!" );
	}
)

var internal_service_html = `
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
	if( !redirect_url.startsWith( "javascript" ) && redirect_url.indexOf( "https://www.google.com/" ) !== -1 && /^[a-zA-Z0-9\.\:/\=]+$/.test( redirect_url ) && redirect_url.length < 100 ) {
		window.location = redirect_url;
	} else {
		window.location = "about:blank";
	}
}
</script>
	</body>
</html>
`
var secret_key = "FLAG:HE_WHO_MUST_NOT_BE_NAMED";

internal_app.get(
	"/",
	function( request, response ) {
		console.log( "The internal service has been visited." );
		response.send(
			internal_service_html
		)
	}
);

internal_app.get(
	"/key.txt",
	function( request, response ) {
		response.send(
			secret_key
		)
	}
);

internal_app.listen(
	3001,
	function() {
		console.log( "The internal server has been started on port 3001 (unmapped in Docker)." );
	}
)