var request = require('request');
var LocalStrategy = require('passport-local').Strategy;
var BearerStrategy = require('passport-http-bearer').Strategy;
var CustomStrategy = require('passport-custom').Strategy;
var bcrypt = require('bcrypt-nodejs');
var jwt = require('jsonwebtoken');
var Sequelize = require('sequelize');

var auth = require('./auth');
const User = require('../src/app/models').User;

module.exports = function(passport) {
  passport.serializeUser(function(user, done) {
    done(null, user.id);
  });
  passport.deserializeUser(function(id, done) {
    done(null, user.findById(id));
  });

  passport.use('local-signup', new LocalStrategy({
    usernameField: 'username',
    passwordField: 'password',
    session: false,
    passReqToCallback: true
  },
    async function(req, email, password, done) {
      /* Check if the user already exists. */
      var old_user = await User.findOne({ where: { email: email} });
      if(old_user) {
        return done(null, false, { message: 'That email address is already registered!' });
      }

      user = User.create({
        name: email,
        email: email,
        password: bcrypt.hashSync(password),
      }).then(function(user) {
        return done(null, user);
      }).catch(function(err) {
        console.error("Error creating user: " + err);
        return done(null, false, { message: 'Error creating the user!' });
      });
    }
  ));

  passport.use('forgot', new CustomStrategy(
    function(req, done) {
      /* We use the username for the token. */
      jwt.verify(req.body.username, auth.jwt.secret, function(err, decoded) {
        if(err) {
          return done(null, false, { message: 'Error validating token: ' + err });
        }

        /* The token was good - attempt to load the user. */
        User.findOne({ where: { name: decoded.id} }).then(function(user) {
          if(user) {
            user.update({
              password: bcrypt.hashSync(req.body.password),
            });
            return done(null, user);
          } else {
            return done(null, false, { message: 'Error resetting password!' });
          }
        })
      });
    }
  ));

  passport.use('local-signin', new LocalStrategy({
    usernameField: 'username',
    passwordField: 'password',
    passReqToCallback: true
  },
    function(req, email, password, done) {
      User.findOne({
        where: {
          [Sequelize.Op.or]: [
            {email: email},
            {name:  email},
          ]

        }
      }).then(function(user) {
        if(!user) {
          return done(null, false, { message: 'Login failed.' });
        }

        bcrypt.compare(password, user.password, function(err, res) {
          if(res) {
            return done(null, user);
          } else {
            return done(null, false, { message: 'Login failed.' });
          }
        });
      });
    })
  );

  passport.use(new BearerStrategy(
    function(token, done) {
      jwt.verify(token, auth.jwt.secret, function(err, decoded) {
        if(err) {
          return done("Failed to verify the JWT token: " + err);
        }

        User.findById(decoded.id).then(function(user) {
          if(user) {
            return done(null, user);
          } else {
            return done("Couldn't find the user that matches the token!");
          }
        })
      });
    }
  ));
};
