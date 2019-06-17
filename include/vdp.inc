VDP_CONTROL_PORT = 0xBF
VDP_DATA_PORT    = 0xBE

	;Small delay between VDP writes.
	.macro vdp_delay ?delay
	LD A, #0x02
	delay:
		DEC A
		JP NZ, delay
	.endm

	;Set CRAM color
	;Used for error codes
	;Uses: A, B
	.macro clear_cram color, ?cram_loop
		LD A, #0x00
		OUT (#0xBF), A
		vdp_delay
		LD A, #0xC0
		OUT (#0xBF), A
		LD B, #32
		cram_loop:
			vdp_delay
			LD A, #color
			OUT (#0xBE), A
		DEC B
		JR NZ, cram_loop
	.endm

	;Writes a single char.
	.macro putc ch
		LD A, ch
		SUB A, #0x20
		OUT (#0xBE), A
		vdp_delay
		LD A, #0x01
		OUT (#0xBE), A
		LD A, #0x0F
		vdp_delay
	.endm
	
	;Set VRAM address relative to 3800h
	;Line 1: 0x3800
	;Line 2: 0x3840
	;etc...
	.macro set_vram_addr addr_h, addr_l
		IN A, (#0xBF)
		LD A, addr_l
		OUT (#0xBF), A
		;vdp_delay
		LD A, addr_h
		AND A, #0x3F
		;OR A, #0x80
		OUT (#0xBF), A
		;vdp_delay
	.endm
	
	.macro gotoxy x, y
		;64 bytes per row
		;2 bytes per col
		;ToDo
	.endm
	
	;Prints a null-terminated string
	;Requires: HL
	.macro puts c_str, ?ch_loop, ?ch_end
		LD HL, c_str
	ch_loop:
		LD A, (HL)
		AND A, A
		;If null, end
		JR Z, ch_end
		;--Print the character--
		SUB A, #0x20
		OUT (#0xBE), A
		vdp_delay
		LD A, #0x01
		OUT (#0xBE), A
		vdp_delay
		INC HL
		JR ch_loop
	ch_end:
	.endm
