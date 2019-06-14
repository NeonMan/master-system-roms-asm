;Bootloadable ROM
	.include /serial.inc/

	.area    _HEADER (ABS)
	; --- VDP blob ---
        .org #0x0000
	.ascii "No VDP blob"


; --- Regular ROM addresses begin here ---
	.org #0xC000
	DI
	LD SP, #0xDFF0
	;Send a single 'U' for calibration/hello
	LD A, #0x55
	CALL write

	echo_loop:
	CALL read
	JP NC, echo_loop
	CALL write
	JP echo_loop
	.ascii "END MAIN"

read:
	serial_read
	RET

write:
	serial_write
	tx_bit_delay
	RET

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
