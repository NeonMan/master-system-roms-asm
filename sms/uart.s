
 ;
 ; Rule number one of embedded development is "make a LED" blink. Rule number
 ; two should be "Now get a serial port going". This file aims to do just that
 ; on the SEGA master system.
 ;
 ; Peripheral ports have two selectable direction pins readable through ports
 ; DDh DCh with the added bonus that TH on port 2 is the high order bit of the
 ; port register, allowing an easy shift-out of the bit through the carry flag
 ;
 ; Now, down to business... We want to:
 ;   - Sample the TH line until a low level is detected.
 ;   - Sample the start bit multiple times for reliable framing.
 ;   - Sample TH line at regular intervals for the data bits.
 ;   - Make result available.
 ;   - Prepare for the next bit.
 ;   - Being able to do this 130 times in a row because... XMODEM ;)
 ;   - Get to 4800bps or, as the bare minimum, 1200bps
 ;
 ; The width of a single bit in T-States:
 ;   300bps:     11822 T-PAL  11931 T-NTSC  11877 T-AVG
 ;   600bps:      5911 T-PAL   5965 T-NTSC   5938 T-AVG
 ;  1200bps:      2955 T-PAL   2982 T-NTSC   2969 T-AVG
 ;  2400bps:      1477 T-PAL   1491 T-NTSC   1484 T-AVG
 ;  4800bps:       738 T-PAL    745 T-NTSC    742 T-AVG
 ;  9600bps:       369 T-PAL    372 T-NTSC    371 T-AVG
 ;
 ; UART:
 ; _______     ___ ___ ___ ___ ___ ___ ___ ___________
 ;        |   |   |   |   |   |   |   |   |   |
 ;        |   |   |   |   |   |   |   |   |   |
 ;        |STA| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |STO
 ;        |   |   |   |   |   |   |   |   |   |
 ;        |___|___|___|___|___|___|___|___|___|
 ;
 ; Start bit (4800bps):
 ; __________                                                            __
 ;           |<-------------------  ~742 T-States  -------------------->|
 ;        -->| |<-- 36T                                                 |
 ;           | |<--------------------  706 T-States  ------------------>|
 ;           |>|   |<-- 46T                                             |
 ;           |                                                          |
 ;           |     S     S     S     S     S     S     S     S          |
 ;           |<82T>|<82T>|<82T>|<82T>|<82T>|<82T>|<82T>|<82T>|<82T> <4T>|
 ;           |     |     |     |     |     |     |     |     |          |
 ;           |_____|_____|_____|_____|_____|_____|_____|_____|__________|
 ;
 ;
 ; Data bit (4800bps):
 ; __________                                                            __
 ;           |<-------------------  ~742 T-States  -------------------->|
 ;           |                                                          |
 ;           |                            S                             |
 ;           |<--------- 371T ----------->|<---------- 371T ----------->|
 ;           |                            |                             |
 ;           |____________________________|_____________________________|
 ;
 ;
 ; Sending a bit should be easier. We use port 0x3F to set the TR bit on port 2
 ; at regular intervals.
 ;
 ; Setting the port bit would be:
 ;   PORT 0x3f <-- 0b01001011 (0x4B)
 ;
 ; Resetting the port would be:
 ;   PORT 0x3f <-- 0b00001011 (0x0B)
 ;        
 ;

_hello:
	ret
 