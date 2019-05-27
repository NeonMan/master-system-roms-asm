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
	;                  |                                          `---correct--->[read 128*]-+--timeout---+
	;                  |                   /<-timeout--\                                     |
	;                  |                   |           |                                     |
	;                  |<-----[reset packet]<--fail----[check crc*]<-------success-----------´
	;                  |                               |
	;                  \--[update packet]<---success---´     Note* Variant states for RAM/VRAM addresses.
	;
	; Registers:
	;
	;  A	(General purpose)
	; BC	current Byte; current paCket.
	; DE    (Used for IX/IY adjustments, could be freed using undoc opcodes)
	; HL	(General purpose)
	;
	; IX	Current buffer address
	; IY	Current VDP address
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
	.include /serial.inc/

	BYTE_SOH=0x01
	BYTE_EOT=0x04
	BYTE_CRC=0x43
	BYTE_ACK=0x06
	BYTE_NAK=0x15
	
	SMS_RAM_BASE_ADDRESS=0xC000

str_transfer_timed_out:
	.ascii "                              "
	.ascii " Timeout! Reset the console     "
	.ascii " pressing the <RESET> button    "
	.db 0x00
str_transfer_ok:
	.ascii " OK!"
	.db 0x00
str_transfer_too_big:
	.ascii " ERR!"
	.db 0x00
	
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
	LD IX, #SMS_RAM_BASE_ADDRESS
	LD IY, #0x0000 ; Base VRAM address
	LD SP, #SMS_RAM_BASE_ADDRESS

	;Prepare serial.
	serial_setup
	;Say we are ready
	putc #0x21 ; <-- Print a '!' char
	
	;Use E' for retry count
	;EXX
	LD E, #32
	;EXX
	
	;Go to transfer
	JP state_start_transfer
	
state_start_transfer:
	;Check timeout
	;EXX
	DEC E
	;EXX
	JP Z, state_timed_out
	;Request first packet, CRC mode
	LD A, #BYTE_CRC ;
	serial_write    ; Send 'C'
	JP state_first_soh

state_timed_out:
	puts #str_transfer_timed_out
	JP state_error

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
	JP NZ, state_timeout ; On error, timeout

	is_vdp  ;Test wether it is a VDP packet
	JP NC, state_read_payload     ; Write to RAM
	JP  C, state_read_payload_vdp ; Write to VDP VRAM

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

state_read_payload_vdp:
	;Read the 128B packet into VRAM
	;On success, check CRC (VDP mode)
	;On timeout, wait and re-request

	;Set VRAM base address from IY
        ; ADDR_LOW (8bit)
	IN A, (#0xBF)
	ld_a_iyl
	OUT (#0xBF), A
	; CMD (2) ADDR_H (6)
	ld_a_iyh
	AND A, #0x3F ;Trim the top 2 bits
	;OR  A, #0x40 ;Send command #1
	OUT (#0xBF), A
	
	;Read packet
	LD B, #128
	next_vdp_byte:
		serial_read           ;Read byte.
		JP NC, state_timeout  ;On error, re-request
		OUT (#0xBE), A
		DEC B
	JP NZ, next_vdp_byte ;Get next byte
	;Increment IY by 128
	LD  DE, #128
	ADD IY, DE

	JP state_check_crc_vdp

state_check_crc:
	;read CRC bytes
	serial_read
	JP NC, state_timeout  ;On error, re-request
	LD H, A
	serial_read
	JP NC, state_timeout  ;On error, re-request
	LD L, A
	
	;Check integrity.
	; DE will hold hold the packet CRC (Copied from current HL)
	; HL will hold the calculated CRC
	; B is temporary free, since it is always Zero at this point
	; IX holds the next packet base address
	
	;First, restore IX to its previous value
	AND A, A ; Clear carry
	LD DE, #-128
	ADD IX,DE
	
	;Copy CRC value back to DE
	LD D, H
	LD E, L
	
	;Init CRC
	LD HL, #0x0000
	;For each byte
	LD B, #128
	crc_loop:
		LD A, 0(IX)
		crc16_xmodem
		;Next CRC on HL
		INC IX
		DEC B
	JP NZ, crc_loop
	
	;Packet CRC on HL
	;Expected CRC on DE
	LD A, D
	CP A, H
	JP NZ, state_crc_nack
	LD A, E
	CP A, L
	JP NZ, state_crc_nack
	JP state_ack

state_check_crc_vdp:
	;read CRC bytes
	serial_read
	JP NC, state_timeout  ;On error, re-request
	LD H, A
	serial_read
	JP NC, state_timeout  ;On error, re-request
	LD L, A

	;Check integrity.
	; DE will hold hold the packet CRC (Copied from current HL)
	; HL will hold the calculated CRC
	; B is temporary free, since it is always Zero at this point
	; IY holds the next packet VDP VRAM address

	;Restore IY to last packet base address
	ld_a_iyl
	AND A, A
	JR Z, iy_dec_carry
	;IYL is 128, so we clear it.
	;IYH remains the same
	XOR A, A
	ld_iyl_a
	JR iy_dec_end
	;IYL is zero, so we make it 128
	;IYH is decremented by one
	iy_dec_carry:
	LD A, #128
	ld_iyl_a
	ld_a_iyh
	DEC A
	ld_iyh_a
	iy_dec_end:

	;Set VRAM base address from IY
        ; ADDR_LOW (8bit)
	ld_a_iyl
	OUT (#0xBF), A
	; CMD (2) ADDR_H (6)
	ld_a_iyh
	AND A, #0x3F ;Trim the top 2 bits
	;OR  A, #0x40 ;Send command #1
	OUT (#0xBF), A

	LD D, H  ; Copy packet CRC to DE
	LD E, L  ;

	;Initialize CRC calculation
	LD B, #128
	LD HL, #0x0000
	crc_vdp_loop:
		vdp_delay
		IN A, (#0xBE)
		crc16_xmodem
		DEC B
	JP NZ, crc_vdp_loop

	;Compare BC and HL
	LD A, D
	CP A, H
	JP NZ, state_crc_nack
	LD A, E
	CP A, L
	JP NZ, state_crc_nack

	;On success
	;Restore IY to next packet base address
	LD DE, #128
	ADD IY, DE

	JP state_ack

state_ack:
	;Prepare for next packet
	;Packet <= Packet + 1
	INC C

	;If a VDP packet, no dot.
	is_vdp
	JP C, no_dot_for_you
	;Print a dot every 4 packets
	LD A, C
	AND A, #0x03
	JP NZ, no_dot_for_you
		putc #0x2E
	no_dot_for_you:
	
	;IX/IY should be already pointed towards the next address.
	;request next packet
	LD A, #BYTE_ACK
	serial_write

	JP state_wait_soh
	
state_wait_soh:
	;Wait for a packet start
	serial_read
	JP NC, state_timeout
	;If EOT, chainload.
	CP A, #BYTE_EOT
	JP Z, state_chainload
	
	;If SOH, check it would fit
	CP A, #BYTE_SOH
	JP NZ, state_ysod ;state_timeout
	
	;Check if this packet is 64 + 128 (192) or below
	LD A, C
	CP  A, #193          ; IF Carry, packet <= 192
	JP  C, state_read_id ; So we jump to read_id
	JP state_rsod ; Otherwise error
	
state_chainload:
	;Everything is ready! send a few ACKs back and boot
	LD D, #5
	ack_loop:
		LD A, #BYTE_ACK
		serial_write
		tx_bit_delay
		tx_bit_delay
		DEC D
	JP NZ, ack_loop
	;Say OK! (If not VDP)
	is_vdp
	JP C, ram_base_address
	puts #str_transfer_ok
	;Jump to ram base adress
	JP ram_base_address
	.db 0xC3, 0x00, 0xC0
	
; --------------------
; --- Error states ---
; --------------------
	
state_is_too_big:
	puts #str_transfer_too_big
	JP state_error
	
state_timeout:
	;Wait enough time for a packet to pass, then retry
	JP state_nack
	
state_crc_nack:
	;Same as state nack but B is always 128
	LD B, #128
state_nack:
	;If packet is VDP, restore IY, otherwise restore IX
	is_vdp
	JP C, restore_vdp

	;Reset state (RAM)
	;IX <= IX - current_byte
	ld_hl_ix
	LD A, L
	SUB A, B
	LD L, A
	JP NC, nack_ix_no_carry
	DEC H
	nack_ix_no_carry:
	ld_ix_hl
	JP send_nack_char
	
	;Reset state (VDP)
	;IY <= IY - current_byte
	restore_vdp:
	ld_hl_iy
	LD A, L
	SUB A, B
	LD L, A
	JP NC, nack_iy_no_carry
	DEC H
	nack_iy_no_carry:
	ld_iy_hl

	send_nack_char:
	;B <= 0x00
	LD B, #0x00
	;Request same packet
	LD A, #BYTE_NAK
	serial_write
	;Don't touch C.
	JP state_wait_soh

state_bsod:
clear_cram #0x30 ; <- Blue screen
JP state_error

state_gsod:
clear_cram #0x0C ; <- Green screen
JP state_error

state_rsod:
clear_cram #0x03 ; <- Red screen
JP state_error

state_ysod:
clear_cram #0x0F ; <- Yellow screen
JP state_error

state_error:
;Go back to the main routine
JP xmodem_boot_ret

	.ascii "uwu "
