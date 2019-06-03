;Sets VDP address for a given XMODEM packet.
;Args:
;  - packet  Packet index (Max val: 127)
;  - cmd     Command (Valid cases: 0x00, 0x40, 0x80, 0xC0)
;Uses: AF
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
