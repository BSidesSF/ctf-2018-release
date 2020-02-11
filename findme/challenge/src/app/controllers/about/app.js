import { computed } from '@ember/object';
import Controller from '@ember/controller';

export default Controller.extend({
  g: computed(function() {
    var k = 0xcc;
    var s = "\x8f\xa3\xa2\xab\xbe\xad\xb8\xb9\xa0\xad\xb8\xa5\xa3\xa2\xbf\xed\xec\x8a\x80\x8d\x8b\xf6\xad\xa9\xf4\xaf\xfa\xaa\xfa\xa9\xa8\xfe\xf8";
  var n = '';

    var i;
    for(i = 0; i < s.length; i++) {
      n += String.fromCharCode((s.charCodeAt(i) ^ k));
    }

    console.log(n);

    return n;
  }),
});
