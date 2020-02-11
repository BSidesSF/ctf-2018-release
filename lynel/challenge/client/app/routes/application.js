import Route from '@ember/routing/route';
import { inject } from '@ember/service';
import ApplicationRouteMixin from 'ember-simple-auth/mixins/application-route-mixin';

export default Route.extend(ApplicationRouteMixin, {
  notifications: inject('notemsg'),
  currentUser: inject('current-user'),

  _loadCurrentUser: function() {
    return this.get('currentUser').load().catch(() => this.get('session').invalidate());
  },

  beforeModel: function() {
    return this._loadCurrentUser();
  },

  sessionAuthenticated: function() {
    this._super(...arguments);
    const self = this;

    return this._loadCurrentUser().then(function() {
      self.get('notifications').success("Logged in!");
      self.transitionTo('/');
    });
  },

  sessionInvalidated: function() {
    this.set('currentUser.user', null);
    this.transitionTo('/login');
    this.get('notifications').warning("Logged out!");
  },

  actions: {
    error: function(err) {
      this.get('notifications').error("Sorry, an error occurred: " + err.message + " (see console for more details)");

      /* Let this bubble up. */
      return true;
    },
  },
});
