	.include /font_blob.inc/
	.include /vdp.inc/

	.area    _HEADER (ABS)
	; --- VDP blob ---
;	.org #0x0400
	.org #0x2000
	font
	.ascii "END TILES       "

	.org (#0x3800 - 16)
	.ascii "BEGIN TILEMAP   "


; --- Regular ROM addresses begin here ---
	.org #0xC000
	DI
	;Put registers into a known state
	LD SP, #0xDFF0 ;SP on RAM top

	;Restore VDP registers
	IN A, (#0xBF)
	LD HL, #vdp_regs
	LD B, #vdp_regs_end-vdp_regs
	reg_loop:
		LD A, (HL)
		OUT (#0xBF), A
		INC HL
	DEC B
	JP NZ, reg_loop

	; Restore CRAM
	LD A, #0x00
	OUT (#0xBF), A
	LD A, #0xC0
	OUT (#0xBF), A
	LD B, #cram_data_end-cram_data ;32Bytes
	LD HL, #cram_data
	cram_loop:
		LD A, (HL)
		OUT (#0xBE), A
		INC HL
	DEC B
	JR NZ, cram_loop

	; Enable VDP
	LD A, #0b11000000
	OUT (#0xBF), A
	LD A, #0x81
	OUT (#0xBF), A

	; ---------------------------
	; --- PROGRAM STARTS HERE ---
	; ---------------------------

	;Clear VRAM with spaces
	set_vram_addr #0x38, #0x00 ;<-- First text line
	LD BC, #(32*28)  ;28 lines at 32 chars
	clear_loop:
		putc #0x20
		DEC BC
	LD A, B
	OR A, C
	JR NZ, clear_loop

	LD HL, #0x0000
	CALL setcur_f
	LD HL, #text_splash
	CALL puts_f

	;Get Vendor ID and Device ID
	;See 39SF020 datasheet

	;Get real data at 0x0000 0x0001 and store on stack
	LD HL, #0x0000
	LD C, (HL)
	INC HL
	LD B, (HL)
	PUSH BC

	;Enter Software ID mode
	LD HL, #0x5555
	LD A, #0xAA
	LD (HL), A
	LD HL, #0x2AAA
	LD A, #0x55
	LD (HL), A
	LD HL, #0x5555
	LD A, #0x90
	LD (HL), A

	;Read Vendor ID
	LD HL, #0x0000
	LD E, (HL)
	;Read Device ID
	INC HL
	LD D, (HL)

	;Print Vendor ID
	LD HL, #0x0200
	CALL setcur_f
	LD HL, #text_vendor_id
	CALL puts_f
	LD A, E
	CALL putb_f

	;Print Device ID
	LD HL, #0x0300
	CALL setcur_f
	LD HL, #text_device_id
	CALL puts_f
	LD A, D
	CALL putb_f

	;Exit Software ID mode
	LD (HL), #0xF0

	;Check if the ROM is potentially not writeable/emulator
	AND A, A   ;Clear C
	POP HL     ;Get stored ROM data @0x0000 word
	SBC HL, DE
	LD  A, H
	OR  A, L
	JR NZ, skip_id_warning ;If match, show warning
		LD HL, #0x0500
		CALL setcur_f
		LD HL, #text_id_warning
		CALL puts_f
	skip_id_warning:

	;Loop forever
	end_loop:
	JR end_loop

	; --- Functions ---

	;Print a Hex byte
	; - A -- A byte.
	putb_f:
		PUSH AF
		LD B, #0x00
		RRA
		RRA
		RRA
		RRA
		AND A, #0x0F
		LD HL, #hex_lut
		LD C, A
		ADD HL, BC
		LD A, (HL)
		CALL putc_f
		POP AF
		AND A, #0x0F
		LD HL, #hex_lut
		LD C, A
		ADD HL, BC
		LD A, (HL)
		CALL putc_f
	RET
	hex_lut:
	.ascii "0123456789ABCDEF"

	;Print a C string
	; - HL -- String address
	puts_f:
		LD A, (HL)
		AND A, A
		JR Z, puts_end
		putc A
		INC HL
		JR puts_f
		puts_end:
	RET

	putc_f:
		putc A
	RET

	;position the cursor
	; - H -- Row    [0, 24]
	; - L -- Column [0, 31]
	setcur_f:
		;64 bytes per row
		;2 bytes per col
		;Base Addr: #0x3800
		IN A, (#VDP_CONTROL_PORT) ;Reset control
		;Shift L LEFT 3 bits
		RL L
		RL L
		RL L
		;Shift `H` two LSB into `L` MSB
		RR H
		RR L
		RR H
		RR L
		;Mask `L` and send to VDP
		LD A, L
		AND A, #0xFE
		OUT (#VDP_CONTROL_PORT), A
		;Mask `H` 3 LSB
		LD A, H
		AND A, #0x07
		;Add base address
		ADD A, #0x38
		;Add command
		OR A, #0x40
		OUT (#VDP_CONTROL_PORT), A
	RET


	; --- Data ---
;	.ascii "--------------------------------"
	text_id_warning:
	.ascii "Warn: Might NOT be a Flash IC   "
	.ascii "      Or Flash not writeable.   "
	.db 0x00
	text_vendor_id:
	.ascii "Vendor: "
	.db #0x00
	text_device_id:
	.ascii "Device: "
	.db #0x00
	text_splash:
	.ascii "Flash info dump           V0.1  "
	.db 0x00
	cram_data:
	.db 0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x3f
	.db 0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x07
	cram_data_end:

	vdp_regs:
	.db 0b00100100, 0x80
	.db 0b10000000, 0x81
	.db 15  , 0x82
	.db 127 , 0x85
	.db 0   , 0x86
	.db 0x00, 0x87
	.db 0x08, 0x88
	.db 0x00, 0x89
	.db 0xff, 0x8a
	vdp_regs_end:

; --- Static RAM (128B) ---
	.org #0xDE00
ram_base_00:
	.dw 0xAABB

; --- End of static RAM ---
	.org #0xDF00
	.ascii "*squeak! bawk!*"

; --- Space reserved for the Stack (~256B) ---
	.org #0xDFF0
	; Canary to prevent code from growing too large.
	.ascii "*chirp~ chirp~*"
	.db #0x00

; --- Nothing on _CODE ---
	.area _CODE (REL)

;Make the compiler quit complaining about _DATA
.area   _HOME
.area   _CODE
.area   _INITIALIZER
.area   _GSINIT
.area   _GSFINAL

.area   _DATA
.area   _INITIALIZED
.area   _BSEG
.area   _BSS
.area   _HEAP

.area   _CODE
