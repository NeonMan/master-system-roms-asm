
	;; Imported functions

	.area    _HEADER (ABS)
	
	;; SDSC ROM header
	.org 0x1fe0
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
	JR init ; <-- Yes, it is very close.

	.org 0x0008
rst_08:
	JP 0xC008

	.org 0x0010
rst_10:
	JP 0xC010

	.org 0x0018
rst_18:
	JP 0xC018

	.org 0x0020
rst_20:
	JP 0xC020

	.org 0x0028
rst_28:
	JP 0xC028

	.org 0x0030
rst_30:
	JP 0xC030

	.org 0x0038
int_vector:
rst_38:
	DI
	JP 0xC038

	;; RAM base address
	.org 0xC000
ram_base_address::

; --- Regular ROM addresses begin here ---
	.area _CODE (REL)
	.ascii "/ramload.S"
	.include /macros.inc/
	.include /vdp.inc/
	;Used to emulate the way XMODEM writes to VDP
	.include /xmodem.inc/

init:
	;Set mapper registers
	LD HL, #0xFFFC
	LD (HL), #0x00
	INC HL
	LD (HL), #0x00
	INC HL
	LD (HL), #0x01
	INC HL
	LD (HL), #0x02
	JR main

main:
	;This ROM when compiled will be 8K in size, so every data past the
	;8K mark will be loaded into RAM base address then jump to it.

	;Make agreen screen
	clear_cram #0x08

	;Load 8K of data into RAM
	LD DE, #0xC000
	LD HL, #0x2000
	LD BC, #0x2000 ;8K
	LDIR

	;Test the RAM
	LD DE, #0xC000
	LD HL, #0x2000
	LD BC, #0x2000 ;8K
	ram_test_loop:
		LD A, (DE)
		CP A, (HL)
		JR NZ, ram_error
		INC DE
		INC HL
		DEC BC
	LD A, B              ;
	OR A, C              ;
	JR NZ, ram_test_loop ;If not finished, Loop.


	;Load data into VRAM
	LD C, #0 ;Counter to 127 packets (128Byte each, therefore 16K in total)
	LD HL, #0x4000 ;<-- HL pointing to 0x4000 already

	;Simulate the way N0RAM loads data in 128Byte chunks
	vram_loop:
		set_vram_packet_addr C, #0x40
		LD B, #128 ;128 bytes per packet
		vram_loop_inner:
			;(VRAM) <- (HL)
			LD A, (HL)
			OUT (#VDP_DATA_PORT), A
			INC HL
			DEC B
			JR NZ, vram_loop_inner
		INC C
		LD A, C
		CP A, #128
	JR NZ, vram_loop ;If we didn't finish, loop.

	;Test VRAM data
	LD C, #0x00  ; (same as vram loop Counter from 0 to 128)
	LD HL, #0x4000
	;Initialize VDP, read at address 0x0000
	CP (HL) ;
	CP (HL) ;
	CP (HL) ; Some delay
	vram_test_loop:
		set_vram_packet_addr C, #0x00
		LD B, #128
		vram_test_loop_inner:
			CP (HL) ;
			CP (HL) ;
			CP (HL) ;
			CP (HL) ; Some delay
			IN A, (#VDP_DATA_PORT) ;A <-- (VRAM)
			CP A, (HL)
			JR NZ, vram_error
			INC HL
			DEC B
		JR NZ, vram_test_loop_inner
	INC C
	LD A, C
	CP A, #128
	JR NZ, vram_test_loop ;If not finished, loop.

	;Chainload to RAM
	JP 0xC000 ;Jump to loaded stuff.

ram_error:
	;Show a Blu Screen Of Death
	clear_cram #0x30
	JR error_loop

vram_error:
	;Show a Red Screen Of Death
	clear_cram #0x03
	JR error_loop

error_loop:
	;Loop forever~
	JR error_loop

sdsc_name::
	.ascii "RAMload -- Helper ROM to test RAM-Loadable ROMs."
	.db 0x00

sdsc_author::
	.ascii "J.Luis <root@heavydeck.net>"
	.db 0x00

sdsc_description::
	.ascii "This ROM when prepended to any data, will load the\r\n"
	.ascii "data past this ROM into RAM then jump into it, making\r\n"
	.ascii "bootlader ROMs bootable on regular cartridges."
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
