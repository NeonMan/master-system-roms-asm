
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

txt_countdown_padding:
	.ascii "             "
	.db 0x00
	
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
	.ascii "                                "
	.ascii "Starting XMODEM transmission.   "
	.ascii "Check docs for connection and   "
	.ascii "Usage instructions.             "
	.db 0x00
txt_syncserial:
	.ascii "Protocol barely defined so...   "
	.ascii "ToDo."
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
	puts #txt_countdown_padding
	
	;Read control port 1
	IN A, (#0xDC)
	AND A, #0x20 ;Mask button 2
	;If 1 or 2 are pressed jump to SyncSerial
	JR Z, use_syncserial
	
	;Use UART
	puts #txt_uart
	;Jump to XMODEM loader (ToDo)
	JR endloop
	
	;Use SyncSerial
use_syncserial:
	puts #txt_syncserial
	JR endloop
	
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
	