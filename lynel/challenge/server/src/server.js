import http from 'http';
import https from 'https';
import path from 'path';

// Create the Express app
import express from 'express';
var app = express();

// Middleware to parse the body
import { json, urlencoded } from 'body-parser';
app.use(json({
  limit: '50mb',
  parameterLimit: 1000000
}));
app.use(urlencoded({
  extended: true,
  limit: '50mb',
  parameterLimit: 1000000
}));

// Console logging
import morgan from 'morgan';
app.use(morgan('dev'));

app.use(function(req, res, next) {
  res.set('Content-Type', 'text/plain');
  res.set('Expires', 'Mon, 25 Dec 2000 06:00:00 GMT');
  res.set('Last-Modified', new Date());
  res.set('Cache-Control', 'max-age=0, no-cache, must-revalidate, proxy-revalidate');
  res.set('Pragma', 'no-cache');
  next();
});

app.use('/', express.static(path.join(__dirname, '/public'), {
  setHeaders: function (res) {
    res.setHeader('Content-Type', 'text/html');
  }
}));

// Include and configure passport
import passport from 'passport';
require('../config/passport')(passport);
app.use(passport.initialize());

// Do authentication stuff - everything after this requires a valid
// bearer token
require('./app/routes/auth.js')(app, passport);
// XXX ------------------------------------------- XXX
// XXX ALL ROUTES NEED TO BE DEFINED BELOW THIS    XXX
// XXX (the previous line enforces authentication) XXX
// XXX ------------------------------------------- XXX

// Manage the user profile
import userRouter from './app/routes/user';
app.use('/api/user', userRouter);

// Install an error handler
import errorHandler from 'api-error-handler';
app.use(errorHandler());

http.createServer(app).listen(3000);
