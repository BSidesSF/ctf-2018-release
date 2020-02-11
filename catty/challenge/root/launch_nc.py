import socket
import subprocess as sp
import re
import os
import random


def main():
    try:
        if "SESSIONID" not in os.environ:
            print('Please Run the following command to Set a Random SessionID For your personal Netcat Instance and run again:\n\nexport SESSIONID="$(printf $RANDOM$RANDOM)"\n')
            return
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('127.0.0.1', 1234))
        s.send(('80ndh834gt42d87ns-'+os.environ["SESSIONID"]+'\r\n').encode())
        response = s.recv(4096).decode('utf-8')
        print(response)
    except:
        print('Error Contacting Netcat Process. Contact Bsides Admins.')
        pass

if __name__ == "__main__":
    main()