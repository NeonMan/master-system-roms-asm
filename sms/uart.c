
/*
 * Rule number one of embedded development is "make an LED" blink. Rule number
 * two should be "Now get a serial port going". This file aims to do just that
 * on the SEGA master system.
 *
 * Peripheral ports have two selectable direction pins readable through port
 * 0xDD with the added bonus that one such pin of pad port B is the high order
 * bit of the port register, allowing an easy shift-out of Port B TH sample bit
 *
 * Now, down to business... We want to:
 *   - Sample the TH line until a low level is detected.
 *   - Sample TH line at regular intervals 4/8 times per data bit.
 *   - Move the result to a buffer
 *   - Prepare for the next bit
 *   - Being able to do this 130 times in a row because... XMODEM ;)
 *   - Get to 4800bps or, as the bare minimum, 1200bps
 *
 * The width of a single bit in T-States:
 *   300bps:     11822 T-PAL  11931 T-NTSC  11877 T-AVG
 *   600bps:      5911 T-PAL   5965 T-NTSC   5938 T-AVG
 *  1200bps:      2955 T-PAL   2982 T-NTSC   2969 T-AVG
 *  2400bps:      1477 T-PAL   1491 T-NTSC   1484 T-AVG
 *  4800bps:       738 T-PAL    745 T-NTSC    742 T-AVG
 *  9600bps:       369 T-PAL    372 T-NTSC    371 T-AVG
 *
 * UART:
 * _______     ___ ___ ___ ___ ___ ___ ___ ___________
 *        |   |   |   |   |   |   |   |   |   |
 *        |   |   |   |   |   |   |   |   |   |
 *        |STA| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |STO
 *        |   |   |   |   |   |   |   |   |   |
 *        |___|___|___|___|___|___|___|___|___|
 *
 * Start bit (4800bps):
 * __________                                                            __
 *           |<-------------------  ~742 T-States  -------------------->|
 *        -->| |<-- 36T                                                 |
 *           | |<--------------------  706 T-States  ------------------>|
 *           |>|   |<-- 46T                                             |
 *           |                                                          |
 *           |     S     S     S     S     S     S     S     S          |
 *           |<82T>|<82T>|<82T>|<82T>|<82T>|<82T>|<82T>|<82T>|<82T> <4T>|
 *           |     |     |     |     |     |     |     |     |          |
 *           |_____|_____|_____|_____|_____|_____|_____|_____|__________|
 *
 *
 * Data bit (4800bps):
 * __________                                                            __
 *           |<-------------------  ~742 T-States  -------------------->|
 *           |                                                          |
 *           |                            S                             |
 *           |<--------- 371T ----------->|<---------- 371T ----------->|
 *           |                            |                             |
 *           |____________________________|_____________________________|
 *
 *
 * Sending a bit should be easier. We use port 0x3F to set the TR bit on port 2
 * at regular intervals.
 *
 * Setting the port bit would be:
 *   PORT 0x3f <-- 0b01001011 (0x4B)
 *
 * Resetting the port would be:
 *   PORT 0x3f <-- 0b00001011 (0x0B)
 *        
 */
#include <stdint.h> 
#include "uart.h"

uint8_t uart_result;
uint8_t uart_status;

/*What sampling a start bit eight times looks like*/
/*Either 0x00 (positive logic) or 0xFF (negative logic)*/
#define UART_VALID_START 0xFF

/*If enabled the result byte will be negated before returning*/
#define UART_FLIP_RESULT

uint8_t uart_get_status(){
    return uart_status;
}

void uart_getc(){
    
    
    __asm
    PERIPHERAL_PORT  = 0xDD
    PRESAMPLE_DELAY  = 26
    POSTSAMPLE_DELAY = 24
    ; ----------------------------------------
    ; --- Detect start bit (82T    budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RET  C                     ;  5T (presumed false)
    ; Remaining budget: 62T
        LD HL, #_uart_status       ; 10T
        LD (HL), #UART_STATUS_NOK  ; 10T
        LD HL, #_uart_result       ; 10T
        LD (HL), #0x00             ; 10T  ; HL points to uart_result
        LD A, #0x00                ;  7T  ; 
        LD BC, #0x00               ; 10T  ; B will keep the start bit sampling
    ; Remaining budget:  5T
        NOP                        ; 4T   ; Delay
    ; Leftover: 1T (Ignored bc of changes)
    
    ; ----------------------------------------
    ; --- Start sample #0/7 (82T   Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 1T
    
    ; ----------------------------------------
    ; --- Start sample #1/7 (82+1T Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59+1T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 2T
    
    ; ----------------------------------------
    ; --- Start sample #2/7 (82+2T Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59+2T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 3T
    
    ; ----------------------------------------
    ; --- Start sample #3/7 (82+3T Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59+3T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 4T
        NOP
    ; Leftover: 0T
    
    ; ----------------------------------------
    ; --- Start sample #4/7 (82T   Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 1T

    ; ----------------------------------------
    ; --- Start sample #5/7 (82+1T Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59+1T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 2T
    
    ; ----------------------------------------
    ; --- Start sample #6/7 (82+2T Budget) ---
    ; ----------------------------------------
        IN   A, (#PERIPHERAL_PORT) ; 11T
        RLA                        ;  4T
        RL   B                     ;  8T
    ; Remaining budget 59+2T
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ;
        PUSH IX                    ; 15T ;
        POP  IX                    ; 14T ; Delay
    ; Leftover: 3T
    
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
            RL   B                     ;  8T
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
    
    __endasm;
    
    
#ifdef UART_FLIP_RESULT
    __asm
    
    LD A,B
    NEG
    LD (HL), A
    
    __endasm;
#endif
    
    __asm
    ; Budget: 742T
    LD HL, #_uart_status     ; 10T
    LD (HL), #UART_STATUS_OK ; 10T
    
    ;Implicit RET from the function return
    ;ret                      ; 10T
    ; Remaining budget: 712T
    __endasm;
}

/*Output port*/
#define UART_OUT_PORT 0x3F

/*What to write to control port to set the line LOW/HIGH*/
#define UART_OUT_LOW 0x0B
#define UART_OUT_HIGH 0x4B

void uart_putc(uint8_t c){
    
    __asm
        DEC SP                ;  6T
        POP AF                ; 10T
        DEC SP                ;  6T; Argument byte on A, Stack restored.
        
        NOP
    
    __endasm;
    
}

void sample(){
    uart_putc('U');
}