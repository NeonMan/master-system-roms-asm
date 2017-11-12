
	;; Imported functions

	.area	_HEADER (ABS)
	
	;; SDSC ROM header
	.org 0x7fe0
	.ascii "SDSC"
	.db 0xff,0xff			; Version (BCD)
	.db 0xff,0xff,0xff,0xff	; Date DD/MM/YYYY (BCD)
	.dw #sdsc_author		; Author pointer
	.dw #sdsc_name			; Name pointer
	.dw #sdsc_description	; Description pointer
	
	;; SEGA header
	.ascii "TMR SEGA"
	.db 0xFF, 0xFF			;Reserved bytes
	.db 0xFF, 0xFF			;Checksum
	.db 0xFF, 0xFF, 0xFF	;Product code and version
	.db 0x6c				;Export SEGA, 32K.
	
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
	putc #0x33 ;3
	putc #0x20
	delay #0x0300
	putc #0x32 ;2
	putc #0x20
	delay #0x0300
	putc #0x31 ;1
	putc #0x20
	delay #0x0300
	
endloop:
	JP endloop
	
	;SDSC info
sdsc_author::
	.ascii "Heavydeck <root@heavydeck.net>"
	.db 0x00

sdsc_name::
	.ascii "N0RAM Bootloader for Master System"
	.db 0x00

sdsc_description::
	.ascii "Put a proper description when the program is complete and "
	.ascii "free rom is not an issue."
	.db 0x00
	
	;End of file
	.ascii "uwu "
	