import { inject } from '@ember/service';
import Controller from '@ember/controller';

export default Controller.extend({
  currentUser: inject('current-user'),

  actions: {
  },
});
