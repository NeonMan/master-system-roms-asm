
	;If A is 1 by the time this macro is used, it will calculate
	;the CRC by using the value at crc_table[index] (16bit)
	;Per macro call 10 Bytes will be used.
	.macro crc16_test_byte, table_index, ?skip_byte
		;If A is zero, skip.
		DEC A
		JR NZ, skip_byte
		LD A, L
		LD HL, (#crc_table + (2 * table_index))
		XOR A, H
		LD H, A
		XOR A, A
		skip_byte:
	.endm

	;Current CRC on HL
	;Next byte on A
	;Resulting CRC in HL
	;
	;Invoking this macro will use ~2K of code.
	;Not to mention it will do n*256 tests for n bytes.
	;Yeah, this gona be a bit slow...
	.macro crc16_xmodem
		;Calculate next index using A
		;tbl_idx = ((crc >> 8) ^ *d) & 0xff;
		XOR A, H  ;Index stored on A
		
		;Calculate next CRC
		;crc = (crc_table[tbl_idx] ^ (crc << 8));
		
		;Each macro call decrements A til it finds the correct
		;table index without using any more regs :3
		crc16_test_byte 1
		crc16_test_byte 2
		crc16_test_byte 3
		crc16_test_byte 4
		crc16_test_byte 5
		crc16_test_byte 6
		crc16_test_byte 7
		crc16_test_byte 8
		crc16_test_byte 9
		crc16_test_byte 10
		crc16_test_byte 11
		crc16_test_byte 12
		crc16_test_byte 13
		crc16_test_byte 14
		crc16_test_byte 15
		crc16_test_byte 16
		crc16_test_byte 17
		crc16_test_byte 18
		crc16_test_byte 19
		crc16_test_byte 20
		crc16_test_byte 21
		crc16_test_byte 22
		crc16_test_byte 23
		crc16_test_byte 24
		crc16_test_byte 25
		crc16_test_byte 26
		crc16_test_byte 27
		crc16_test_byte 28
		crc16_test_byte 29
		crc16_test_byte 30
		crc16_test_byte 31
		crc16_test_byte 32
		crc16_test_byte 33
		crc16_test_byte 34
		crc16_test_byte 35
		crc16_test_byte 36
		crc16_test_byte 37
		crc16_test_byte 38
		crc16_test_byte 39
		crc16_test_byte 40
		crc16_test_byte 41
		crc16_test_byte 42
		crc16_test_byte 43
		crc16_test_byte 44
		crc16_test_byte 45
		crc16_test_byte 46
		crc16_test_byte 47
		crc16_test_byte 48
		crc16_test_byte 49
		crc16_test_byte 50
		crc16_test_byte 51
		crc16_test_byte 52
		crc16_test_byte 53
		crc16_test_byte 54
		crc16_test_byte 55
		crc16_test_byte 56
		crc16_test_byte 57
		crc16_test_byte 58
		crc16_test_byte 59
		crc16_test_byte 60
		crc16_test_byte 61
		crc16_test_byte 62
		crc16_test_byte 63
		crc16_test_byte 64
		crc16_test_byte 65
		crc16_test_byte 66
		crc16_test_byte 67
		crc16_test_byte 68
		crc16_test_byte 69
		crc16_test_byte 70
		crc16_test_byte 71
		crc16_test_byte 72
		crc16_test_byte 73
		crc16_test_byte 74
		crc16_test_byte 75
		crc16_test_byte 76
		crc16_test_byte 77
		crc16_test_byte 78
		crc16_test_byte 79
		crc16_test_byte 80
		crc16_test_byte 81
		crc16_test_byte 82
		crc16_test_byte 83
		crc16_test_byte 84
		crc16_test_byte 85
		crc16_test_byte 86
		crc16_test_byte 87
		crc16_test_byte 88
		crc16_test_byte 89
		crc16_test_byte 90
		crc16_test_byte 91
		crc16_test_byte 92
		crc16_test_byte 93
		crc16_test_byte 94
		crc16_test_byte 95
		crc16_test_byte 96
		crc16_test_byte 97
		crc16_test_byte 98
		crc16_test_byte 99
		crc16_test_byte 100
		crc16_test_byte 101
		crc16_test_byte 102
		crc16_test_byte 103
		crc16_test_byte 104
		crc16_test_byte 105
		crc16_test_byte 106
		crc16_test_byte 107
		crc16_test_byte 108
		crc16_test_byte 109
		crc16_test_byte 110
		crc16_test_byte 111
		crc16_test_byte 112
		crc16_test_byte 113
		crc16_test_byte 114
		crc16_test_byte 115
		crc16_test_byte 116
		crc16_test_byte 117
		crc16_test_byte 118
		crc16_test_byte 119
		crc16_test_byte 120
		crc16_test_byte 121
		crc16_test_byte 122
		crc16_test_byte 123
		crc16_test_byte 124
		crc16_test_byte 125
		crc16_test_byte 126
		crc16_test_byte 127
		crc16_test_byte 128
		crc16_test_byte 129
		crc16_test_byte 130
		crc16_test_byte 131
		crc16_test_byte 132
		crc16_test_byte 133
		crc16_test_byte 134
		crc16_test_byte 135
		crc16_test_byte 136
		crc16_test_byte 137
		crc16_test_byte 138
		crc16_test_byte 139
		crc16_test_byte 140
		crc16_test_byte 141
		crc16_test_byte 142
		crc16_test_byte 143
		crc16_test_byte 144
		crc16_test_byte 145
		crc16_test_byte 146
		crc16_test_byte 147
		crc16_test_byte 148
		crc16_test_byte 149
		crc16_test_byte 150
		crc16_test_byte 151
		crc16_test_byte 152
		crc16_test_byte 153
		crc16_test_byte 154
		crc16_test_byte 155
		crc16_test_byte 156
		crc16_test_byte 157
		crc16_test_byte 158
		crc16_test_byte 159
		crc16_test_byte 160
		crc16_test_byte 161
		crc16_test_byte 162
		crc16_test_byte 163
		crc16_test_byte 164
		crc16_test_byte 165
		crc16_test_byte 166
		crc16_test_byte 167
		crc16_test_byte 168
		crc16_test_byte 169
		crc16_test_byte 170
		crc16_test_byte 171
		crc16_test_byte 172
		crc16_test_byte 173
		crc16_test_byte 174
		crc16_test_byte 175
		crc16_test_byte 176
		crc16_test_byte 177
		crc16_test_byte 178
		crc16_test_byte 179
		crc16_test_byte 180
		crc16_test_byte 181
		crc16_test_byte 182
		crc16_test_byte 183
		crc16_test_byte 184
		crc16_test_byte 185
		crc16_test_byte 186
		crc16_test_byte 187
		crc16_test_byte 188
		crc16_test_byte 189
		crc16_test_byte 190
		crc16_test_byte 191
		crc16_test_byte 192
		crc16_test_byte 193
		crc16_test_byte 194
		crc16_test_byte 195
		crc16_test_byte 196
		crc16_test_byte 197
		crc16_test_byte 198
		crc16_test_byte 199
		crc16_test_byte 200
		crc16_test_byte 201
		crc16_test_byte 202
		crc16_test_byte 203
		crc16_test_byte 204
		crc16_test_byte 205
		crc16_test_byte 206
		crc16_test_byte 207
		crc16_test_byte 208
		crc16_test_byte 209
		crc16_test_byte 210
		crc16_test_byte 211
		crc16_test_byte 212
		crc16_test_byte 213
		crc16_test_byte 214
		crc16_test_byte 215
		crc16_test_byte 216
		crc16_test_byte 217
		crc16_test_byte 218
		crc16_test_byte 219
		crc16_test_byte 220
		crc16_test_byte 221
		crc16_test_byte 222
		crc16_test_byte 223
		crc16_test_byte 224
		crc16_test_byte 225
		crc16_test_byte 226
		crc16_test_byte 227
		crc16_test_byte 228
		crc16_test_byte 229
		crc16_test_byte 230
		crc16_test_byte 231
		crc16_test_byte 232
		crc16_test_byte 233
		crc16_test_byte 234
		crc16_test_byte 235
		crc16_test_byte 236
		crc16_test_byte 237
		crc16_test_byte 238
		crc16_test_byte 239
		crc16_test_byte 240
		crc16_test_byte 241
		crc16_test_byte 242
		crc16_test_byte 243
		crc16_test_byte 244
		crc16_test_byte 245
		crc16_test_byte 246
		crc16_test_byte 247
		crc16_test_byte 248
		crc16_test_byte 249
		crc16_test_byte 250
		crc16_test_byte 251
		crc16_test_byte 252
		crc16_test_byte 253
		crc16_test_byte 254
		crc16_test_byte 255
		crc16_test_byte 0
	.endm


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

    ;Same thing. Byte on A, CRC on HL
    .macro crc16_xmodem_alt
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