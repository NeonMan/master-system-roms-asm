	
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

	;Sets Carry flag if current packet is to be
	;written on VDP VRAM instead of RAM
	.macro is_vdp, ?test_fail, ?test_end
		;Check if this packet is 64 or below
		LD A, C
		CP A, #65           ; IF A <= 64, it is a RAM packet
		JP C, test_fail     ; So we return NCarry

		;Otherwise, fall down and return Carry
		AND A, A
		CCF
		JR test_end
	test_fail:
		AND A, A
	test_end:
	.endm

VDP_CONTROL_PORT = 0xBF
VDP_DATA_PORT    = 0xBE

;Sets VDP address for a given XMODEM packet.
;Args:
;  - packet  Packet index (Max val: 127)
;  - cmd     Command (Valid cases: 0x00, 0x40, 0x80, 0xC0)
;Uses: AF
.macro set_vram_packet_addr packet, cmd
	IN A, (#VDP_CONTROL_PORT)  ;Clear VDP

	;Command Low byte
	LD A, packet
	SRL A          ; LSB in C
	LD A, #0x00    ; Clear A
	RRA            ; Put packet LSB in A's MSB
	OUT (#VDP_CONTROL_PORT), A ; Send low byte

	;command High byte
	LD A, packet
	SRL A
	;AND A, #0x3F
	OR A, cmd
	OUT (#VDP_CONTROL_PORT), A
.endm
