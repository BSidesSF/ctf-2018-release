const auth = require('../../../config/auth.js');
const jwt = require('jsonwebtoken');

const User = require('../models').User;
const mail = require('../lib/mail');

module.exports = function(app, passport) {
  function generate_login_token(user_id) {
    console.log('issuing login jwt token with id = ' + user_id + '...');
    return jwt.sign(
      { id: user_id, type: 'login' },
      auth.jwt.secret,
      { expiresIn: auth.jwt.expiresIn }
    )
  }

  function generate_reset_token(user_id) {
    console.log('issuing reset jwt token with id = ' + user_id + '...');
    return jwt.sign(
      { id: user_id, type: 'reset' },
      auth.jwt.secret,
      { expiresIn: auth.jwt.expiresIn }
    )
  }

  app.get('/api/auth/forgot', function(req, res, next) {
    var email = req.query.email;

    User.findOne({
      where: { email: email}
    }).then(function(user) {
      if(!user) {
        console.error("No such user: " + email);
        return res.status(200).send({error: "Unknown email address: " + email});
      }

      // Since we're using a signed token, we can generate and forget
      var token = generate_reset_token(user.name);
      var reset_msg = auth.reset.msg.replace('#token#', token);

      mail.sendMail(
        user.email, // To:
        "Password reset!", // Subject
        reset_msg, // Body
        function(err) {
          if(err) {
            console.error("Error sending token: " + err);
            return res.status(200).send({error: "Error sending token!"});
          }
          return res.status(200).send({message: "Mail sent, check your email!"});
        }
      );
    }).catch(function(err) {
      console.error("Error finding user for reset: " + err);
      return next(null, false, { message: 'Error!' });
    });
  });

  // Handle confirmed forgot-password
  app.post('/api/auth/forgot', function(req, res, next) {
    passport.authenticate('forgot', function(err, user, info) {
      console.log("In /api/auth/forgot POST route");

      // Check if there was an error somewhere
      if(err) {
        console.log("Entered /api/auth/forgot with an error: " + err);
        return res.status(401).send({
          error: "Error resetting password!",
        });
      }

      // If the user failed to load, that means the reset failed
      if(!user) {
        console.error('Failed to reset password: ' + info['message']);

        return res.status(401).send({
          error: "Couldn't reset password: " + info['message'],
        });
      }

      // Since everything is good, issue a new token!
      console.log("Issuing new token!");
      return res.status(200).send({
        access_token: generate_login_token(user.id)
      });
    })(req, res, next);
  });

  // Handle local signup.
  app.post('/api/auth/signup', function(req, res, next) {
    passport.authenticate('local-signup', function(err, user, info) {
      console.log("In /api/auth/signup route");
      if(err) {
        console.log("err: " + err);
        return res.status(401).send({
          error: "Error signing up!",
        });
      }
      if(!user) {
        console.error('Failed to create user: ' + info['message']);

        return res.status(401).send({
          error: "Couldn't create user: " + info['message'],
        });
      }

      return res.status(200).send({
        access_token: generate_login_token(user.id)
      });
    })(req, res, next);
  });

  // Handle local signin.
  app.post('/api/auth/signin', function(req, res, next) {
    passport.authenticate('local-signin', function(err, user, info) {
      if(err) {
        console.log("err: " + err);
        return res.status(401).send({
          error: "Error signing in!",
        });
      }
      if(!user) {
        console.error(info['message']);

        /* Always return a generic response. */
        return res.status(401).send({
          error: "Login failed!",
        });
      }

      return res.status(200).send({
        access_token: generate_login_token(user.id)
      });
    })(req, res, next);
  });

  // All other requests
  app.use(function(req, res, next) {
    passport.authenticate('bearer', function(err, user, info) {
      if(err) {
        console.log("err: " + err);
        return res.status(401).send({
          error: "Bad token!",
        });
      }
      if(!user) {
        /* Always return a generic response. */
        console.log("User isn't authenticated");
        return res.status(401).send({
          error: "Bad token!",
        });
      }

      // Save the user so it can be used elsewhere
      req.user = user;

      return next(null);
    })(req, res, next);
  });
};

