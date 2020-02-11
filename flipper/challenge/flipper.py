""" Coin Flipper Service """

from Crypto.Cipher import AES
import base64
import errno
import flask
import functools
import hashlib
import hmac
import json
import math
import os
import os.path
import random
import re


SESSION_ID = 'flipsession'

app = flask.Flask(__name__)


class FlipperError(Exception):
    """Base Exception"""


class InvalidUsername(FlipperError):
    """Raised when a username does not match expectations."""


class UserExists(FlipperError):
    """Raised when trying to register an existing user."""


class InvalidPassword(FlipperError):
    """Raised when invalid password on login."""


class InvalidCookie(FlipperError):
    """Raised when there's an issue with the cookie."""


class InvalidPadding(InvalidCookie):
    """Padding-specific exception."""


class InvalidCSRF(FlipperError):
    """Raised with invalid CSRF."""


class Config(object):
    """Configuration"""

    __all__ = ['cookie_key', 'data_dir']

    def __init__(self, fname):
        self.fname = fname
        self.cookie_key = os.urandom(32).encode('hex')
        self.data_dir = 'data'
        try:
            with open(fname) as fp:
                config = json.load(fp)
            for attr in self.__all__:
                if attr in config:
                    setattr(self, attr, config.get(attr))
        except IOError:
            pass
        if not os.path.exists(self.data_dir):
            os.mkdir(self.data_dir, 0o700)

    def save(self):
        try:
            saves = {}
            for attr in self.__all__:
                saves[attr] = getattr(self, attr)
            with open(self.fname, 'w') as fp:
                json.dump(saves, fp)
        except IOError:
            pass

    @classmethod
    def get(cls):
        try:
            return cls._singleton
        except AttributeError:
            cls._singleton = cls('config.json')
            cls._singleton.save()
            return cls._singleton


