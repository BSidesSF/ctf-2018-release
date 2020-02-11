const nodemailer = require('nodemailer');

var auth = require('../../../config/auth.js');

module.exports = {
  sendMail: function(to, subject, text, next) {
    const username = encodeURIComponent(auth.email.username);
    const password = encodeURIComponent(auth.email.password);

    var connectionString = 'smtps://' + username + ':' + password + '@smtp.gmail.com:465';
    var smtpTransport = nodemailer.createTransport(connectionString);

    smtpTransport.sendMail({to:to, from:auth.email.from, subject:subject, text: text}, function(err, info) {
      if (err) {
        next(err);
      }
      next(null);
    });
  }
};
