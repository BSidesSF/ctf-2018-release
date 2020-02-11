import { inject } from '@ember/service';
import Controller from '@ember/controller';
import Validator from '../../mixins/validator';
import AjaxService from 'ember-ajax/services/ajax';

export default Controller.extend(Validator, {
  notifications: inject('notemsg'),

  // By default, we sign in, not sign up
  action: 'signin',

  actions: {
    // Handle attempted signins
    signin: function() {
      const self = this;
      const email = this.get('email');
      const password = this.get('password');

      this.get('session').authenticate('authenticator:signin', email, password).then(() => {
        /* No actions are necessary here. */
      }, (err) => {
        self.get('notifications').error(err.error ? err.error : err);
      });
    },

    // Handle signing up for a new account
    signup: function() {
      const self = this;

      if(!this.check_email()) {
        return;
      }
      if(!this.check_password()) {
        return;
      }

      const email = this.get('email');
      const password = this.get('password');

      this.get('session').authenticate('authenticator:signup', email, password).then(() => {
        /* No actions are necessary here. */
      }, (err) => {
        self.get('notifications').error(err.error ? err.error : err);
      });
    },

    // Handle the first (easy) part of forgot-password
    forgot: function() {
      const email = this.get('email');
      const self = this;

      if(!this.check_email()) {
        return;
      }

      new AjaxService().request('/api/auth/forgot?email=' + encodeURIComponent(email)).then(function(response) {
        if(response.message) {
          self.get('notifications').success(response.message);
          self.transitionToRoute('/login/forgot');
        } else {
          self.get('notifications').error(response.error ? response.error : response);
        }
      });
    },

    goto_signup: function() {
      this.set('action', 'signup');
    },
    goto_forgot: function() {
      this.set('action', 'forgot');
    },
    goto_signin: function() {
      this.set('action', 'signin');
    },
  },
});
