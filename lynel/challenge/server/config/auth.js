module.exports = {
  'jwt': {
    'secret': process.env.JWT_SECRET,
    'expiresIn': process.env.JWT_EXPIRY,
  },

  'email': {
    'username':  process.env.EMAIL_USERNAME,
    'password':  process.env.EMAIL_PASSWORD,
    'from':      process.env.EMAIL_FROM,
  },

  'reset': {
    'msg': process.env.RESET_MSG,
  }
};
