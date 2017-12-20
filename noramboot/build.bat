@echo off
call clean.bat
cls

sdasz80 -l -o -s -g noramboot.S
sdasz80 -l -o -s -g vdp.S
sdasz80 -l -o -s -g font.S
sdasz80 -l -o -s -g xmodem-boot.S
sdasz80 -l -o -s -g test-cr16-xmodem.S
sdasz80 -l -o -s -g test-serial.S

sdcc -o noramboot.ihx -mz80 --no-std-crt0 --code-loc 128 xmodem-boot.rel font.rel vdp.rel noramboot.rel
sdcc -o test-crc16-xmodem.ihx -mz80 --no-std-crt0 --code-loc 128 font.rel vdp.rel test-cr16-xmodem.rel
sdcc -o test-serial.ihx -mz80 --no-std-crt0 --code-loc 128 font.rel vdp.rel test-serial.rel

makebin -p noramboot.ihx noramboot.sms
makebin -p test-crc16-xmodem.ihx test-crc16-xmodem.sms
makebin -p test-serial.ihx test-serial.sms

python ..\scripts\fix-checksum.py noramboot.sms
python ..\scripts\fix-checksum.py test-crc16-xmodem.sms
python ..\scripts\fix-checksum.py test-serial.sms

pause
