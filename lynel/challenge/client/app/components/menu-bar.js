import { inject } from '@ember/service';
import Component from '@ember/component';

export default Component.extend({
  session: inject('session'),
  currentUser: inject('current-user'),

  init: function() {
    this._super(...arguments);
  },

  actions: {
    logout: function() {
      return this.get('currentUser').logout();
    },
  },
});
