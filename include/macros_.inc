	;Set CRAM color (00bbggrr)
	;Used for error codes
	;Uses: A, B
	.macro clear_cram color, ?cram_loop
	LD A, #0x00
		OUT (#0xBF), A
		CP(HL)
		CP(HL)
		CP(HL)
		LD A, #0xC0
		OUT (#0xBF), A
		LD B, #32
		cram_loop:
			CP(HL)
			CP(HL)
			CP(HL)
			LD A, #color
			OUT (#0xBE), A
		DEC B
		JR NZ, cram_loop
	.endm
