	
	; --- Utility macros
	
	; Clear carry flag
	.macro CLRC ?flag_clr
		JR NC, flag_clr
		CCF
		flag_clr:
	.endm
	
	; Set carry flag
	.macro SETC ?flag_set
		JR C, flag_set
		CCF
		flag_set:
	.endm
	
	; HL <- SP
	.macro ld_hl_sp
		LD HL, #0x00
		ADD HL, SP
	.endm

	; --- Accumulator <-> shadow register operations

	;A <- B'
	.macro ld_a_bp
		EXX
		LD A, B
		EXX
	.endm

	;B' <- A
	.macro ld_bp_a
		EX AF, AF'
		EXX
		LD A, B
		EXX
	.endm

	;A <- C'
	.macro ld_a_cp
		EXX
		LD A, C
		EXX
	.endm

	;C' <- A
	.macro ld_cp_a
		EXX
		LD C, A
		EXX
	.endm

	;A <- D'
	.macro ld_a_dp
		EXX
		LD A, D
		EXX
	.endm

	;D' <- A
	.macro ld_dp_a
		EXX
		LD D, A
		EXX
	.endm

	;A <- E'
	.macro ld_a_ep
		EXX
		LD A, E
		EXX
	.endm

	;E' <- A
	.macro ld_ep_a
		EXX
		LD E, A
		EXX
	.endm

	;A <- H'
	.macro ld_a_hp
		EXX
		LD A, H
		EXX
	.endm

	;H' <- A
	.macro ld_hp_a
		EXX
		LD H, A
		EXX
	.endm

	;A <- L'
	.macro ld_a_lp
		EXX
		LD A, L
		EXX
	.endm

	;L' <- A
	.macro ld_lp_a
		EXX
		LD L, A
		EXX
	.endm

	; --- Operations between A <-> IXH/IXL/IYH/IYL ---

	;A <- IXL
	.macro ld_a_ixl
		.db 0xDD
		LD A, L
	.endm
	
	;A <- IXH
	.macro ld_a_ixh
		.db 0xDD
		LD A, H
	.endm
	
	;A <- IYL
	.macro ld_a_iyl
		.db 0xFD
		LD A, L
	.endm
	
	;A <- IYH
	.macro ld_a_iyh
		.db 0xFD
		LD A, H
	.endm
	
	;IXL <- A
	.macro ld_ixl_a
		.db 0xDD
		LD L, A
	.endm
	
	;IXH <- A
	.macro ld_ixh_a
		.db 0xDD
		LD H, A
	.endm
	
	;IYL <- A
	.macro ld_iyl_a
		.db 0xFD
		LD L, A
	.endm
	
	;IYH <- A
	.macro ld_iyh_a
		.db 0xFD
		LD H, A
	.endm

	; --- Operations between HL <-> IX/IY ---

	;Load IX into HL
	.macro ld_hl_ix
		.db 0xDD ;
		LD A, H  ; LD A, IXH
		LD H, A
		.db 0xDD ;
		LD A, L  ; LD A, IHL
		LD L, A
	.endm
	
	;Load IY into HL
	.macro ld_hl_iy
		.db 0xFD ;
		LD A, H  ; LD A, IYH
		LD H, A
		.db 0xFD ;
		LD A, L  ; LD A, IYL
		LD L, A
	.endm
	
	;Load HL into IX
	.macro ld_ix_hl
		LD A, H
		.db 0xDD ;
		LD H, A  ; LD IXH, A
		LD A, L
		.db 0xDD ;
		LD L, A  ; LD IXL, A
	.endm
	
	;Load HL into IY
	.macro ld_iy_hl
		LD A, H
		.db 0xFD ;
		LD H, A  ; LD IYH, A
		LD A, L
		.db 0xFD ;
		LD L, A  ; LD IYL, A
	.endm
	

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
