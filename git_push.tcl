#!/bin/sh
# \

package require Expect

spawn git push origin master 
expect "Username for \'https://github.com\':"
send "JDRiley\r"
expect "Password for \'https://JDRiley@github.com\':"
send "Qrzmixfornow08\r"
expect eof
exit 0
