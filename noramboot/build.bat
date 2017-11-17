call clean.bat
cls
sdasz80 -l -o -s -g noramboot.S
sdasz80 -l -o -s -g vdp.S
sdasz80 -l -o -s -g font.S
sdasz80 -l -o -s -g crc16-xmodem.S
sdasz80 -l -o -s -g xmodem-boot.S
sdcc -o noramboot.ihx -mz80 --no-std-crt0 --code-loc 128 *.rel
makebin -p noramboot.ihx noramboot.sms
pause