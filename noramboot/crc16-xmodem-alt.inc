;CRC implementation with large ROM usage but Zero register usage.

	;CRC table, words.
	.macro crc16_word_table
		.dw 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7
		.dw 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
		.dw 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6
		.dw 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de
		.dw 0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485
		.dw 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d
		.dw 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4
		.dw 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc
		.dw 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823
		.dw 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b
		.dw 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12
		.dw 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a
		.dw 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41
		.dw 0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49
		.dw 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70
		.dw 0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78
		.dw 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f
		.dw 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067
		.dw 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e
		.dw 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256
		.dw 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d
		.dw 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405
		.dw 0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c
		.dw 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634
		.dw 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab
		.dw 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3
		.dw 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a
		.dw 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92
		.dw 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9
		.dw 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1
		.dw 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8
		.dw 0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	.endm

	;If A is 1 by the time this macro is used, it will calculate
	;the CRC by using the value at crc_table[index] (16bit)
	;Per macro call 10 Bytes will be used.
	.macro crc16_test_byte, table_index, crc_table, ?skip_byte
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
	.macro crc16_xmodem, ?crc_table, ?crc_table_end
		JP crc_table_end
		crc_table:
		crc16_word_table
		crc_table_end:
		
		;Calculate next index using A
		;tbl_idx = ((crc >> 8) ^ *d) & 0xff;
		XOR A, H  ;Index stored on A
		
		;Calculate next CRC
		;crc = (crc_table[tbl_idx] ^ (crc << 8));
		
		;Each macro call decrements A til it finds the correct
		;table index without using any more regs :3
		crc16_test_byte 1, crc_table
		crc16_test_byte 2, crc_table
		crc16_test_byte 3, crc_table
		crc16_test_byte 4, crc_table
		crc16_test_byte 5, crc_table
		crc16_test_byte 6, crc_table
		crc16_test_byte 7, crc_table
		crc16_test_byte 8, crc_table
		crc16_test_byte 9, crc_table
		crc16_test_byte 10, crc_table
		crc16_test_byte 11, crc_table
		crc16_test_byte 12, crc_table
		crc16_test_byte 13, crc_table
		crc16_test_byte 14, crc_table
		crc16_test_byte 15, crc_table
		crc16_test_byte 16, crc_table
		crc16_test_byte 17, crc_table
		crc16_test_byte 18, crc_table
		crc16_test_byte 19, crc_table
		crc16_test_byte 20, crc_table
		crc16_test_byte 21, crc_table
		crc16_test_byte 22, crc_table
		crc16_test_byte 23, crc_table
		crc16_test_byte 24, crc_table
		crc16_test_byte 25, crc_table
		crc16_test_byte 26, crc_table
		crc16_test_byte 27, crc_table
		crc16_test_byte 28, crc_table
		crc16_test_byte 29, crc_table
		crc16_test_byte 30, crc_table
		crc16_test_byte 31, crc_table
		crc16_test_byte 32, crc_table
		crc16_test_byte 33, crc_table
		crc16_test_byte 34, crc_table
		crc16_test_byte 35, crc_table
		crc16_test_byte 36, crc_table
		crc16_test_byte 37, crc_table
		crc16_test_byte 38, crc_table
		crc16_test_byte 39, crc_table
		crc16_test_byte 40, crc_table
		crc16_test_byte 41, crc_table
		crc16_test_byte 42, crc_table
		crc16_test_byte 43, crc_table
		crc16_test_byte 44, crc_table
		crc16_test_byte 45, crc_table
		crc16_test_byte 46, crc_table
		crc16_test_byte 47, crc_table
		crc16_test_byte 48, crc_table
		crc16_test_byte 49, crc_table
		crc16_test_byte 50, crc_table
		crc16_test_byte 51, crc_table
		crc16_test_byte 52, crc_table
		crc16_test_byte 53, crc_table
		crc16_test_byte 54, crc_table
		crc16_test_byte 55, crc_table
		crc16_test_byte 56, crc_table
		crc16_test_byte 57, crc_table
		crc16_test_byte 58, crc_table
		crc16_test_byte 59, crc_table
		crc16_test_byte 60, crc_table
		crc16_test_byte 61, crc_table
		crc16_test_byte 62, crc_table
		crc16_test_byte 63, crc_table
		crc16_test_byte 64, crc_table
		crc16_test_byte 65, crc_table
		crc16_test_byte 66, crc_table
		crc16_test_byte 67, crc_table
		crc16_test_byte 68, crc_table
		crc16_test_byte 69, crc_table
		crc16_test_byte 70, crc_table
		crc16_test_byte 71, crc_table
		crc16_test_byte 72, crc_table
		crc16_test_byte 73, crc_table
		crc16_test_byte 74, crc_table
		crc16_test_byte 75, crc_table
		crc16_test_byte 76, crc_table
		crc16_test_byte 77, crc_table
		crc16_test_byte 78, crc_table
		crc16_test_byte 79, crc_table
		crc16_test_byte 80, crc_table
		crc16_test_byte 81, crc_table
		crc16_test_byte 82, crc_table
		crc16_test_byte 83, crc_table
		crc16_test_byte 84, crc_table
		crc16_test_byte 85, crc_table
		crc16_test_byte 86, crc_table
		crc16_test_byte 87, crc_table
		crc16_test_byte 88, crc_table
		crc16_test_byte 89, crc_table
		crc16_test_byte 90, crc_table
		crc16_test_byte 91, crc_table
		crc16_test_byte 92, crc_table
		crc16_test_byte 93, crc_table
		crc16_test_byte 94, crc_table
		crc16_test_byte 95, crc_table
		crc16_test_byte 96, crc_table
		crc16_test_byte 97, crc_table
		crc16_test_byte 98, crc_table
		crc16_test_byte 99, crc_table
		crc16_test_byte 100, crc_table
		crc16_test_byte 101, crc_table
		crc16_test_byte 102, crc_table
		crc16_test_byte 103, crc_table
		crc16_test_byte 104, crc_table
		crc16_test_byte 105, crc_table
		crc16_test_byte 106, crc_table
		crc16_test_byte 107, crc_table
		crc16_test_byte 108, crc_table
		crc16_test_byte 109, crc_table
		crc16_test_byte 110, crc_table
		crc16_test_byte 111, crc_table
		crc16_test_byte 112, crc_table
		crc16_test_byte 113, crc_table
		crc16_test_byte 114, crc_table
		crc16_test_byte 115, crc_table
		crc16_test_byte 116, crc_table
		crc16_test_byte 117, crc_table
		crc16_test_byte 118, crc_table
		crc16_test_byte 119, crc_table
		crc16_test_byte 120, crc_table
		crc16_test_byte 121, crc_table
		crc16_test_byte 122, crc_table
		crc16_test_byte 123, crc_table
		crc16_test_byte 124, crc_table
		crc16_test_byte 125, crc_table
		crc16_test_byte 126, crc_table
		crc16_test_byte 127, crc_table
		crc16_test_byte 128, crc_table
		crc16_test_byte 129, crc_table
		crc16_test_byte 130, crc_table
		crc16_test_byte 131, crc_table
		crc16_test_byte 132, crc_table
		crc16_test_byte 133, crc_table
		crc16_test_byte 134, crc_table
		crc16_test_byte 135, crc_table
		crc16_test_byte 136, crc_table
		crc16_test_byte 137, crc_table
		crc16_test_byte 138, crc_table
		crc16_test_byte 139, crc_table
		crc16_test_byte 140, crc_table
		crc16_test_byte 141, crc_table
		crc16_test_byte 142, crc_table
		crc16_test_byte 143, crc_table
		crc16_test_byte 144, crc_table
		crc16_test_byte 145, crc_table
		crc16_test_byte 146, crc_table
		crc16_test_byte 147, crc_table
		crc16_test_byte 148, crc_table
		crc16_test_byte 149, crc_table
		crc16_test_byte 150, crc_table
		crc16_test_byte 151, crc_table
		crc16_test_byte 152, crc_table
		crc16_test_byte 153, crc_table
		crc16_test_byte 154, crc_table
		crc16_test_byte 155, crc_table
		crc16_test_byte 156, crc_table
		crc16_test_byte 157, crc_table
		crc16_test_byte 158, crc_table
		crc16_test_byte 159, crc_table
		crc16_test_byte 160, crc_table
		crc16_test_byte 161, crc_table
		crc16_test_byte 162, crc_table
		crc16_test_byte 163, crc_table
		crc16_test_byte 164, crc_table
		crc16_test_byte 165, crc_table
		crc16_test_byte 166, crc_table
		crc16_test_byte 167, crc_table
		crc16_test_byte 168, crc_table
		crc16_test_byte 169, crc_table
		crc16_test_byte 170, crc_table
		crc16_test_byte 171, crc_table
		crc16_test_byte 172, crc_table
		crc16_test_byte 173, crc_table
		crc16_test_byte 174, crc_table
		crc16_test_byte 175, crc_table
		crc16_test_byte 176, crc_table
		crc16_test_byte 177, crc_table
		crc16_test_byte 178, crc_table
		crc16_test_byte 179, crc_table
		crc16_test_byte 180, crc_table
		crc16_test_byte 181, crc_table
		crc16_test_byte 182, crc_table
		crc16_test_byte 183, crc_table
		crc16_test_byte 184, crc_table
		crc16_test_byte 185, crc_table
		crc16_test_byte 186, crc_table
		crc16_test_byte 187, crc_table
		crc16_test_byte 188, crc_table
		crc16_test_byte 189, crc_table
		crc16_test_byte 190, crc_table
		crc16_test_byte 191, crc_table
		crc16_test_byte 192, crc_table
		crc16_test_byte 193, crc_table
		crc16_test_byte 194, crc_table
		crc16_test_byte 195, crc_table
		crc16_test_byte 196, crc_table
		crc16_test_byte 197, crc_table
		crc16_test_byte 198, crc_table
		crc16_test_byte 199, crc_table
		crc16_test_byte 200, crc_table
		crc16_test_byte 201, crc_table
		crc16_test_byte 202, crc_table
		crc16_test_byte 203, crc_table
		crc16_test_byte 204, crc_table
		crc16_test_byte 205, crc_table
		crc16_test_byte 206, crc_table
		crc16_test_byte 207, crc_table
		crc16_test_byte 208, crc_table
		crc16_test_byte 209, crc_table
		crc16_test_byte 210, crc_table
		crc16_test_byte 211, crc_table
		crc16_test_byte 212, crc_table
		crc16_test_byte 213, crc_table
		crc16_test_byte 214, crc_table
		crc16_test_byte 215, crc_table
		crc16_test_byte 216, crc_table
		crc16_test_byte 217, crc_table
		crc16_test_byte 218, crc_table
		crc16_test_byte 219, crc_table
		crc16_test_byte 220, crc_table
		crc16_test_byte 221, crc_table
		crc16_test_byte 222, crc_table
		crc16_test_byte 223, crc_table
		crc16_test_byte 224, crc_table
		crc16_test_byte 225, crc_table
		crc16_test_byte 226, crc_table
		crc16_test_byte 227, crc_table
		crc16_test_byte 228, crc_table
		crc16_test_byte 229, crc_table
		crc16_test_byte 230, crc_table
		crc16_test_byte 231, crc_table
		crc16_test_byte 232, crc_table
		crc16_test_byte 233, crc_table
		crc16_test_byte 234, crc_table
		crc16_test_byte 235, crc_table
		crc16_test_byte 236, crc_table
		crc16_test_byte 237, crc_table
		crc16_test_byte 238, crc_table
		crc16_test_byte 239, crc_table
		crc16_test_byte 240, crc_table
		crc16_test_byte 241, crc_table
		crc16_test_byte 242, crc_table
		crc16_test_byte 243, crc_table
		crc16_test_byte 244, crc_table
		crc16_test_byte 245, crc_table
		crc16_test_byte 246, crc_table
		crc16_test_byte 247, crc_table
		crc16_test_byte 248, crc_table
		crc16_test_byte 249, crc_table
		crc16_test_byte 250, crc_table
		crc16_test_byte 251, crc_table
		crc16_test_byte 252, crc_table
		crc16_test_byte 253, crc_table
		crc16_test_byte 254, crc_table
		crc16_test_byte 255, crc_table
		crc16_test_byte 0, crc_table
	.endm
