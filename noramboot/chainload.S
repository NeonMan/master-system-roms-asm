;
;Chainloader code. Used to boot other media when Bootloader fails.
;  When called, it will look for a valid ROM by counting differences and
;  checking wether two reads of the same memory yield the same CRC. If
;  successful. it will check if the detected ROM is the loader itself and
;  if not, boot it.
;
;  Pressing the adequate button on the D-Pad will make the loader
;  boot without any checks a particular ROM media.
;
;                   [U] <---- [BIOS]
;      [Card] --> [L] [R] <-- [Cartridge]
;  [Ext Port] ----> [D]
;

	.include /crc16-xmodem.inc/
	.include /vdp.inc/

	;Will set Carry flag if two SLOT1 Reads were the same
	;Uses: AF, HL, BC, DE
	;
	.macro test_read, ?crc_loop_first, ?crc_loop_second, ?crc_bit_loop_first, ?crc_bit_loop_second, ?rv_err, ?rv_end
		LD HL, #0x0000
		LD BC, #0x0000 ; Test 256B (B=0)
		LD DE, #0x0000 ; Initial CRC
		crc_loop_first:
			LD A, (HL)
			EX DE, HL
			LD C, #8
			crc_bit_loop_first:
				crc16_xmodem_bit
			DEC C
			JR NZ, crc_bit_loop_first
			EX DE, HL
		DEC B
		JR NZ, crc_loop_first

		;CRC on DE, push it to the stack
		PUSH DE

		;Repeat
		LD HL, #0x0000
		LD BC, #0x0000 ; Test 256B (B=0)
		LD DE, #0x0000 ; Initial CRC
		crc_loop_second:
			LD A, (HL)
			EX DE, HL
			LD C, #8
			crc_bit_loop_second:
				crc16_xmodem_bit
			DEC C
			JR NZ, crc_bit_loop_second
			EX DE, HL
		DEC B
		JR NZ, crc_loop_second

		;Second CRC on DE
		;First CRC on Stack
		;Compare them
		POP HL
		LD A, H
		CP A, D
		JR NZ, rv_err
		LD A, L
		CP A, E
		JR NZ, rv_err

		AND A, A
		CCF
		JR rv_end
		rv_err:
		AND A, A
		rv_end:
	.endm

	;Will set Carry flag if a loader has been detected.
	;SP must point to signature.
	.macro test_loader, ?test_loop, ?rv_err, ?rv_end
		LD HL, #0x0000
		ADD HL, SP
		LD DE, #chainloader_signature
		LD B, #chainloader_signature_end - chainloader_signature
		test_loop:
			LD A, (DE)
			CP A, (HL)
			JR NZ, rv_err
		DEC B
		JR NZ, test_loop
		AND A, A
		CCF
		JR rv_end
		rv_err:
		AND A, A
		rv_end:
	.endm

	.area	_CODE (REL)
	.ascii "<chainload.S>"

	;Constants for PORT #0x3E
	ENABLE_BIOS      = 0b11100011
	ENABLE_EXT       = 0b01101011
	ENABLE_CARD      = 0b11001011
	ENABLE_CARTRIDGE = 0b10101011

	;Chainloader signature
	;The CRC of the following 16 bytes must be Zero. Used to
	;detect that this is the loader's ROM during automatic load.
	;Also, if using some more memory, this could be passed on the stack.
	chainloader_signature:
	.db #0xD2, #0x85, #0x73, #0x80, #0x2C, #0x15, #0xD9, #0x2E
	.db #0x02, #0x21, #0xF0, #0x78, #0x59, #0x39, #0xAC, #0xD0
	chainloader_signature_end:

	chainload::
	; --- Copy loader_blob to stack via SP ---
	LD BC, #loader_blob_end - loader_blob
	LD HL, #loader_blob_end - 1
	blob_copy_loop:
		LD A, (HL)
		PUSH AF
		INC SP
		DEC HL
		DEC BC
	LD A, B
	OR A, C
	JR NZ, blob_copy_loop
	; --- End blob copy ---

	;SP now contains the Address of the boot blob.
	;Store it on DE.
	LD HL, #0x0000
	ADD HL, SP
	EX DE, HL

