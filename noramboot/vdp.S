
	.area	_CODE (REL)
	.ascii "/vdp.S"
	
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
	.db 0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x3f
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
	JP vdp_init_ret

	;End of file mark
	.ascii "uwu "