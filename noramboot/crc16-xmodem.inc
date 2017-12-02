	;Per-bit CRC update function.
	;For use by crc16_xmodem
	.macro crc16_xmodem_bit ?bit_set, ?bit_clr, ?end_bit, ?flip_bits
			;if (c & i)
			RLCA
			JR NC, bit_clr
			
			bit_set:
			AND A, A ;Clear C
			RL L  ;
			RL H  ; Shift HL left, store MSB on C
			JR C, end_bit ;if bit set, skip
			;if bit not set, HL = HL ^ 0x1021
			flip_bits:
			EX AF, AF'
				LD A, H
				XOR A, #0x10
				LD H, A
				LD A, L
				XOR A, #0x21
				LD L, A
			EX AF, AF'
			JR end_bit

			;else
			bit_clr:
			AND A, A ;Clear C
			RL L ;
			RL H ; Shift HL left, store MSB on C
			JR C, flip_bits ;If bit set, flip.
			;if not, skip

			end_bit:
	.endm

	;Current CRC on HL
	;Next byte on A
	;Resulting CRC in HL
	;
	;Uses: AF, HL, AF'
	;232 bytes total. Is also faster, but uses AF' in addition to AF and HL
	.macro crc16_xmodem
		;for (i = 0x80; i > 0; i >>= 1)  /*For each bit. MSB first, i is the mask*/

			crc16_xmodem_bit
			crc16_xmodem_bit
			crc16_xmodem_bit
			crc16_xmodem_bit

			crc16_xmodem_bit
			crc16_xmodem_bit
			crc16_xmodem_bit
			crc16_xmodem_bit

		;endfor
	.endm