call clean.bat
cls
sdasz80 -l -o -s -g noramboot.S
sdasz80 -l -o -s -g vdp.S
sdasz80 -l -o -s -g font.S
sdasz80 -l -o -s -g crc16-xmodem.S
sdcc -o noramboot.ihx --code-loc 0100 *.rel
makebin -p noramboot.ihx noramboot.sms
pause
