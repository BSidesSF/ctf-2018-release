Go to the site and log in; you'll get a cookie set such as:

    auth=e4d689c098b13d8da9a211417dba116a|username=guest&date=2017-01-28T17:12:33-0800&secret_length=5&

Note the 5-byte secret - you know what's being hashed, and how, so it's simply
a matter of bruteforcing the 5-character secret and forging your own cookie.
