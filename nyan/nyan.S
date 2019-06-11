	.include /nyan_anim.inc/

	.area    _HEADER (ABS)
	; --- VDP blob ---
        .org #0x0000
	.ascii "TILES           "
	mnyan_tiles

	.ascii "TILEMAP         "
	;Add org here
	mnyan_tilemap

; --- Regular ROM addresses begin here ---
	.org #0xC000
	DI
	
	
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
