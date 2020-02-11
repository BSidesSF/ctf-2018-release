import Service from '@ember/service';
import RSVP from 'rsvp';
import { inject } from '@ember/service';

export default Service.extend({
  // Note: be careful with injections.. since most other services use
  // currentUser in some way, it's important to avoid circular dependencies!
  session: inject(),
  store: inject(),
  notifications: inject('notemsg'),
  ajax: inject('ajax'),

  // This is the all-important user object, which has the name and stats of the
  // currently authenticated user
  user: null,

  init: function() {
    this._super(...arguments);
  },

  load: function() {
    const self = this;

    if (this.get('session.isAuthenticated')) {
      return this.get('store').queryRecord('user', {}).then((user) => {
        self.set('user', user);
      });
    } else {
      return RSVP.resolve(null);
    }
  },

  logout: function() {
    this.get('session').invalidate();
  },
});