class User(object):
    """A user in the flipper system."""

    USERNAME_RE = r'^[A-Za-z0-9]{3,32}$'

    def __init__(self, username, is_admin, csrf=None):
        if not re.match(self.USERNAME_RE, username):
            raise InvalidUsername("Invalid Username")
        self.username = username
        self.admin = bool(is_admin)
        self.data = {
                'admin': bool(is_admin),
        }
        if csrf:
            self.data['csrf_token'] = csrf
        self._loaded = False

    @classmethod
    def from_cookie(cls, cookie_string):
        config = Config.get()
        crypter = CookieCrypter(config.cookie_key)
        # is_admin is stored for quick lookups
        data = crypter.decrypt(cookie_string)
        return cls(data['username'], data['admin'], data['csrf_token'])

    @classmethod
    def register(cls, username, password):
        user = cls(username, False)
        user.password = password
        user.data['csrf_token'] = os.urandom(16).encode('hex')
        try:
            fd = os.open(user._filename, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
        except OSError as e:
            if e.errno == errno.EEXIST:
                raise UserExists("User already exists.")
            raise
        with os.fdopen(fd, 'w') as fp:
            json.dump(user.data, fp)
        return user

    @classmethod
    def login(cls, username, password):
        user = cls(username, False)
        user.load()
        hashed = hashlib.sha256(password.encode('utf-8')).hexdigest()
        if not hmac.compare_digest(hashed, user.password.encode('utf-8')):
            raise InvalidPassword("Invalid password.")
        user.admin = user.data['admin']
        return user

    @classmethod
    def by_name(cls, username):
        user = cls(username, False)
        if not os.path.exists(user._filename):
            return None
        return user

    @property
    def session_cookie(self):
        config = Config.get()
        crypter = CookieCrypter(config.cookie_key)
        # is_admin is stored for quick lookups
        return crypter.encrypt(
                {
                    'username': self.username,
                    'admin': self.admin,
                    'csrf_token': self.csrf_token,
                })

    @property
    def _filename(self):
        config = Config.get()
        return os.path.join(config.data_dir, self.username + '.json')

    @property
    def password(self):
        self.load()
        return self.data.get('password', os.urandom(32).encode('hex'))

    @password.setter
    def password(self, pw):
        # TODO: use a stronger hash
        self.data['password'] = hashlib.sha256(pw.encode('utf-8')).hexdigest()

    @property
    def flips(self):
        self.load()
        return self.data.get('saved_flips', '')

    @flips.setter
    def flips(self, value):
        self.load()
        self.data['saved_flips'] = value

    @property
    def save_name(self):
        self.load()
        return self.data.get('saved_name')

    @save_name.setter
    def save_name(self, value):
        self.load()
        self.data['saved_name'] = value

    @property
    def csrf_token(self):
        self.load()
        return self.data.get('csrf_token', os.urandom(16).encode('hex'))

    def load(self):
        if self._loaded:
            return self
        try:
            with open(self._filename) as fp:
                self.data.update(json.load(fp))
            self._loaded = True
        except IOError:
            pass
        return self

    def save(self):
        with open(self._filename, 'w') as fp:
            json.dump(self.data, fp)


class HKDF(object):
    """Implementation of RFC5869, I think."""

    hash_func = hashlib.sha256

    def __init__(self, ikm, salt=None):
        self.hashlen = self.hash_func().digest_size
        if salt is None:
            salt = '\x00' * self.hashlen
        self.prk = self.HMAC(salt, ikm)

    @classmethod
    def HMAC(cls, key, message):
        mac = hmac.new(key, message, digestmod=cls.hash_func)
        return mac.digest()

    def expand(self, info='', L=32):
        if L > 255*self.hashlen:
            raise RuntimeError('Requested longer length than available.')
        t = ''
        okm = ''
        N = int(math.ceil(float(L)/self.hashlen))
        n = 0
        while n < N:
            t = self.HMAC(self.prk, t + info + chr(n))
            n += 1
            okm += t
        return okm[:L]


class PKCS7(object):

    @staticmethod
    def pad(s, size=16):
        padlen = size - (len(s) % size)
        assert padlen < 256
        assert padlen > 0
        return s + (chr(padlen) * padlen)

    @staticmethod
    def strip(s):
        padlen = ord(s[-1])
        if padlen == 0:
            raise InvalidPadding('Null padding.')
        pad = s[-padlen:]
        # Avoid padding oracle timing attacks, I hope.
        expected_pad = chr(padlen) * padlen
        if not hmac.compare_digest(pad, expected_pad):
            raise InvalidPadding('Bad byte in padding!')
        return s[:-padlen]


class CookieCrypter(object):

    hash_func = hashlib.sha256

    def __init__(self, key):
        hkdf = HKDF(key)
        self.mac_key = hkdf.expand(info='mac', L=256/8)
        self.crypt_key = hkdf.expand(info='crypt', L=256/8)

    def encrypt(self, data):
        data_serialized = json.dumps(
                data, sort_keys=True, separators=(',', ':'))
        iv = os.urandom(AES.block_size)
        aes_impl = AES.new(self.crypt_key, mode=AES.MODE_CBC, IV=iv)
        data_crypted = aes_impl.encrypt(
                PKCS7.pad(data_serialized, AES.block_size))
        signer = hmac.new(self.mac_key, data_crypted, digestmod=self.hash_func)
        mac = signer.digest()
        cookie_data = iv + data_crypted + mac
        return base64.b64encode(cookie_data)

    def decrypt(self, data):
        try:
            data = base64.b64decode(data)
        except TypeError:
            raise InvalidPadding('Invalid B64 Padding.')
        mac_size = self.hash_func().digest_size
        iv = data[:AES.block_size]
        mac = data[-mac_size:]
        ctext = data[AES.block_size:-mac_size]
        if (len(ctext) == 0 or len(ctext) % AES.block_size != 0 or
                len(iv) != AES.block_size or len(mac) != mac_size):
            raise InvalidCookie('Invalid cookie length.')
        signer = hmac.new(self.mac_key, ctext, digestmod=self.hash_func)
        if not hmac.compare_digest(mac, signer.digest()):
            raise InvalidCookie('Invalid cookie signature!')
        aes_impl = AES.new(self.crypt_key, mode=AES.MODE_CBC, IV=iv)
        decrypted = aes_impl.decrypt(ctext)
        plaintext = PKCS7.strip(decrypted)
        try:
            return json.loads(plaintext)
        except ValueError:
            raise InvalidCookie('Invalid cookie contents.')


def with_user(f):
    @functools.wraps(f)
    def add_user_arg(*args, **kwargs):
        user = None
        session = flask.request.cookies.get(SESSION_ID)
        if session:
            try:
                user = User.from_cookie(session)
            except InvalidCookie:
                resp = flask.redirect('/')
                resp.delete_cookie(SESSION_ID)
                return resp
        return f(user, *args, **kwargs)
    return add_user_arg


def redirect_session(user, dest='/'):
    resp = flask.redirect(dest)
    resp.set_cookie(SESSION_ID, user.session_cookie)
    return resp


def get_flips(n, seed=None):
    n = n if n <= 10 else 10
    if seed is not None:
        random.seed(int(seed))
    return ''.join(random.choice(('T', 'H')) for c in range(n))


@app.route('/')
@with_user
def index(user):
    no = int(flask.request.args.get('no', 2))
    seed = flask.request.args.get('seed', None)
    flips = get_flips(no, seed)
    return flask.render_template(
            'index.html', user=user, flips=flips, no=len(flips),
            winner=(flips == ('H'*10)))


@app.route('/login', methods=['POST', 'GET'])
def login():
    error = ''
    if flask.request.method == 'POST':
        username = flask.request.form.get('username', '')
        password = flask.request.form.get('password', '')
        try:
            user = User.login(username, password)
            return redirect_session(user)
        except InvalidPassword:
            error = 'Invalid Password'
    return flask.render_template('login.html', error=error)


@app.route('/logout')
def logout():
    resp = flask.redirect('/')
    resp.delete_cookie(SESSION_ID)
    return resp


@app.route('/flipper.py')
def source():
    return flask.send_file(__file__)


@app.route('/register', methods=['GET', 'POST'])
def register():
    error = ''
    if flask.request.method == 'POST':
        username = flask.request.form.get('username', '')
        password = flask.request.form.get('password', '')
        try:
            user = User.register(username, password)
            return redirect_session(user)
        except FlipperError as ex:
            error = str(ex)
    return flask.render_template('register.html', error=error)


@app.route('/save', methods=['POST'])
@with_user
def save(user):
    if not user:
        return flask.abort(403)
    try:
        flips = flask.request.form['flips']
        name = flask.request.form['name']
        csrf_token = flask.request.form['csrf_token']
    except KeyError:
        return flask.abort(500)
    # TODO: fix timing attack
    if csrf_token != user.csrf_token:
        return flask.abort(403)
    for c in flips:
        if c not in ('T', 'H'):
            flask.abort(500)
    user.flips = flips
    user.save_name = name
    user.save()
    return flask.redirect('/')


@app.route('/admin/list')
@with_user
def list_users(user):
    if not user or not user.admin:
        return flask.abort(403)
    cfg = Config.get()
    users = [x.split('.')[0] for x in os.listdir(cfg.data_dir)]
    users.sort()
    return flask.render_template('admin_list.html', user=user, users=users)


@app.route('/admin/view/<username>')
@with_user
def view_user(user, username):
    if not user or not user.admin:
        return flask.abort(403)
    target_user = User.by_name(username)
    if not target_user:
        return flask.abort(404)
    return flask.render_template(
            'admin_view.html', user=user, target=target_user)


if __name__ == '__main__':
    Config.get().save()
    app.run(debug=True)
