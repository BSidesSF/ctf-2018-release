import sys
import telnetlib

FORWARD='w'
BACK='s'
LEFT='a'
RIGHT='d'
LOOK='l'
TALK='t'
ITEM1='1'

t = telnetlib.Telnet('localhost', 8083)
inputs = "!" + FORWARD*7 + LOOK + RIGHT + FORWARD*5 + LOOK + FORWARD*2 + LEFT + FORWARD*9
inputs += RIGHT + FORWARD*4 + RIGHT + FORWARD + LEFT + ITEM1 + FORWARD*4 + ITEM1
inputs += FORWARD*2 + RIGHT + FORWARD*3 + RIGHT + ITEM1 + FORWARD*2 + LEFT + FORWARD*2
inputs += LEFT + ITEM1 + FORWARD*2+ ITEM1 + FORWARD*2 + RIGHT + FORWARD*2
inputs += (ITEM1 + FORWARD)*4 + RIGHT + ITEM1 + FORWARD*3 + LEFT + FORWARD*3
inputs += ITEM1 + FORWARD + ITEM1 + FORWARD + LEFT + FORWARD*4

inputs+="!"+ RIGHT*4
t.write(inputs)
while True:
    sys.stdout.write(t.read_eager())
    sys.stdout.flush()
