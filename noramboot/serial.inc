
	;Sends byte stored on A through serial port.
	;	Uses: AF
	.macro serial_write_inner
		;Dummy macro, yet.
	.endm
	
	
	;Reads byte from serial port.
	;	On Success:
	;		- Stores result on A
	;		- Carry flag is SET
	;	On error:
	;		- Error code on A
	;		- Carry flag is CLEARED
	;
	;	Error codes:
	;		0: Timeout
	;
	;	Uses: AF
	.macro serial_read_inner
		;Return timeout error. Dummy macro
		XOR A, A ; Clear A (timeuot) and clear carry.
	.endm
	
	;Simulates an XMODEM transmission.
	;
	;	Uses: AF
	;		HL' Number of packets
	;		 B' Current packet
	;		 C' Current byte
	.macro serial_read_sim
		EXX
		
		EXX
		AND A, A  ;Clear C
		CCF       ;Set C (success)
	.endm
	
	;Prepares shadow registers for use with serial_read_sim
	.macro serial_setup_sim
		EXX
		LD HL, #0x10
		XOR A, A
		LD B, A
		LD C, A
		EXX
	.endm
	
	
	;Actual macros that will be called
	.macro serial_setup
		;serial_setup
		serial_setup_sim
	.endm
	
	.macro serial_write
		serial_write_inner
	.endm
	
	.macro serial_read
		;serial_read_inner
		serial_read_sim
	.endm