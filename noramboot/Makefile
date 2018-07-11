
all: rom

objects: noramboot.rel vdp.rel font.rel xmodem-boot.rel

noramboot.rel: noramboot.S
	sdasz80 -l -o -g -s -g noramboot.S

vdp.rel: vdp.S
	sdasz80 -l -o -g -s -g vdp.S

font.rel: font.S
	sdasz80 -l -o -g -s -g font.S

xmodem-boot.rel: xmodem-boot.S
	sdasz80 -l -o -g -s -g xmodem-boot.S

rom: objects
	sdcc -o noramboot.ihx -mz80 --no-std-crt0 --code-loc 128 --no-peep --no-peep-return *.rel
	makebin -p noramboot.ihx noramboot.sms
	python3 ../scripts/fix-checksum.py noramboot.sms

clean:
	-rm *.rel
	-rm *.map
	-rm *.lk
	-rm *.sym
	-rm *.lst
	-rm *.ihx
	-rm *.noi
	-rm *.sms

