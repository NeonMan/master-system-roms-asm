
	;; Imported functions

	.area    _HEADER (ABS)
	
	;; SDSC ROM header
	.org 0x7fe0
	.ascii "SDSC"
	.db 0xff,0xff            ; Version (BCD)
	.db 0xff,0xff,0xff,0xff    ; Date DD/MM/YYYY (BCD)
	.dw #sdsc_author        ; Author pointer
	.dw #sdsc_name            ; Name pointer
	.dw #sdsc_description    ; Description pointer
	
	;; SEGA header
	.ascii "TMR SEGA"
	.db 0xFF, 0xFF            ;Reserved bytes
	.db 0xFF, 0xFF            ;Checksum
	.db 0xFF, 0xFF, 0xFF    ;Product code and version
	.db 0x6c                ;Export SEGA, 32K.
	
	;;Make NMI RESET the ROM
	.org 0x0066
	JR 0x0000
	
	;; Entry point
	.org 0x0000
	DI
	JP init
	
	;; RAM base address
	.org 0xC000
ram_base_address::

	.area _CODE (REL)
	.ascii "/noramboot.S"
	.include /macros.inc/

txt_countdown_padding:
	.ascii "             "
	.db 0x00
	
txt_hello:
	.ascii "------------------------------  "
	.ascii "-- N0RAM  XMODEM Bootloader --  "
	.ascii "--            heavydeck.net --  "
	.ascii "------------------------------  "
	.ascii " Connect UART transceiver on    "
	.ascii " the CONTROL 2 port as shown    "
	.ascii "                                "
	.ascii " CONTROL 2     Serial config    "
	.ascii " o o o o o     4800bps 8/N/1    "
	.ascii "  o o o o                       "
	.ascii "    | | `-- RX (PC TX)          "
	.ascii "    | `---- Ground              "
	.ascii "    `------ TX (PC RX)          "
	.ascii "                                "
	.ascii " Booting in: "
	.db 0x00
txt_uart:
	.ascii "                                "
	.ascii " Starting XMODEM transmission    " ;<-- Extra ' ' intentional.
	.db 0x00
txt_shitpost:
	.ascii "                                "
	.ascii "      *purr*                    "
	.ascii "              |\__/,|   (`\     "
	.ascii "   *purr*     |_ _  |.--.) )    "
	.ascii "              ( T   )     /     "
	.ascii "             (((^_(((/(((_>     "
	.ascii "                                "
	.ascii " Press [PAUSE] to pet the cat   "
	.db 0x00
	
	;; Initialize
init:
	JP vdp_init
vdp_init_ret::
	;Show some info
	puts #txt_hello
	;And a nice countdown
	delay #0x0300
	putc #0x33 ;3
	putc #0x20
	delay #0x0300
	putc #0x32 ;2
	putc #0x20
	delay #0x0300
	putc #0x31 ;1
	putc #0x20
	delay #0x0300
	puts #txt_countdown_padding
	
	;Read control port 1
	IN A, (#0xDC)
	AND A, #0x20 ;Mask button 2
	;If 1 or 2 are pressed jump to Shitpost
	JR Z, use_shitpost
	
	;Use UART
	puts #txt_uart
	;Jump to XMODEM loader
	JP xmodem_boot
xmodem_boot_ret::
	JR endloop
	
	;Show some shitpost
use_shitpost:
	puts #txt_shitpost
	JR endloop
	
endloop:
	JP endloop
	
	;SDSC info
sdsc_author::
	.ascii "Heavydeck <root@heavydeck.net>"
	.db 0x00

sdsc_name::
	.ascii "N0RAM Bootloader for Master System"
	.db 0x00

sdsc_description::
	.ascii "Put a proper description when the program is complete and "
	.ascii "free rom is not an issue."
	.db 0x00
	;End of file
	.ascii "uwu "
	

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
