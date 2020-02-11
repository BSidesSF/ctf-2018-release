# Overview

This is a server that requires users to verify themselves. This is done by
verifying their IP address by signing it with an unknown private key.

The problem is, only the first 4 bytes are checked, so the user is able to fake
a signature using only the public key!

# Crypto details

M - message -> <ip><pad>
e - encrypting exponent
d - decrypting exponent
C - ciphertext -> M^e % N
Decrypt - C^d mod N

N - modulous - find two primes P and Q and then P * Q = N
e - exponent - 3 or 65537 (probably the latter)
d and e are derived from P and Q

e - publicExponent: 65537 (0x10001)

d - privateExponent:
14756cd1f22069438f3cb4d0b1b5182de3a2d2ebe2776e7a14ce95e2dfc0dfa443b461afc73ffdcdb5c65d04dcd80f705ee8d6313e9efd6e6acabbda0c8910e699162594d0c60b1788575695001e4de978582c87641c566470a00d422af0729bede8ab3dcc189a91abeeddddc6b0c061e946aef8f0054b265d30985fce00d111

n - modulus:
0095715ed3927fecf060cad0c9f916c42a568876346d34a464fcb58c4f98ee4aca1c52c93885c1d3e10ed6e0b49c80a2c4087af3d8e029e3648eb2ebef58ff959fe31785b4e12a49b2099612c53fcb49d01364816d015d8ff076cfb5af10027a84b77673f8eb022a46bdab960405bf42befc648c2f73e47993ba82140b4ce755a5


$ openssl genrsa 1024 > /tmp/key.pem
$ openssl rsa -in /tmp/key.pem -text

e is the public exponent
d is the private exponant
n is the modulus

For testing, a valid signature for 127.0.0.1 ("\x7F\x00\x00\x01") padding with 0x41 is:

4e7dac965f5d30d23e929e6fbacbb672781047ff67a8a3a24f71aa840f9079b70d2d2fb84bef9927d0081384e185e8b28bcce60679160a76410d0e32d313660c1fd48d1c92a93bfa8eb92214667f00343d91fadcec4e6aa420435322b1cd080f761e9d1fbeb48ac3e2524e1e00c8591495ff7594d01c2f7a830e1bf375bad8a1

\x4e\x7d\xac\x96\x5f\x5d\x30\xd2\x3e\x92\x9e\x6f\xba\xcb\xb6\x72\x78\x10\x47\xff\x67\xa8\xa3\xa2\x4f\x71\xaa\x84\x0f\x90\x79\xb7\x0d\x2d\x2f\xb8\x4b\xef\x99\x27\xd0\x08\x13\x84\xe1\x85\xe8\xb2\x8b\xcc\xe6\x06\x79\x16\x0a\x76\x41\x0d\x0e\x32\xd3\x13\x66\x0c\x1f\xd4\x8d\x1c\x92\xa9\x3b\xfa\x8e\xb9\x22\x14\x66\x7f\x00\x34\x3d\x91\xfa\xdc\xec\x4e\x6a\xa4\x20\x43\x53\x22\xb1\xcd\x08\x0f\x76\x1e\x9d\x1f\xbe\xb4\x8a\xc3\xe2\x52\x4e\x1e\x00\xc8\x59\x14\x95\xff\x75\x94\xd0\x1c\x2f\x7a\x83\x0e\x1b\xf3\x75\xba\xd8\xa1

