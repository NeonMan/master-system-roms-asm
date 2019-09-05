includedirs = -I../include/
name=flash-tools

all: flash-tools.ihx

objects: flash-tools.rel

flash-tools.rel: flash-tools.S
	sdasz80 $(includedirs) -l -o -g -s -y -w $<

flash-tools.ihx: objects
	sdcc $(includedirs) -o $@ -mz80 --no-std-crt0 --code-loc 64 --no-peep --no-peep-return *.rel
	makebin -p -s 65536 $@ flash-tools.sms
	../scripts/make_bootable.py flash-tools.sms flash-tools.boot.sms

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
selfboot: flash-tools.ihx
	cat ../selfboot/selfboot.sms flash-tools.boot.sms > flash-tools.selfboot.sms
	#../scripts/pad-selfboot.sh flash-tools.selfboot.sms
