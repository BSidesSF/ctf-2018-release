import { computed } from '@ember/object';
import Mixin from '@ember/object/mixin';
import $ from 'jquery';

export default Mixin.create({
  bad_email: computed('email', function() {
    const email = this.get('email');

    if(email == '') {
      return "Email is missing!";
    }

    if(!/\S+@\S+\.\S+/.test(email)) {
      return "Email doesn't look valid!";
    }

    return null;
  }),

  bad_password: computed('password', 'password_again', function() {
    const pw1 = this.get('password');
    const pw2 = this.get('password_again');

    if(!pw1) {
      return "Password shouldn't be blank!";
    }

    if(pw1.length < 8) {
      return "Passwords must be at least 8 characters";
    }

    if(pw1.match(/^[a-z]*$/)) {
      return "Passwords must contain an uppercase or symbol";
    }

    if(pw1 !== pw2) {
      return "Passwords don't match!";
    }

    return null;
  }),

  check_email: function() {
    const err = this.get('bad_email');

    if(err) {
      this.get('notifications').error(err);
      $('#email').focus();

      return false;
    }
    return true;
  },

  check_password: function() {
    const err = this.get('bad_password');

    if(err) {
      this.get('notifications').error(err);
      $('#password').focus();

      return false;
    }
    return true;
  },
});
