
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

txt_hello:
	.ascii "0123456789"
txt_hello_end:
	
	;; Initialize
init:
	JP vdp_init
vdp_init_ret::
	
	;;Say hello
	LD HL, #txt_hello
	LD B, #txt_hello_end-txt_hello
loop_hello:
	LD A, (HL)
	SUB A, #0x20
	OUT (#0xBE), A
	LD A, #0x01
	OUT (#0xBE), A
	INC HL
	DEC B
	JP NZ, loop_hello
	
	
endloop:
	JP endloop
	
	;End of file
	.ascii "uwu "
	