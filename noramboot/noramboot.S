
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
nmi_vector:
	JR 0x0000
	
	;; Entry point
	.org 0x0000
reset_vector:
	DI
	JP init

	.org 0x0008
rst_08:
	JR 0x0000

	.org 0x0010
rst_10:
	JR 0x0000

	.org 0x0018
rst_18:
	JR 0x0000

	.org 0x0020
rst_20:
	JR 0x0000

	.org 0x0028
rst_28:
	JR 0x0000

	.org 0x0030
rst_30:
	JR 0x0000

	.org 0x0038
int_vector:
rst_38:
	JR 0x0000

	;; RAM base address
	.org 0xC000
ram_base_address::

	.area _CODE (REL)
	.ascii "<noramboot.S>"
	.include /macros.inc/
	.include /serial.inc/
	.include /vdp.inc/

txt_hello:
	;Horizontal bar
	.ascii "{~~~~~~~~~~~~~~~~~~~~~~~~~~~~}  "
	.ascii "| N0RAM    XMODEM Bootloader |  "
	.ascii "| V1.3         heavydeck.net |  "
	.ascii "[~~~~~~~~~~~~~~~~~~~~~~~~~~~~]  "

	.ascii " CONTROL 2     Serial config    "
	.ascii " o o o o o     14400bps 8/N/1   "
	.ascii "  o o o o                       "
	.ascii "    | | [~~ TX (PC RX)          "
	.ascii "    | [~~~~ Ground              "
	.ascii "    [~~~~~~ RX (PC TX)          "
	.ascii "                                "
	.ascii " Check README for usage info.   "
	.ascii "                                "
	;Draw empty progressbar, 18 chars, Line 16 (15)
	.ascii "     RAM:"
		.db #130
		.db #128, #128, #128, #128
		.db #128, #128, #128, #128
		.db #128, #128, #128, #128
		.db #128, #128, #128
		.db #131
		.ascii "      "
	.ascii "                                " ; <-- To add retry count later
;	.ascii " Retries: 0000h                 "
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

	set_vram_addr #0x3B, #0x53

	;And a nice countdown
	delay #0x0600

	;Jump to XMODEM loader
	JP xmodem_boot
xmodem_boot_ret::
	; Use one of these as an action on XMODEM failure
	JP endloop      ; <-- Ask user to manually Reset
	;JP reset_vector ; <-- Reset without asking

endloop:
	;XMODEM failed, next option is trying to chainload
	;some other media (External/Card/Cartridge).
	;This option requires some RAM to work but, we don't
	;need RAM to be free anymore since upload failed, so
	;we setup the Stack to be used. ;)
	LD SP, #0xDFF0
	;and CALL the chainloader
	CALL chainload

	;If chainloader fails, reset the bootloader.
	;If RESET button is (not)pressed, jump to 0x0000
	endloop_inner:
		IN A, (#0xDD)
		AND A, #0x10
		JP Z, #0x0000   ;Regular mode  (reset when RESET is pressed)
;		JP NZ, #0x0000  ;Bootloop mode (reset unless RESET is pressed)
	;Otherwise, wait.
	JP endloop_inner
	
	;SDSC info
sdsc_name::
	.ascii "N0RAM -- XMODEM Bootloader for Master System"
	.db 0x00

sdsc_author::
	.ascii "J.Luis <root@heavydeck.net>"
	.db 0x00

sdsc_description::
	.ascii "A program to easily upload a program on the SEGA Master\r\n"
	.ascii "System without a need to flash any EPROM. This program\r\n"
	.ascii "can load a binary of up to 24KB (8K RAM + 16K VRAM) using\r\n"
	.ascii "the XMODEM protocol. Executable code shall be prepared to\r\n"
	.ascii "run from RAM base address 0xC000 and use up to 8K of\r\n"
	.ascii "combined Program/RAM memory. Any data transmitted past\r\n"
	.ascii "the initial 8K will be saved on the VDP VRAM up to a max\r\n"
	.ascii "of 16 additional KB, allowing the preload of VDP data\r\n"
	.ascii "before booting.\r\n"
	.ascii "\r\n"
	.ascii "Check README for additional info. Source code and sample\r\n"
	.ascii "bootloadable ROMS available at:\r\n"
	.ascii "https://bitbucket.org/Heavydeck/master-system-roms/\r\n"
	.ascii "Released under the MIT license, see COPYING for the\r\n"
	.ascii "complete licensing text."
	.db 0x00
	;End of file
	.ascii "</noramboot.S>"
	

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
