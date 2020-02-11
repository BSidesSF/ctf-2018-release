#!/usr/bin/expect
set timeout 30
spawn adduser netcat
expect "Enter new UNIX password:" { send "Totes_Difficult_Password1927\r" }
expect "Retype new UNIX password:" { send "Totes_Difficult_Password1927\r" }
send "\r"
send "\r"
send "\r"
send "\r"
send "\r"
send "Y\r"
interact