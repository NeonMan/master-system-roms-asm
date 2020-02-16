includedirs = -I../include/

all: masterterm.ihx

objects: masterterm.rel

masterterm.rel: masterterm.S
	sdasz80 $(includedirs) -l -o -g -s -y -w $<

masterterm.ihx: objects
	sdcc $(includedirs) -o masterterm.ihx -mz80 --no-std-crt0 --code-loc 64 --no-peep --no-peep-return *.rel
	makebin -s 65536 masterterm.ihx masterterm.sms
	python3 ../scripts/make_bootable.py masterterm.sms masterterm.boot.sms

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
selfboot: masterterm.ihx
	cat ../selfboot/selfboot.sms masterterm.boot.sms > masterterm.selfboot.sms

winselfboot: masterterm.ihx
	type ..\selfboot\selfboot.sms masterterm.boot.sms > masterterm.selfboot.sms
