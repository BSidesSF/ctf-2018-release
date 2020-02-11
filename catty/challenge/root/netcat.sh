#!/bin/bash

function run_it() {
	cd /home/netcat
	while [ 1 ]; do timeout 30 bash -c 'echo "10000" | nc -n -lubp 4444 127.255.26.100 10000 && wall -n < flag.txt'; done
}
/usr/sbin/sshd -D &
run_it
