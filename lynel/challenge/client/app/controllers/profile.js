import { inject } from '@ember/service';
import Controller from '@ember/controller';
import Validator from '../mixins/validator';
import { oneWay } from "@ember/object/computed"

export default Controller.extend(Validator, {
  notifications: inject('notemsg'),
  ajax: inject('ajax'),
  currentUser: inject('current-user'),

  // This binding lets us use our Validator without having to tell it to use
  // 'model.email' instead of 'email' - simply alias it
  email: oneWay('model.email'),

  actions: {

    save: function() {
      const self = this;

      if(!this.check_email()) {
        return;
      }

      if(this.get('bad_username')) {
        return;
      }

      this.get('ajax').put('/api/user/current_user', { data: {
        user: {
          name: this.get('model.name'),
          email: this.get('model.email'),
          comment: this.get('model.comment'),
        }
      }}).then(function(result) {
        if(result.error) {
          self.get('notifications').error("Error saving profile: " + result.error);
        } else {
          self.get('notifications').success("Saved!");
          self.get('currentUser').load().then(function() {
            self.transitionToRoute('index');
          });
        }
      });
    },

    change_password: function() {
      const self = this;

      if(!this.check_password()) {
        return;
      }

      return this.get('ajax').post('/api/user/changepw', {
        data: {
          oldpw: this.get('old_password'),
          newpw: this.get('password'),
        }
      }).then(function(response) {
        if(response.message) {
          self.get('notifications').success(response.message);
        } else {
          self.get('notifications').error(response.error);
        }
      });
    },

    check_username: function() {
      const self = this;
      const username = this.get('model.name');

      this.set('bad_username', null);

      if(username == '') {
        this.set('bad_username', "Username is required!");
      }

      // Let them use their own username
      if(username == this.get('currentUser.user.name')) {
        return null;
      }

      return this.get('ajax').request('/api/user/checkuser?name=' + encodeURIComponent(username)).then(function(response) {
        if(response.available) {
          self.set('bad_username', null);
        } else {
          self.set('bad_username', "Name already in use!");
        }
      });

    },
  }
});
