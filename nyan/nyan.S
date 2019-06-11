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
	;Restore VDP config (ToDo)

	; Restore CRAM
	LD A, #0x00
	OUT (#0xBF), A
	LD A, #0xC0
	OUT (#0xBF), A
	LD B, #32
	LD HL, #cram_data
	cram_loop:
		vdp_delay
		LD A, (HL)
		OUT (#0xBE), A
		INC HL
	DEC B
	JR NZ, cram_loop
	
	; --- Loop forever ---
	end_loop:
	JR end_loop

	cram_data:
	mnyan_palette
	cram_data_end:

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
