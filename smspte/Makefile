
all: rom

objects: smspte.rel

smspte.rel: smspte.S
	sdasz80 -l -o -g -s -g smspte.S

rom: objects
	sdcc -o smspte.ihx -mz80 --no-std-crt0 --code-loc 128 *.rel
	makebin -p smspte.ihx smspte.sms

clean:
	-rm *.rel
	-rm *.map
	-rm *.lk
	-rm *.sym
	-rm *.lst
	-rm *.ihx
	-rm *.noi
	-rm *.sms

