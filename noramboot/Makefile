
all: rom

objects: noramboot.rel vdp.rel font.rel crc16-xmodem.rel xmodem-boot.rel

noramboot.rel: noramboot.S
	sdasz80 -l -o -g -s noramboot.S

vdp.rel: vdp.S
	sdasz80 -l -o -g -s -g vdp.S

font.rel: font.S
	sdasz80 -l -o -g -s -g font.S

crc16-xmodem.rel: crc16-xmodem.S
	sdasz80 -l -o -g -s -g crc16-xmodem.S

xmodem-boot.rel: xmodem-boot.S
	sdasz80 -l -o -g -s -g xmodem-boot.S

rom: objects
	-sdcc -o noramboot.ihx -mz80 --no-std-crt0 --code-loc 128 *.rel
	makebin -p noramboot.ihx noramboot.sms

clean:
	-rm *.rel
	-rm *.map
	-rm *.lk
	-rm *.sym
	-rm *.lst
	-rm *.ihx
	-rm *.noi
	-rm *.sms

