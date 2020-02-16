	.include /nyan_anim.inc/
	.include /vdp.inc/

	.area    _HEADER (ABS)
	; --- VDP blob ---
        .org #0x0000
	mnyan_tiles
	.ascii "END TILES       "

	.org (#0x3800 - 16)
	.ascii "BEGIN TILEMAP   "
	mnyan_tilemap


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

	; --- Loop forever ---
	end_loop:
	JR end_loop

	cram_data:
	.db #0x00, #0x10, #0x2A, #0x2B, #0x38, #0x0C, #0x23, #0x03, #0x31, #0x2F, #0x3F, #0x0F, #0x3B, #0x0B, #0x10, #0x10
	.db #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10, #0x10
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

; --- The last bytes of the ROM ---
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
