'use strict';

const crypto = require('crypto');
const { filter, map } = require('lodash');

module.exports = (sequelize, DataTypes) => {
  const Op = sequelize.Op;
  const User = sequelize.define('User', {
    'id': {
      primaryKey: true,
      type: DataTypes.UUID,
      defaultValue: DataTypes.UUIDV4
    },
    email: DataTypes.STRING,
    password: DataTypes.STRING,
    name: {
      type: DataTypes.STRING,
      unique: true,
    },
    comment: DataTypes.STRING,
  });

  return User;
};