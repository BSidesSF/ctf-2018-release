'use strict';

const bcrypt = require('bcrypt-nodejs');
const uuidv4 = require('uuid/v4');

const { API_EMAIL, API_USERNAME, API_PASSWORD } = process.env;

module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.bulkDelete('Users', { 'email': API_EMAIL })
    .then(() => {
      return queryInterface.bulkInsert('Users', [{
        id: uuidv4(),
        name: 'admin',
        email: 'ctf2018@skullsecurity.org',
        password: bcrypt.hashSync('738dc4d25ac0d3f1ba0aa1d57553dde1'),
        comment: 'FLAG:dc6cae34a63c',
        createdAt: new Date(),
        updatedAt: new Date(),
      }], {});
    });
  },

  down: (queryInterface, Sequelize) => {
    return queryInterface.bulkDelete('Users', { 'where': { 'email': 'ctf2018@skullsecurity.org' }});
  }
};
