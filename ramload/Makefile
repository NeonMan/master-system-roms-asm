#Add noramboot as well since it 'emulates' XMODEM loads
includedirs = -I../include/ -I../noramboot/

all: ramload.ihx

objects: ramload.rel

ramload.rel: ramload.S
	sdasz80 $(includedirs) -l -o -g -s -y -w $<

ramload.ihx: objects
	sdcc $(includedirs) -o ramload.ihx -mz80 --no-std-crt0 --code-loc 64 --no-peep --no-peep-return *.rel
	makebin -p ramload.ihx ramload.sms
	python3 ../scripts/fix-checksum.py ramload.sms 8

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

