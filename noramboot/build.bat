call clean.bat
cls
sdasz80 -l -o -s -g noramboot.S
sdasz80 -l -o -s -g vdp.S
sdasz80 -l -o -s -g font.S
sdcc -o noramboot.ihx --code-loc 0100 *.rel
makebin noramboot.ihx noramboot.sms
