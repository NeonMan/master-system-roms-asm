	;XMODEM bootloader, the fun begins, all registers free.
	;
	;Reg A  is presumed dirty on pretty much _any_ macro call
	;Reg HL will be used thorougly for all-arround many calls
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
	
	; Registers:
	;
	;  A    (General purpose)
	; BC    current Byte; current paCket.
	; DE    
	; HL    (General purpose)
	;
	; IX
	; IY
	; 
	;  A'
	; BC'
	; DE'
	; HL'
	
	.area _CODE (REL)
	.ascii "/xmodem-boot.S"
	
	.include /macros.inc/
	.include /crc16-xmodem.inc/
	
xmodem_boot::
	;Go back to main routine; Transmission error?
	JP xmodem_boot_ret
	
	;Dummy call to CRC routine for space estimation
	crc16_xmodem
	
	.ascii "uwu "
