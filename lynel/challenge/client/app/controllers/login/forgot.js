import { inject } from '@ember/service';
import Controller from '@ember/controller';
import Validator from '../../mixins/validator';

export default Controller.extend(Validator, {
  notifications: inject('notemsg'),

  queryParams: ['token'],

  actions: {
    // This handles the second (hard) part of the password reset
    forgot: function() {
      if(!this.check_password()) {
        return;
      }

      const password = this.get('password');
      const token = this.get('token');

      /* Pass in the token instead of the username. */
      this.get('session').authenticate('authenticator:forgot', token, password).then(() => {
        /* No actions are necessary here. */
      }, (err) => {
        this.get('notifications').error(err['error']);
      });
    },
  },
});
