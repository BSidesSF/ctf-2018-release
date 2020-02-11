import { inject as service } from '@ember/service';
import Controller from '@ember/controller';
import { computed } from '@ember/object';

export default Controller.extend({
  session: service('session'),
  currentUser: service('current-user'),

  route: computed('currentPath', function() {
    return this.get('currentPath').split('.').join('-');
  }),

  actions: {
    logout: function() {
      this.get('session').invalidate();
    },
  }
});
