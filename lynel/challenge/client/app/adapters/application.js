import DS from 'ember-data';
import DataAdapterMixin from 'ember-simple-auth/mixins/data-adapter-mixin';

// This ensures that all outgoing Ember-Data requests will have api/ prepended,
// and will use the ember-simple-auth torii authorizer we built
export default DS.RESTAdapter.extend(DataAdapterMixin, {
  namespace: 'api',
  authorizer: 'authorizer:application'
});
