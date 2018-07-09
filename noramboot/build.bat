@echo off
call clean.bat
cls

sdasz80 -l -o -s -g noramboot.S
sdasz80 -l -o -s -g vdp.S
sdasz80 -l -o -s -g font.S
sdasz80 -l -o -s -g xmodem-boot.S
sdasz80 -l -o -s -g test-cr16-xmodem.S
sdasz80 -l -o -s -g test-serial.S

rem --- 128 for normal ROM
rem --- 49152 for RAM base (+128 -> 49280)
sdcc -o noramboot.ihx -mz80 --no-std-crt0 --code-loc 128 --no-peep --no-peep-return xmodem-boot.rel font.rel vdp.rel noramboot.rel
sdcc -o test-crc16-xmodem.ihx -mz80 --no-std-crt0 --code-loc 128 test-cr16-xmodem.rel font.rel vdp.rel
sdcc -o test-serial.ihx -mz80 --no-std-crt0 --code-loc 128 --no-peep --no-peep-return font.rel vdp.rel test-serial.rel

makebin -p noramboot.ihx noramboot.sms
makebin -p test-crc16-xmodem.ihx test-crc16-xmodem.sms
makebin -p test-serial.ihx test-serial.sms

rem --- RAM versions
rem sdcc -o test-crc16-xmodem-ram.ihx -mz80 --no-std-crt0 --code-loc 49152 test-cr16-xmodem.rel font.rel vdp.rel 
rem makebin -p -s 65535 test-crc16-xmodem-ram.ihx test-crc16-xmodem-ram-raw.sms
rem python ../scripts/trim.py test-crc16-xmodem-ram-raw.sms test-crc16-xmodem-ram.sms 49152


python ..\scripts\fix-checksum.py noramboot.sms
python ..\scripts\fix-checksum.py test-crc16-xmodem.sms
python ..\scripts\fix-checksum.py test-serial.sms

pause
