import { Router } from 'express';
import { User } from '../models';

import bcrypt from 'bcrypt-nodejs';
import Sequelize from 'sequelize';

const userRouter = new Router();

userRouter.get('/current_user', function(req, res) {
  return res.status(200).send({
    user: {
      id: req.user.id,
      name: req.user.name,
      email: req.user.email,
      comment: req.user.comment,
    },
  });
});

// Edit the current user
userRouter.put('/current_user', async function(req, res) {
  const self = this;

  // Make sure the email address looks legit
  if(!/\S+@\S+\.\S+/.test(req.body.user.email)) {
    return res.status(200).send({
      error: "Please enter a valid email address!",
    });
  }

  if(!req.body.user.name) {
    return res.status(200).send({
      error: "Please enter a name!",
    });
  }

  var user = null;

  // Make sure the email address isn't already in use. This has a race
  // condition, in theory, but the database also enforces this constraint
  // (they'll just get a worse error)
  if(req.user.provider == 'local') {
    user = await User.findOne({
      where: {
        email: req.body.user.email,
        id: { [Sequelize.Op.ne]: req.user.id },
        /* Don't update the comment to prevent messing up the flag */
      }
    });
    if(user) {
      return res.status(200).send({
        error: "Somebody else is using that email address!",
      });
    }
  }

  const update = {
    name: req.body.user.name,
    email: req.body.user.email,
  }

  req.user.update(update).then(function() {
    return res.status(200).send({
      message: "Ok!",
    });
  }).catch(function(err) {
    return res.status(200).send({
      error: "Couldn't update: " + err,
    });
  });
});

userRouter.post('/changepw', function(req, res) {
  const oldpw = req.body.oldpw;
  const newpw = req.body.newpw;

  if(newpw === '') {
    return res.status(200).send({
      error: "Password can't be blank!"
    });
  }

  // Validate the old password
  bcrypt.compare(oldpw, req.user.password, function(err, status) {
    if(!status || err) {
      console.error("Password change failed: " + (err ? err.toString() : "Bad password"));
      return res.status(200).send({
        error: "Old password didn't match!",
      });
    }

    req.user.update({
      password: bcrypt.hashSync(newpw),
    }).then(function() {
      return res.status(200).send({
        message: "Password changed!"
      });
    });
  });
});

userRouter.get('/checkuser', async function(req, res) {
  const users = await User.findAll({
    attributes: ['id'],
    where: {
      'name': req.query.name,
    },
  })

  return res.status(200).send({available: users.length == 0});
});

export default userRouter;
