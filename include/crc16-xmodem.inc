	;Per-bit CRC update function.
	;For use by crc16_xmodem
	;Size: 51 Bytes
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
			;XOR H with 0001 0000
			RL H ;Bit 7 ^ 0  (don'f flip)
			RL H ;Bit 6 ^ 0  (don'f flip)
			RL H ;Bit 5 ^ 0  (don'f flip)
			RL H ;Bit 4 ^ 1  (flip with CCF)
			CCF

			RL H ;Bit 3 ^ 0  (don'f flip)
			RL H ;Bit 2 ^ 0  (don'f flip)
			RL H ;Bit 1 ^ 0  (don'f flip)
			RL H ;Bit 0 ^ 0  (don'f flip)

			RL H ;Shift LSB stored in C

			;XOR L with 0010 0001
			RL L ;Bit 7 ^ 0 (don't flip)
			RL L ;Bit 6 ^ 0 (don't flip)
			RL L ;Bit 5 ^ 1 (flip with CCF)
			CCF
			RL L ;Bit 4 ^ 0 (don't flip)

			RL L ;Bit 3 ^ 0 (don't flip)
			RL L ;Bit 2 ^ 0 (don't flip)
			RL L ;Bit 1 ^ 0 (don't flip)
			RL L ;Bit 0 ^ 1 (flip with CCF)
			CCF

			RL L ;Shift LSB stored in C

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
	;Uses: AF, HL
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
