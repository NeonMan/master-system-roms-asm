
all: rom

objects: smspte.rel font.rel

font.rel:
	sdasz80 -l -o -g -s -g font.S

smspte.rel: smspte.S
	sdasz80 -l -o -g -s -g smspte.S

rom: objects
	sdcc -o smspte.ihx -mz80 --no-std-crt0 --code-loc 128 *.rel
	makebin -p smspte.ihx smspte.sms
	python3 ../scripts/fix-checksum.py smspte.sms

clean:
	-rm *.rel
	-rm *.map
	-rm *.lk
	-rm *.sym
	-rm *.lst
	-rm *.ihx
	-rm *.noi
	-rm *.sms

