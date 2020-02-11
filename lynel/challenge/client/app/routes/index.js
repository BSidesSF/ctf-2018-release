import Route from '@ember/routing/route';
import AuthenticatedRouteMixin from 'ember-simple-auth/mixins/authenticated-route-mixin';

// Automatically redirect to /login by using the AuthenticatedRouteMixin
export default Route.extend(AuthenticatedRouteMixin, {
});
