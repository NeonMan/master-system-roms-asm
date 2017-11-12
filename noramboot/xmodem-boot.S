	;XMODEM bootloader, the fun begins, all registers free.
	;
	;Reg A  is presumed dirty on pretty much _any_ macro call
	;
	
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
