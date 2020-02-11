import os
import json
import base64
import sys

orig = {
        'admin': False,
        'username': 'foo@bar.com',
        'csrf_token': os.urandom(8).encode('hex'),
        }


def xor(a, b):
    return ''.join(chr(ord(x) ^ ord(y)) for x, y in zip(a, b))


input_cookie = sys.argv[1].strip()

plaintext = json.dumps(orig, sort_keys=True, separators=(',', ':'))
pos = plaintext.index('false')
print(input_cookie)
raw = base64.b64decode(input_cookie)
iv_out = raw[:pos]
iv_out += xor(raw[pos:pos+5], xor('false', 'true '))
iv_out += raw[pos+5:16]
print(raw[:16].encode('hex'))
print(iv_out.encode('hex'))
tampered = base64.b64encode(iv_out + raw[16:])
print(tampered)
