import { inject } from '@ember/service';
import Route from '@ember/routing/route';
import AuthenticatedRouteMixin from 'ember-simple-auth/mixins/authenticated-route-mixin';

export default Route.extend(AuthenticatedRouteMixin, {

  currentUser: inject('current-user'),

  model() {
    // Make a copy of the fields so we can change them without changing the
    // model
    return {
      'name': this.get('currentUser').get('user.name'),
      'email': this.get('currentUser').get('user.email'),
      'comment': this.get('currentUser').get('user.comment'),
    };
  },
});
