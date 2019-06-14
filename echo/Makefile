includedirs = -I../include/

all: echo.ihx

objects: echo.rel

echo.rel: echo.S
	sdasz80 $(includedirs) -l -o -g -s -y -w $<

echo.ihx: objects
	sdcc $(includedirs) -o echo.ihx -mz80 --no-std-crt0 --code-loc 64 --no-peep --no-peep-return *.rel
	makebin -p -s 65536 echo.ihx echo.sms
	../scripts/make_bootable.py echo.sms echo.boot.sms --no-vram

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
selfboot: echo.ihx
	cat ../selfboot/selfboot.sms echo.boot.sms > echo.selfboot.sms
	../scripts/pad-selfboot.sh echo.selfboot.sms
