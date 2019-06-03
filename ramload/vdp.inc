.macro set_vram_packet_addr packet, cmd
	IN A, (#0xBF)  ;Clear VDP

	;Command Low byte
	LD A, packet
	SRL A          ; LSB in C
	LD A, #0x00    ; Clear A
	RRA            ; Put packet LSB in A's MSB
	OUT (#0xBF), A ; Send low byte

	;command High byte
	LD A, packet
	SRL A
	;AND A, #0x3F
	OR A, cmd
	OUT (#0xBF), A
.endm
