includedirs = -I../include/

all: nyan.ihx

objects: nyan.rel

nyan.rel: nyan.S nyan_anim.inc
	sdasz80 $(includedirs) -l -o -g -s -y -w $<

nyan.ihx: objects
	sdcc $(includedirs) -o nyan.ihx -mz80 --no-std-crt0 --code-loc 64 --no-peep --no-peep-return *.rel
	makebin -s 65536 -p nyan.ihx nyan.sms
	../scripts/make_bootable.py nyan.sms nyan.boot.sms

clean:
	-rm *.rel
	-rm *.map
	-rm *.lk
	-rm *.sym
	-rm *.lst
	-rm *.ihx
	-rm *.noi

mrproper: clean
	-rm *.sms

# --- Additional targets ---

#Make an emulator/cartridge friendly ROM from the bootable ROM
ramload: nyan.ihx
	cat ../ramload/ramload.sms nyan.boot.sms > nyan.ramload.sms
