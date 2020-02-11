//import Ember from 'ember';
import ToriiAuthenticator from 'ember-simple-auth/authenticators/torii';
//import config from '../config/environment';

// This receives the token from Google and exchanges it for an jwt token from
// our back-end service.
export default ToriiAuthenticator.extend({
//  torii: Ember.inject(),
//  ajax: Ember.inject(),
//
//  authenticate() {
//    return this._super(...arguments).then((data) => {
//      var token;
//      if(data.provider == 'google-oauth2-bearer-v2') {
//        token = data.access_token;
//      } else {
//        token = '-unknown provider-';
//      }
//
//      return this.get('ajax').request(config.torii.providers[data.provider].tokenExchangeUri, {
//        type: 'POST',
//        dataType: 'json',
//        contentType: 'application/json',
//        data: JSON.stringify({
//          access_token: token,
//        })
//      }).then( (response) => {
//        return {
//          access_token: response.access_token,
//          provider: data.provider,
//        };
//      })
//    });
//  }
});
