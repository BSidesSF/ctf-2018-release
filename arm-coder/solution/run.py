import sys
import os.path

buf = open(os.path.join(os.path.dirname(__file__), 'shellcode')).read()
print(len(buf))
sys.stdout.write(buf)
