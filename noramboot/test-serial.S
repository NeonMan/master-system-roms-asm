
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
	.org 0x0066
	JR 0x0000
	
	;; Entry point
	.org 0x0000
	DI
	JP init

	.area _CODE (REL)
	.ascii "/test-crc16-xmodem.S"
	.include /macros.inc/
	.include /crc16-xmodem.inc/
	.include /serial.inc/

	
	
txt_hello:
	.ascii "------------------------------  "
	.ascii "--   UART serial port test  --  "
	.ascii "------------------------------  "
	.db 0x00

txt_dot:
	.ascii "."
	.db 0x00
	
txt_end:
	.ascii "!!! "
	.db 0x00


	;; Initialize
init:
	LD SP, #0xE000
	JP vdp_init
vdp_init_ret::

	
	;Bring line up
	;Pull line UP
	LD A, #UART_UP        ;  7T
	OUT (#IO_PORT),A      ; 11T
	
	;Show some info
	puts #txt_hello
	puts #txt_end	
	LD A, #0x55
	serial_write
	tx_bit_delay ;Add stop bit since we will send multiple Bytes.
	
	LD B, #0x00
test_all:
	LD A, B
	serial_write
	tx_bit_delay ;Add stop bit since we will send multiple Bytes.
	INC B
	JP NZ, test_all
	
	puts #txt_end
	
endloop:
	JP endloop
	
	;SDSC info
sdsc_author::
	.ascii "Heavydeck <root@heavydeck.net>"
	.db 0x00

sdsc_name::
	.ascii "Test the software serial port."
	.db 0x00

sdsc_description::
	.ascii "Just that."
	.db 0x00
	;End of file
	.ascii "uwu "
	

;Make the compiler quit complaining about _DATA
.area   _HOME
.area   _CODE
.area   _INITIALIZER
.area   _GSINIT
.area   _GSFINAL

.area   _DATA
.area   _INITIALIZED
.area   _BSEG
.area   _BSS
.area   _HEAP

.area   _CODE
