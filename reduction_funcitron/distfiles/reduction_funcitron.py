#!/usr/bin/env python3
import argparse
import math
import random
import hashlib
import codecs

"""
Given the following MD5 Rainbow table that was generate using the attached
program, determine the password for this md5 hash:

    5663a63cd71f654627fdf0f90d9c2058

Here is the rainbow table:

    [['mark', 'Y2OuTCTYXeUMaN4DWr4B'],
     ['baggett', 'NpJZe4eesU8qJlNzGMPr'],
     ['ron', '8GzqN6E7voOKkdcPgOiP'],
     ['bowes', 'aLHplSmsM8ZrTZKxHsfr'],
     ['bsidessf', 'bDYidRnoiV3TS2lShBLC'],
     ['bsidesaugusta', '0AYiefH2gXVuvDrmGpUd'],
     ['gpyc', 'YVLXsgNdmXsdBiOzMOL5'],
     ['sec573', '5ZzlCF0gioDLULrSDacZ']]

Good luck!
"""

def reduction_function(ahash_hexdigest):
    charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    ahash_digest = codecs.decode(ahash_hexdigest,"HEX")
    bignumber = int.from_bytes(ahash_digest, byteorder = "little")
    base = len(charset)
    max_length = 20
    rnum = bignumber % base**max_length
    random_length = math.ceil(math.log(rnum, base))
    pw = ""
    while bignumber > 0 and len(pw)<random_length:
        pw = pw + charset[bignumber%base]
        bignumber -= (bignumber % base) 
        bignumber //= base-1
    return pw

parser = argparse.ArgumentParser()
parser.add_argument("HASH", type=str, help = "A Hexidecimal hash to reduce to the next possible password")
parser.add_argument("-r","--rehash", action="store_true", help = "If this argument is passed it will reduce the hash to a new password and then convert the new password to a MD5 hash.")
args = parser.parse_args()

newpw = reduction_function(args.HASH)
if args.rehash:
    newpw= hashlib.md5(newpw.encode()).hexdigest()

print(newpw)