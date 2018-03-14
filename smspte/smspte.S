
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
	JP asm_main

	.area _CODE (REL)
	.ascii "/smspte.S"
	;.include /macros.inc/

color_line:
	.db 0x20
	.db 0x80, 0x80, 0x80, 0x80 
	.db 0x81, 0x81, 0x81, 0x81 
	.db 0x82, 0x82, 0x82, 0x82 
	.db 0x83, 0x83, 0x83, 0x83 
	.db 0x84, 0x84, 0x84, 0x84 
	.db 0x85, 0x85, 0x85, 0x85 
	.db 0x86, 0x86, 0x86, 0x86 
	.db 0x20, 0x20, 0x20
	.db 0x00


asm_main:
	CALL vdp_init
	LD SP, #0xDFFF
	
	;Print the 7 color bars block.
	LD D, #0x18
	screen1_loop:
		LD HL, #color_line
		line_colors_loop:
			LD A, (HL)
			INC HL
			AND A, A
			JR Z, line_end
			LD B, #0x01
			CALL putc
			JR line_colors_loop
		line_end:
		DEC D
		LD A, D
		AND A, A
		JR NZ, screen1_loop
	
	
	
endloop:
	JP endloop

; ---------------------
; --- VDP functions ---
; ---------------------

;Small delay between VDP writes.
vdp_delay:
	LD A, #0x0F
	delay:
		DEC A
		JP NZ, delay
	RET

;Writes a single char.
putc:
	SUB A, #0x20
	OUT (#0xBE), A
	CALL vdp_delay
	LD A, B
	OUT (#0xBE), A
	LD A, #0x0F
	CALL vdp_delay
	RET
	
; --------------------------
; --- VDP initialization ---
; --------------------------
	
initial_registers:
	.db 0b00100100, 0x80
	.db 0b10000000, 0x81
	.db 15		, 0x82
	.db 127	   , 0x85
	.db 0		 , 0x86
	.db 0x00, 0x87
	.db 0x08, 0x88
	.db 0x00, 0x89
	.db 0xff, 0x8a
initial_registers_end:

initial_palette:
	;   BLK  BLU  GRE  RED  CYA  MAG  YEL
	.db 0x00,0x20,0x08,0x02,0x28,0x22,0x0a,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x2a
	.db 0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x07
initial_palette_end:

;Used registers: AF, BC, HL
vdp_init::
	;Setup registers
	LD HL, #initial_registers
	LD B, #initial_registers_end-initial_registers
	reg_loop:
		LD A, (HL)
		OUT (#0xBF), A
		INC HL
		DEC B
		JP NZ, reg_loop
	
	;Clear VRAM
	LD HL, #0x4000
	XOR A, A
	clear_loop:
		OUT (#0xBE), A
		DEC HL
		CP A, H
		JP NZ, clear_loop
		CP A, L
		JP NZ, clear_loop
	
	;Set VRAM address
	LD A, #0x00
	OUT (#0xBF), A
	LD A, #0x60
	OUT (#0xBF), A
	
	;Load font
	LD HL, #font_data
	LD BC, #font_data_end
	font_loop:
		LD A, (HL)
		OUT (#0xBE), A
		OUT (#0xBE), A
		OUT (#0xBE), A
		OUT (#0xBE), A
		INC HL
		LD A, H
		CP A, B
		JP NZ, font_loop
		LD A, L
		CP A, C
		JP NZ, font_loop
	
	;Load the color blocks
	LD HL, #colorblock
	LD BC, #colorblock_end
	
	colorblock_loop:
		LD A, (HL)
		OUT(#0xBE), A
		INC HL
		LD A, H
		CP A, B
		JP NZ, colorblock_loop
		LD A, L
		CP A, C
		JP NZ, colorblock_loop
	
	;Set VRAM address
	LD A, #0x00
	OUT (#0xBF), A
	LD A, #0xc0
	OUT (#0xBF), A
	
	;Load palette
	LD HL, #initial_palette
	LD B, #initial_palette_end-initial_palette
	pal_loop:
		LD A, (HL)
		OUT (#0xBE), A
		INC HL
		DEC B
		JP NZ, pal_loop
	
	;Set screen on
	LD A, #0b11000000
	OUT (#0xBF), A
	LD A, #0x81
	OUT (#0xBF), A
	
	;Prepare VRAM to receive chars
	LD A, #0x00
	OUT (#0xBF), A
	LD A, #0x78
	OUT (#0xBF), A
	
	;Go back
	RET

; -----------------
; --- SDSC Info ---
; -----------------

	;SDSC info
sdsc_author::
	.ascii "Heavydeck <root@heavydeck.net>"
	.db 0x00

sdsc_name::
	.ascii "SMS version of the SMTPE colorbars."
	.db 0x00

sdsc_description::
	.ascii "Needing an easily recognizable video test pattern on a "
	.ascii "scope and finding no ROM doing it, I decided to do mine."
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
