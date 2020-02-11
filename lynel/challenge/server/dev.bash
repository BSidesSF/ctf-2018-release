# Port
export API_PORT=3000
export API_HOST='0.0.0.0'

# This is for the bearer token we issue - the secret should be long, random,
# and secret
export JWT_SECRET='This is an example of a poorly chosen secret :)'
export JWT_EXPIRY='90 days'

# Email settings
export EMAIL_USERNAME='bsidessf.ctf.2018@gmail.com'
export EMAIL_PASSWORD='vmfrhkpgjvatjzct'
export EMAIL_FROM='bsidessf.ctf.2018@gmail.com'

# Password reset settings
export RESET_MSG="Hi, this is from your friendly password resetter!

Your password reset token is

#token#

Please enter that into the form at /login/forgot!

If you didn't request a reset, please ignore this message!"
