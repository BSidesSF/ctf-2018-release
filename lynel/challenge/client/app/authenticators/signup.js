import { inject } from '@ember/service';
import OAuth2PasswordGrant from 'ember-simple-auth/authenticators/oauth2-password-grant';
import config from '../config/environment';

export default OAuth2PasswordGrant.extend({
  ajax: inject(),
  serverTokenEndpoint: config.torii.providers['local-signup'].tokenExchangeUri,
});
