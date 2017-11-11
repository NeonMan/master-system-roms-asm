	;configurable delay loop
	;Uses: HL
	.macro delay iterations, ?delay_loop, ?delay_inner
		LD HL, iterations
		delay_loop:
			XOR A, A
			delay_inner:
				DEC A
				JP NZ, delay_inner
			DEC HL
			LD A, H
			AND A, A
			JP NZ, delay_loop
			LD A, L
			AND A, A
			JP NZ, delay_loop
			XOR A, A
	.endm

	;Small delay between VDP writes.
	.macro vdp_delay ?delay
	LD A, #0x0F
	delay:
		DEC A
		JP NZ, delay
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
		LD A, addr_l
		OUT (#0xBE), A
		vdp_delay
		LD A, addr_h
		AND A, #0x07
		OR A, #0x78
		OUT (#0xBE), A
		vdp_delay
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