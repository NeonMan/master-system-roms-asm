
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
	
	;; Entry point
	.org 0x0000
	DI
	JP init

	.area _CODE (REL)
	.ascii "/noramboot.S"
	.include /macros.inc/

txt_hello:
	.ascii "------ N0RAM Bootloader ------       "
	.db 0x00
	
	;; Initialize
init:
	JP vdp_init
vdp_init_ret::
	puts #txt_hello
	
	
endloop:
	JP endloop
	
	;End of file
	.ascii "uwu "
	