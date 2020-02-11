import { inject } from '@ember/service';
import AjaxService from 'ember-ajax/services/ajax';
import { computed } from '@ember/object';

export default AjaxService.extend({
  session: inject(),

  headers: computed('session.data.authenticated.access_token', {
    get() {
      let headers = {};
      const authToken = this.get('session.data.authenticated.access_token');
      if(authToken) {
        headers['Authorization'] = 'Bearer ' + authToken;
      }
      return headers;
    }
  })
});
