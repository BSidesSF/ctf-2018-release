import { inject } from '@ember/service';
import DS from 'ember-data';
import DataAdapterMixin from 'ember-simple-auth/mixins/data-adapter-mixin';

export default DS.RESTAdapter.extend(DataAdapterMixin, {
  namespace: 'api',
  ajax: inject('ajax'),

  queryRecord() {
    return this.get('ajax').request('/api/user/current_user?cache_buster=' + Math.random());
  },

  updateRecord(store, type, snapshot) {
    var data = {};
    var serializer = store.serializerFor(type.modelName);

    serializer.serializeIntoHash(data, type, snapshot);

    var url = '/api/user/current_user?cache_buster=' + Math.random();

    return this.get('ajax').put(url, { data: data });
  }
});