;	; --- Copy loader blob below #0xD000
;	LD DE, #0xCFFF
;	LD HL, #loader_blob_end - 1
;	LD BC, #loader_blob_end - loader_blob
;	LDDR
;	INC DE ;Adjust pointer by one byte

	; --- Copy signature on stack via SP ---
	LD B, #chainloader_signature_end - chainloader_signature
	LD HL, #chainloader_signature_end - 1
	sig_copy_loop:
		LD A, (HL)
		PUSH AF
		INC SP
		DEC HL
		DEC B
	JR NZ, sig_copy_loop
	; --- End signature copy ---

	;By this time, our stack has grown by chainloader_signature size plus loader_blob size
	;And DE contains the address of the loader_blob
	;
	;Stack looks like this:
	;
	; SP -> [ Loader signature ]
	; DE -> [ Loader boot blob ]
	;       [ Return Address   ]

	;Jump to loader blob
;	JP loader_blob ; Uncomment for testing on Emulicious debugger
	PUSH DE   ; Put loader address on the stack
	RET       ; "Jump" to loader address.

	blob_return:
	;We need to fix the stack to return back to N0RAM, so we perform SP <= SP - (All the stuff in the stack)
	LD HL, #((loader_blob_end - loader_blob) + (chainloader_signature_end - chainloader_signature))
	ADD HL, SP
	LD SP, HL
	RET

	; ------------------------
	; --- Chainloader blob ---
	; ------------------------
	;
	;This code must run from RAM. All memory access relative to stack.
	;  The stack looks like this:
	; SP -> [ Loader signature ]
	loader_blob:
	;First, check if we are skipping autodetection because th
	;user is pressing the CONTROL 1 D-Pad
	IN A, (#0xDC) ;
	AND A, #0x0F  ;
	LD B, A       ; Pad status on `B`
	; Test [UP]
	LD  A, #0x01
	AND A, B
	JR  Z, load_bios
	; Test [DOWN]
	LD  A, #0x02
	AND A, B
	JR  Z, load_external
	; Test [LEFT]
	LD  A, #0x04
	AND A, B
	JR  Z, load_card
	; Test [RIGHT]
	LD  A, #0x08
	AND A, B
	JR  Z, load_cartridge
	;Otherwise, use autodetect
	JR load_auto

	;Direct loaders
	load_bios:
	LD A, #ENABLE_BIOS
	OUT (#0x3E), A
	JR write_info_and_reset
	load_external:
	LD A, #ENABLE_EXT
	OUT (#0x3E), A
	JR write_info_and_reset
	load_card:
	LD A, #ENABLE_CARD
	OUT (#0x3E), A
	JR write_info_and_reset
	load_cartridge:
	LD A, #ENABLE_CARTRIDGE
	OUT (#0x3E), A
	write_info_and_reset:
	;ToDo Write BIOS info for comaptibility info
	JP 0x0000 ;And reset console

	;Media autodetection.
	;Test, in this order if it contains a non-loader ROM
	;  * EXT Port
	;  * Card
	;  * Cartridge
	load_auto:

	;EXT port load
	LD A, #ENABLE_EXT
	OUT (#0x3E), A
	test_read
	JR NC, skip_ext
	test_loader
	JR C, skip_ext
	JP 0x0000
	skip_ext:

	;CARD load
	LD A, #ENABLE_CARD
	OUT (#0x3E), A
	test_read
	JR NC, skip_card
	test_loader
	JR C, skip_card
	JP 0x0000
	skip_card:

	;CARTRIDGE port load
	LD A, #ENABLE_CARTRIDGE
	OUT (#0x3E), A
	test_read
	JR NC, skip_cartridge
	test_loader
	JR C, skip_cartridge
	JP 0x0000
	skip_cartridge:

	;If everything fails, Go back to ROM
	;But first we need to find where in the world is Carm... The bootloader

	;EXT loader test
	LD A, #ENABLE_EXT
	OUT (#0x3E), A
	test_loader
	JP C, blob_return

	;CARD loader test
	LD A, #ENABLE_CARD
	OUT (#0x3E), A
	test_loader
	JP C, blob_return

	;CARTRIDGE loader test
	LD A, #ENABLE_CARTRIDGE
	OUT (#0x3E), A
	test_loader
	JP C, blob_return

	;BIOS loader test
	LD A, #ENABLE_BIOS
	OUT (#0x3E), A
	test_loader
	JP C, blob_return

	;If somehow we get here, something very bad happened. Do a BSOD
	;and Loop forever
	clear_cram #0x30
	chain_error_loop:
	JR chain_error_loop

	loader_blob_end:

	.ascii "</chainload.S>"
