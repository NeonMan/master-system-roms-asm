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
	; >[  Init  ]---[reqest]-->[wait SOH]---yes------->[read idx]-----incorrect---+                       |
	;                  |<----------/<-------timeout--------´      |                                       |
	;                  |                                          `---correct---->[read 128]+---timeout---+
	;                  |                    /-timeout-\                                     |
	;                  |<-----[reset packet]<--fail----[check crc]<--------success----------´
	;                  \--[update packet]<---success--´
	;
	; Registers:
	;
	;  A    (General purpose)
	; BC    current Byte; current paCket.
	; DE
	; HL    (General purpose)
	;
	; IX    Current buffer address
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
	.include /crc16-xmodem.inc/

; --- Unit test mock functions ---


	
xmodem_boot::
	;Go back to main routine; Transmission error?
	JP xmodem_boot_ret
	
	;Check the marvelous state machine diagram, the dead trees version.
state_init:
	;Prepare variables for the first transfer.
	;Packet <= 1
	LD C, #0x01
	;Byte <= 0
	LD B, #0x00
	;Buffer <= RAM_BASE_ADDRESS
	LD HL, #0xC000    ;Master System RAM base address.
	ld_ix_hl          ;Store on IX.

	JP state_start_transfer
	
state_start_transfer:
	;Request first packet, CRC mode
	
	JP state_first_soh
	
state_first_soh:
	;Wait for the start of transmission byte.
	;On success, continue, on timeout go back to start_transfer.
	
	JP C, state_read_id
	JP state_start_transfer
	
state_read_id:
	;Wait for the packet ID bytes
	;On success go to 128b read.
	;On timeout, wait and re-request packet
	
	JP C, state_read_payload
	JP state_timeout
	
state_read_payload:
	;Read the 128B packet into memory
	;On success, check CRC
	;On timeout, wait and re-request
	
	JP C, state_check_crc
	JP state_timeout
	
state_check_crc:
	;Check integrity.
	;On success, ACK
	;On error, NACK
	
	crc16_xmodem
	
	JP C, state_ack
	JP state_nack

state_ack:
	;Prepare for next packet
	;request next packet
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
	
	; --- Error states ---
state_timeout:
	;Wait enough time for a packet to pass, then retry
	JP state_nack
	
state_nack:
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
	;Don't touch C.
	JP state_wait_soh
	
	.ascii "uwu "