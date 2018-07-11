	;XMODEM bootloader, the fun begins, all registers free.
	;
	;Reg A  is presumed dirty on pretty much _any_ macro call
	;Reg HL will be used thorougly for all-arround many calls
	;
	; Where relevant, carry flag will be set if a macro call succeeds.
	;
	; XMODEM state requires knowing:
	;  * Current packet [0 255]
	;  * Current byte   [0 128]
	;  * CRC or Checksum (1/2 Bytes)
	;  * Expected CRC or Checksum
	;
	; State machine:
	;                  /<---------------------------------------------------------+<----------------------+
	;                  |                                                          |                       |
	; >[  Init  ]---[reqest]-->[wait SOH]---yes------->[read idx]-+---incorrect---+                       |
	;                  |           |                       |      |                                       |
	;                  |<--timeout-/<-------timeout--------´      |                                       |
	;                  |                                          `---correct---->[read 128]-+--timeout---+
	;                  |                   /<-timeout--\                                     |
	;                  |                   |           |                                     |
	;                  |<-----[reset packet]<--fail----[check crc]<--------success-----------´
	;                  |                               |
	;                  \--[update packet]<---success---´
	;
	; Registers:
	;
	;  A	(General purpose)
	; BC	current Byte; current paCket.
	; DE
	; HL	(General purpose)
	;
	; IX	Current buffer address
	; IY	
	; SP	
	;
	;  A'   Let's use
	; BC'   these for
	; DE'   unit tests
	; HL'   >:3
	
	.area _CODE (REL)
	.ascii "/xmodem-boot.S"
	
	.include /macros.inc/
	.include /crc16-xmodem-alt.inc/
	.include /serial.inc/

	BYTE_SOH=0x01
	BYTE_EOT=0x04
	BYTE_CRC=0x43
	BYTE_ACK=0x06
	BYTE_NAK=0x15
	
	SMS_RAM_BASE_ADDRESS=0xC000
	
xmodem_boot::
	JP state_init
	
	;Check the marvelous state machine diagram, the dead trees version.
state_init:
	;Prepare variables for the first transfer.
	;Packet <= 1
	LD C, #0x01
	;Byte <= 0
	LD B, #0x00
	;D <= 0, E <= 0
	LD D, #0x00
	LD E, #0x00
	;Buffer <= RAM_BASE_ADDRESS
	LD HL, #SMS_RAM_BASE_ADDRESS
	ld_ix_hl  ;Store on IX.
	ld_iy_hl  ;And on IY.
	LD SP, HL ;And on SP.

	;Prepare serial.
	serial_setup
	;Say we are ready
	putc #0x21
	;Go to transfer
	JP state_start_transfer
	
state_start_transfer:
	;Request first packet, CRC mode
	LD A, #BYTE_CRC ;
	serial_write    ; Send 'C'
	JP state_first_soh
	
state_first_soh:
	;Wait for the start of transmission byte.
	;On success, continue, on timeout go back to start_transfer.
	
	serial_read
	JP NC, state_start_transfer ;On error, restart
	
	CP A, #BYTE_SOH
	JP NZ, state_start_transfer ;If not Start-of-transfer restart.
	
	JP state_read_id
	
state_read_id:
	;Wait for the packet ID bytes
	;On success go to 128b read.
	;On timeout, wait and re-request packet
	serial_read
	JP NC, state_timeout
	serial_read
	JP NC, state_timeout
	
	;Negated packet ID *should* be on `A`
	XOR A, #0xFF  ;Get real packet ID
	CP A, C       ;Compare against expected packet
	JP Z, state_read_payload ;On success, goto next state
	
	;On error, timeout
	JP state_timeout
	
state_read_payload:
	;Read the 128B packet into memory
	;On success, check CRC
	;On timeout, wait and re-request
	LD B, #128
	next_payload_byte:
		serial_read           ;Read byte.
		JP NC, state_timeout  ;On error, re-request
		LD 0(IX), A           ;
		INC IX                ;Copy byte to RAM
		DEC B
	JP NZ, next_payload_byte  ;Get next byte
	JP state_check_crc
	
state_check_crc:
	;read CRC bytes
	serial_read
	JP NC, state_timeout  ;On error, re-request
	LD B, A
	serial_read
	JP NC, state_timeout  ;On error, re-request
	LD C, A
	
	;Check integrity.
	;On success, ACK
	;On error, NACK
	
	crc16_xmodem
	
	JP C, state_ack
	JP state_nack

state_ack:
	;Prepare for next packet
	;request next packet
	LD A, #BYTE_ACK
	serial_write
	;Packet <= Packet + 1
	INC C
	;Byte <= 0
	LD B, #0x00
	;IX should be already pointed towards the next address.

	JP state_wait_soh
	
state_wait_soh:
	;Wait for a transfer start
	;On success, get id
	;On timeout, re-request
	;On Eond of Transmission, Boot.
	;Note: Needs argument to see if it needs send an ACK or NACK
	JP C, state_read_id
	JP state_ack
	JP state_nack
	JP state_chainload
	
state_chainload:
	;Jump to ram base adress
	;JP RAM_BASE_ADDRESS
	JP state_chainload
	; --- Error states ---
state_timeout:
	;Wait enough time for a packet to pass, then retry
	JP state_nack
	
state_nack:
	JP state_init
	
	;Reset state
	;IX <= IX - current_byte
	ld_hl_ix
	LD A, L
	SUB A, B
	LD L, A
	JP PO, nack_no_underflow
	DEC H
	nack_no_underflow:
	ld_ix_hl
	;B <= 0x00
	LD B, #0x00
	;Request same packet
	LD A, #BYTE_NAK
	serial_write
	;Don't touch C.
	JP state_wait_soh
	
	
state_error_one:
putc #0x31
JP state_error

state_error_two:
putc #0x32
JP state_error

state_error_three:
putc #0x33
JP state_error

state_error_four:
putc #0x34
JP state_error

state_error:
JP state_error
	
	
	
	.ascii "uwu "
