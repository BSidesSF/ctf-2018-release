#!/usr/bin/env python3
import subprocess as sp
import socketserver
import threading
import socket
import subprocess
import codecs
import random
import os
import signal
import time
import queue


netcat_process_container = {}

def check_port(port_no):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.bind(("127.0.0.1", port_no))
    except socket.error as e:
        if e.errno == 98:
            return True
        else:
            pass
    s.close()
    return False

def kill_process(tty_num):
    try:
        global netcat_process_container
        if tty_num in netcat_process_container:
            print("DELETED!")
            os.killpg(netcat_process_container[tty_num], signal.SIGTERM)
            del netcat_process_container[tty_num]
    except:
        return

def execute_netcat(cmd, q):
    try:
        pro = sp.Popen(cmd, stdout=sp.PIPE, shell=True, preexec_fn=os.setsid) 
        q.put(os.getpgid(pro.pid))
        pro.wait()
    except:
        return

def start_ssh():
    ssh = sp.Popen('/usr/sbin/sshd -D',shell=True)
    ssh.wait()

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        try:
            global netcat_process_container
            while True:
                #self.request.sendall(b'Would you Like a new Netcat Listener? "Y"es or "N"o:\n')
                result = self.request.recv(1024).strip().lower().decode('utf-8')
                if bool(result):
                    break
            while True:
                try:
                    result, tty_num = result.split('-')
                    if result != '80ndh834gt42d87ns' or type(int(tty_num)) != int:
                        break
                    tty_num = int(tty_num)
                except Exception as e:
                    #print(str(e))
                    break
                threading.Thread(target=kill_process, args=[tty_num]).start()
                listen_port = random.choice(range(2000, 65535))
                while check_port(listen_port):
                    listen_port = random.choice(range(2000, 65535))
                tmp = list(range(2000, 65535))
                tmp.remove(listen_port)
                from_port = random.choice(tmp)
                while check_port(from_port):
                    from_port = random.randint(2000, 65535)
                tmp = list(range(2000, 65535))
                tmp.remove(listen_port)
                tmp.remove(from_port)
                third_port = random.choice(tmp)
                while check_port(third_port):
                    third_port = random.choice(tmp)
                netcat_command = 'echo 10000 | nc -n -lubp '+str(listen_port)+" 127."+str(random.randint(2,255))+"."+str(random.randint(2,255))+"."+str(random.randint(2,255))+" "+str(from_port)+' && nc 127.0.0.1 '+ str(third_port) + ' < flag.txt'
                #netcat_command = 'nc -n -lubp '+str(listen_port)+" 127."+str(random.randint(2,255))+"."+str(random.randint(2,255))+"."+str(random.randint(2,255))+" "+str(from_port)+' < flag.txt'
                q = queue.Queue()
                full_netcat_command = "timeout 300 bash -c \"while [ 1 ]; do timeout 30 bash -c '"+netcat_command+"'; done\""
                #print(full_netcat_command)
                threading.Thread(target=execute_netcat, args=[full_netcat_command, q]).start()
                while True:
                    if q.empty():
                        time.sleep(1)
                    else:
                        process_id = q.get()
                        break
                netcat_process_container[tty_num] = process_id
                self.request.sendall(('\n---------------------------------------------------\nNetcat has been run on your behalf, with the following command:\n\n'+netcat_command+'\n\nAll you have to do is connect to it and get the flag (within 5 minutes)!\n---------------------------------------------------\n').encode())
                break
        except:
            pass



class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


if __name__ == "__main__":
    threading.Thread(target=start_ssh).start()
    HOST, PORT = "127.0.0.1", 1234
    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    server.serve_forever()

