#Add noramboot as well since it 'emulates' XMODEM loads
includedirs = -I../include/ -I../noramboot/

all: selfboot.ihx

objects: selfboot.rel

selfboot.rel: selfboot.S
	sdasz80 $(includedirs) -l -o -g -s -y -w $<

selfboot.ihx: objects
	sdcc $(includedirs) -o selfboot.ihx -mz80 --no-std-crt0 --code-loc 64 --no-peep --no-peep-return *.rel
	makebin -p selfboot.ihx selfboot.sms
	python3 ../scripts/fix-checksum.py selfboot.sms 8

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

