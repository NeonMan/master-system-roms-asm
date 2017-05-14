
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

; --------------------------
; --- Values from uart.h ---
; --------------------------

UART_STATUS_OK  = 0x00
UART_STATUS_NOK = 0xFF

; void uart_getc()
_uart_getc::
    PERIPHERAL_PORT  = 0xDD
    PRESAMPLE_DELAY  = 24   ; Original value 26  ; Tune these two to select the bit width and
    POSTSAMPLE_DELAY = 23   ; Original value 24  ; where in the bit the sample is made.
    UART_VALID_START = 0x00 ; What the sampled start bit looks like if correctly sampled.
    ; ----------------------------------------
    ; --- Detect start bit (82T    budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        LD HL, #_uart_status       ; 10T
        LD (HL), #UART_STATUS_NOK  ; 10T
        RET  C                     ;  5T (presumed false)
    ; Remaining budget: 42T
        LD HL, #_uart_result       ; 10T
        LD (HL), #0x00             ; 10T  ; HL points to uart_result
        LD A, #0x00                ;  7T  ; 
        LD BC, #0x00               ; 10T  ; B will keep the start bit sampling
    ; Remaining budget:  5T
        ;NOP                        ; 4T   ; Delay
    ; Leftover: 1T (Ignored bc of changes)
    
    
    ; ------------------------------
    ; --- Sample start bit macro ---
    ; ------------------------------
    .macro sample_start_bit
            IN   A, (#PERIPHERAL_PORT) ; 11T
            RLA                        ;  4T
            RL   B                     ;  8T
        ; Remaining budget 59T
            PUSH IX                    ; 15T ;
            POP  IX                    ; 14T ;
            PUSH IX                    ; 15T ;
            POP  IX                    ; 14T ; Delay
        ; Leftover: 1T
    .endm
    
    ;Perform the first 7 samples.
    sample_start_bit
    sample_start_bit
    sample_start_bit
    sample_start_bit
    
    sample_start_bit
    sample_start_bit
    sample_start_bit
    
    ;Last sample needs to do some extra processing instead of a
    ;busy delay, hence is sampled separate.
    
    ; ----------------------------------------
    ; --- Start sample #7/7 (82+3T Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
        
    ; --- Check if it is a start bit-
    ; Last sample of the start bit has been made. To check
    ; wether this is a start bit or not we shall check the
    ; current value of B and test against 0xFF or 0x00
    ; depending on UART polarity. Presumed positive here.
    ;       Remaining sample budget: 59+3T
    ; Additional last sample budget:    4T
    ;                        Budget:   66T
    
        LD   A, B                  ;  4T
        CP   #UART_VALID_START     ;  7T
        RET  NZ                    ;  5T (presumed false)
    ; Remaining budget: 50T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH HL                    ; 11T ;
        POP  HL                    ; 10T ; Delay
        
        NOP
        NOP
        NOP
        NOP
        NOP
        
    ; Leftover: 0
    
    
    ; We start sampling the data bits. A will be used for
    ; port input and delay counter, while B will store the
    ; sampled byte
    
        ; ----------------------------------------
        ; --- Sample data bit macro (371+371T) ---
        ; ----------------------------------------
        .macro sample_bit ?delay_pre,?delay_post
        ; Skip 371T
            LD   A, #PRESAMPLE_DELAY   ;  7T
            
        delay_pre:
            DEC  A                     ;  4T  ;
            JP   NZ, delay_pre         ; 10T  ; Execute 26 times
            
        ; Sample
            IN   A, (#PERIPHERAL_PORT) ; 11T
            RLA                        ;  4T
            RR   B                     ;  8T
        ; Remaining budget: 354 (skip it)
            LD   A, #POSTSAMPLE_DELAY  ;  7T
            
        delay_post:
            DEC  A                     ;  4T  ;
            JP   NZ, delay_post        ; 10T  ; Execute 24 times
            NOP                        ;  4T  ; Delay
            LD   (HL), B               ;  7T  ; Store (partial) result
            
        ; Leftover: 0
        .endm
    
    ; ---------------------
    ; --- Sample 8 bits ---
    ; ---------------------
    sample_bit
    sample_bit
    sample_bit
    sample_bit
    
    sample_bit
    sample_bit
    sample_bit
    sample_bit
    
    ; By this time the value shall be stored at uart_result (HL)
    ; A should be Zero and B should have a copy of the result.
    ;
    ; The stop bit starts here so we should have a reasonable ammount
    ; of time to copy the byte to a buffer even using C
    
    ; Budget: 742T
    LD HL, #_uart_status     ; 10T
    LD (HL), #UART_STATUS_OK ; 10T
    
    ret                      ; 10T
    ; Remaining budget: 712T
 

;void uart_putc(uint8_t c){
_uart_putc::
        ;742 T-States for every bit.
        ;Port-2 TR will be our TX pin.
        ;
        ;Delay times have been refined using a logic analyzer to fit the width
        ;of every bit within 1% of the expected width.
        
        UART_DOWN = 0xBB
        UART_UP   = 0xFB
        IO_PORT   = 0x3F
        START_DELAY = 45
        DATA_DELAY  = 49
        STOP_DELAY  = 52
        
        ;Pull line DOWN to send a start bit
        LD A, #UART_DOWN      ;  7T
        OUT (#IO_PORT),A      ; 11T
        
        ; -----------------
        ; --- START BIT ---
        ; -----------------
        ;742 T-States
        POP HL                ; 10T* ; Save return value
        DEC SP                ;  6T
        POP BC                ; 10T
        DEC SP                ;  6T; Argument byte on B, Stack restored.
        PUSH HL               ; 11T* ; Restore return value
        
        ;Remaining budget: 720T (-25T)
        LD C, #START_DELAY    ;  7T
    tx_start_delay:
        DEC C                 ;  4T
        JP NZ, tx_start_delay ; 10T
        NOP                   ;  4T
        NOP                   ;  4T
        NOP                   ;  4T
        
        ;Remaining budget: 2T
        
        ; ----------------------
        ; --- DATA BIT MACRO ---
        ; ----------------------
        .macro sendbit ?tx_bit_one,?tx_bit_zero,?tx_bit,?tx_delay
                RR B               ;  8T ; Move bit to Carry flag

                JP NC, tx_bit_zero ; 10T ;
                JP C,  tx_bit_one  ; 10T ;
            tx_bit_zero:                 ;
                LD A, #UART_DOWN   ;  7T ;
                JP tx_bit          ; 10T ;
            tx_bit_one:                  ; Regardless of path taken
                LD A, #UART_UP     ;  7T ; 17T
            tx_bit:
                OUT (#IO_PORT), A  ; 11T ; Change bit edge
                
            ; Before edge change we spent 25 + 11 T-States
            ; Time to waste enough time to fill 706 T-States
                LD C, #DATA_DELAY  ;  7T
            tx_delay:
                DEC C              ;  4T
                JP NZ, tx_delay    ; 10T
            ; Remaining budget: 13 T
            ;NOP                    ;  4T
            ;NOP                    ;  4T
            ;NOP                    ;  4T
            ; Remainder: 1T
        .endm
        
        sendbit
        sendbit
        sendbit
        sendbit
        
        sendbit
        sendbit
        sendbit
        sendbit
        
        NOP                ;  4T
        NOP                ;  4T
        NOP                ;  4T
        
        ;Use 25T til OUT command
        LD A, #UART_UP     ;  7T
        NOP                ;  4T
        NOP                ;  4T
        NOP                ;  4T
        NOP                ;  4T
        ;Remainder: 2T
        OUT (#IO_PORT), A  ; 11T ; Change bit edge
        
        ; ----------------
        ; --- STOP BIT ---
        ; ----------------
        ;Use 742 T-States
            LD C, #STOP_DELAY  ;  7T
        tx_delay:
            DEC C              ;  4T
            JP NZ, tx_delay    ; 10T
			
			ret
 
 
; void uart_putc_fast(uint8_t c)
_uart_putc_fast::
        ;742 T-States for every bit.
        ;Port-2 TR will be our TX pin.
        ;
        ;Delay times have been refined using a logic analyzer to fit the width
        ;of every bit within 1% of the expected width.
        
        UART_DOWN = 0xBB
        UART_UP   = 0xFB
        IO_PORT   = 0x3F
        START_DELAY = 45
        DATA_DELAY  = 49
        STOP_DELAY  = 52
        
        ;Pull line DOWN to send a start bit
        LD A, #UART_DOWN      ;  7T
        OUT (#IO_PORT),A      ; 11T
        
        ; -----------------
        ; --- START BIT ---
        ; -----------------
        ;742 T-States
        POP HL                ; 10T* ; Save return value
        DEC SP                ;  6T
        POP BC                ; 10T
        DEC SP                ;  6T; Argument byte on B, Stack restored.
        PUSH HL               ; 11T* ; Restore return value
        
        ;Remaining budget: 720T (-25T)
        LD C, #START_DELAY    ;  7T
    tx_fast_start_delay:
        DEC C                 ;  4T
        JP NZ, tx_fast_start_delay ; 10T
        NOP                   ;  4T
        NOP                   ;  4T
        NOP                   ;  4T
        
        ;Remaining budget: 2T
        
        sendbit
        sendbit
        sendbit
        sendbit
        
        sendbit
        sendbit
        sendbit
        sendbit
        
        NOP                ;  4T
        NOP                ;  4T
        NOP                ;  4T
        
        ;Use 25T til OUT command
        LD A, #UART_UP     ;  7T
        NOP                ;  4T
        NOP                ;  4T
        NOP                ;  4T
        NOP                ;  4T
        ;Remainder: 2T
        OUT (#IO_PORT), A  ; 11T ; Change bit edge
        
        ;Skip STOP bit and return immediatly
		ret
    
;void uart_putc_faster(uint8_t c)
_uart_putc_faster::
        UART_DOWN = 0xBB
        UART_UP   = 0xFB
        IO_PORT   = 0x3F
        START_DELAY = 45
        DATA_DELAY  = 49
        STOP_DELAY  = 52
        
        ;Read parameters
        POP HL                ; 10T* ; Save return value
        DEC SP                ;  6T
        POP BC                ; 10T
        DEC SP                ;  6T; Argument byte on B, Stack restored.
        PUSH HL               ; 11T* ; Restore return value
        
        ;Pull line DOWN to send a start bit
        LD A, #UART_DOWN      ;  7T
        OUT (#IO_PORT),A      ; 11T
        ;Widen the start bit as needed, use logical analyzer.
        NOP                   ;  4T
        
        ; ----------------------
        ; --- DATA BIT MACRO ---
        ; ----------------------
        .macro sendbit_faster ?ftx_bit_one,?ftx_bit_zero,?ftx_bit
                RR B               ;  8T ; Move bit to Carry flag
                JP NC, ftx_bit_zero ; 10T ;
                JP C,  ftx_bit_one  ; 10T ;
            ftx_bit_zero:                 ;
                LD A, #UART_DOWN   ;  7T ;
                JP ftx_bit          ; 10T ;
            ftx_bit_one:                  ; Regardless of path taken
                LD A, #UART_UP     ;  7T ; 17T
            ftx_bit:
                OUT (#IO_PORT), A  ; 11T ; Change bit edge
        .endm
        
        sendbit_faster
        sendbit_faster
        sendbit_faster
        sendbit_faster
        
        sendbit_faster
        sendbit_faster
        sendbit_faster
        sendbit_faster
        
        LD A, #UART_UP     ;  7T
        OUT (#IO_PORT), A  ; 11T ; Change bit edge
		ret