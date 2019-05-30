#!/bin/sh
#Append $1 to the ramload.sms ROM
cat ramload.sms "$1" > "$1.boot.sms"
