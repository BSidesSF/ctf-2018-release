import NotificationsService from 'ember-cli-notifications/services/notification-messages-service';

export default NotificationsService.extend({
  init() {
    this._super(...arguments);
    this.setDefaultAutoClear(true);
  }
});

