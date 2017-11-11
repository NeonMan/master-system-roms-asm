
	;; Imported functions

	.area	_HEADER (ABS)
	
	;; SDSC ROM header
	.org 0x7fe0
	.ascii "SDSC"
	.db 0xff,0xff			; Version (BCD)
	.db 0xff,0xff,0xff,0xff	; Date DD/MM/YYYY (BCD)
	.db 0xff,0xff			; Author pointer
	.db 0xff,0xff			; Name pointer
	.db 0xff,0xff			; Description pointer
	
	;;Make NMI RESET the ROM
	;.org 0x0066
	;JR 0x0000
	
	;; Entry point
	.org 0x0000
	DI
	JP init

	.area _CODE (REL)
	.ascii "/noramboot.S"
	.include /macros.inc/

txt_hello:
	.ascii "------------------------------  "
	.ascii "--     N0RAM Bootloader     --  "
	.ascii "------------------------------  "
	.ascii " Bootloader using UART/XMODEM   "
	.ascii " Hold 2 to use Sync Serial      "
	.ascii "                                "
	.ascii " Booting in: "
	.db 0x00
txt_uart:
	.ascii "CONTROL 2                       "
	.ascii "o o o o o--Vcc (5V, Optional)   "
	.ascii " o o o o                        "
	.ascii "   | | `---PC TX (SMS RX)       "
	.ascii "   | `-----Ground               "
	.ascii "   `-------PC RX (SMS TX)       "
	.db 0x00
	
	;; Initialize
init:
	JP vdp_init
vdp_init_ret::
	;Show some info
	puts #txt_hello
	;And a nice countdown
	delay #0x0300
	putc #0x33
	putc #0x20
	delay #0x0300
	putc #0x32
	putc #0x20
	delay #0x0300
	putc #0x31
	putc #0x20
	delay #0x0300
	
endloop:
	JP endloop
	
	;End of file
	.ascii "uwu "
	