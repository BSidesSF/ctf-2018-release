#!/usr/bin/expect
set timeout 30
spawn adduser bsides
expect "Enter new UNIX password:" { send "bsides\r" }
expect "Retype new UNIX password:" { send "bsides\r" }
send "\r"
send "\r"
send "\r"
send "\r"
send "\r"
send "Y\r"
interact