
	;; Imported functions

	.area    _HEADER (ABS)
	
	;; SDSC ROM header
	.org 0x7fe0
	.ascii "SDSC"
	.db 0xff,0xff            ; Version (BCD)
	.db 0xff,0xff,0xff,0xff  ; Date DD/MM/YYYY (BCD)
	.dw #sdsc_author         ; Author pointer
	.dw #sdsc_name           ; Name pointer
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
	;Horizontal bar
	.ascii "{~~~~~~~~~~~~~~~~~~~~~~~~~~~~}  "
	.ascii "| N0RAM    XMODEM Bootloader |  "
	.ascii "| V1.2         heavydeck.net |  "
	.ascii "[~~~~~~~~~~~~~~~~~~~~~~~~~~~~]  "

	.ascii " CONTROL 2     Serial config    "
	.ascii " o o o o o     9600bps 8/N/1    "
	.ascii "  o o o o                       "
	.ascii "    | | [~~ TX (PC RX)          "
	.ascii "    | [~~~~ Ground              "
	.ascii "    [~~~~~~ RX (PC TX)          "
	.ascii "                                "
	.ascii "This bootloader also does VRAM  "
	.ascii "upload. Check README for info.  "
	.ascii "Booting in: "
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
	.ascii " Press <RESET> to pet the cat   "
	.db 0x00
	
	;; Initialize
init:
	;Put mapper in a known state
	LD HL, #0xFFFC
	LD (HL), #0x00
	INC HL
	LD (HL), #0x00
	INC HL
	LD (HL), #0x01
	INC HL
	LD (HL), #0x02
	;Prepare VDP
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
	;If RESET button is pressed, jump to 0x0000
	IN A, (#0xDD)
	AND A, #0x10
	JP Z, #0x0000
	;Otherwise, wait.
	JP endloop
	
	;SDSC info
sdsc_author::
	.ascii "Heavydeck <root@heavydeck.net>"
	.db 0x00

sdsc_name::
	.ascii "N0RAM -- XMODEM Bootloader for Master System"
	.db 0x00

sdsc_description::
	.ascii "A program to easily upload a program on the SEGA Master "
	.ascii "System without a need to flash any EPROM. This program "
	.ascii "can load a binary of up to 8KB (All the SMS ROM) using "
	.ascii "the XMODEM protocol. ROMs shall be prepared to run from "
	.ascii "RAM base address 0xC000 instead of the regular 0x0000. "
	.ascii "As of V1.2 the bootloader will upload to VRAM all ROM "
	.ascii "data past the 8K mark up to 16K of VRAM data, allowing "
	.ascii "the preload of VDP data before booting, saving RAM on "
	.ascii "the bootloadable program. Max upload size is 24K."
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
