
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
	
	.macro set_vram_addr addr_h, addr_l
	
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