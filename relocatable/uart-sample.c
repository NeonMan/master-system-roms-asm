
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
 *   - Get to 1200bps or, as the bare minimum, 300bps
 *
 * The with of a single bit in miliseconds and in T-States:
 *    300bps       3.333  ms    11823 T-PAL    11931 T-NTSC
 *    600bps       1.667  ms     5913 T-PAL     5967 T-NTSC
 *   1200bps       0.833  ms     2956 T-PAL     2982 T-NTSC
 *   1T (NTSC)     0.2794 us     0.99 T-PAL        1 T-NTSC
 *   1T (PAL)      0.2819 us        1 T-PAL    1.009 T-NTSC
 *
 * UART:
 *
 * _______    __ __ __ __ __ __ __ __ ______                                  
 *        |  |  |  |  |  |  |  |  |  |                                        
 *        |  |  |  |  |  |  |  |  |  |                                        
 *        |ST|00|01|02|03|04|05|06|07|ST                                      
 *        |  |  |  |  |  |  |  |  |  |                                        
 *        |__|__|__|__|__|__|__|__|__|                                        
 *
 * Start bit (1200bps):
 *
 * __________                                                            __
 *           | <------------------  ~2970 T-States  ------------------> |
 *        -->| |<-- 36T                                                 |
 *           | |<--------------------  2934 T-States  ----------------> |
 *           |                                                          |
 *           |                                                          |
 *           |                                                          |
 *           |                                                          |
 *           |__________________________________________________________|
 *
 *
 * Data bit sampling:
 * __________                                                             __
 *           |<-------------------  ~2970 T-States  -------------------->|  
 *           |<297> <297> <297> <297> <297> <297> <297> <297> <297> <297>|  
 *           |           |     |     |     |     |     |     |           |  
 *           |        S0>|  S1>|  S2>|  S3>|  S4>|  S5>|  S6>|           |  
 *           |           |     |     |     |     |     |     |           |  
 *           |<-- 594 -->|     |     |     |     |     |     |<-- 594 -->|  
 *           |           |     |     |     |     |     |     |           |  
 *           |___________|_____|_____|_____|_____|_____|_____|___________|  
 *
 *
 * Sampling the TH line using IN and the carry bit takes XXX T -states (T)
 *
 *
 *          IN   A, (#0xDD)            ; 11T
 *          RLCA                      ;  4T
 *          RET  C                     ;  5T
 *    ;                             ---> 36T
 *
 *    ; -----------------------------------------
 *    ; --- Skip 2933 T-states til bit0 start ---
 *    ; --- Use this to initiallize stuff     ---
 *    ; -----------------------------------------
 *
 *    ;Bit 0
 *        
 */
 
 
 
/*

    ;Pre-sample delay (594T)
    LD    A,32                 ;   7T
sample_delay:
    NOP                        ;   4T
    DEC   A                    ;   4T
    JP NZ, sample_delay        ;  10T
    AND 0xFF                   ;   7T
    NOP                        ;   4T
    ;                       ---> 594T

    
    
    
    
    ;Acquire sample
    IN   A, (#0xDD)            ;  11T
    RLC                        ;   4T
    LD   A,(sample_storage)    ;  13T
    RRC                        ;   4T
    LD   (sample_storage),A    ;  13T
    ;                       ---> +45T
    ;Inter-sample delay (252T)
    LD    A,13                 ;   7T
intersample_delay:
    NOP                        ;   4T
    DEC   A                    ;   4T
    JP NZ, intersample_delay   ;  10T
    LD    A,0x00               ;   7T
    NOP                        ;   4T 
    ;                       --->+252T

    
    ;          Sample-total ---> 297T
 */
 
 
 
 
 
/*

    IN   A, (#0xDD)            ; 11T
    RLC                        ;  4T
    LD   A,(sample_storage)    ; 13T
    RRC                        ;  4T
    LD   (sample_storage),A    ; 13T
    ;                     -----> 45T

 */
 
#include <stdint.h> 

uint8_t uart_sample_0;
uint8_t uart_sample_1;
uint8_t uart_sample_2;
uint8_t uart_sample_3;

uint8_t uart_sample_4;
uint8_t uart_sample_5;
uint8_t uart_sample_6;
uint8_t uart_sample_7;

uint8_t uart_result;


void get_byte(){
    __asm
    
    ;Start bit. Return if line is up.
        IN   A, (#0xDD)            ; 11T
        RLCA                       ;  4T
        RET  C                     ;  5T
        
    ;Initialize stuff
        LD HL, #0x0000             ; 10T
        LD (_uart_sample_0), HL    ; 16T
        LD (_uart_sample_1), HL    ; 16T
        LD (_uart_sample_2), HL    ; 16T
        LD (_uart_sample_3), HL    ; 16T
        LD (_uart_sample_4), HL    ; 16T
        LD (_uart_sample_5), HL    ; 16T
        LD (_uart_sample_6), HL    ; 16T
        LD (_uart_sample_7), HL    ; 16T
        LD (_uart_result), HL      ; 16T
        
    ; -------------------------------------------------
    ; --- Skip util the start of the first data bit ---
    ; -------------------------------------------------
    
    ;Pre-sample delay (594T)
        LD    A,#32                ;   7T
    presample_delay:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, presample_delay     ;  10T
        LD A,#0x00                 ;   7T
        NOP                        ;   4T
        ;                       ---> 594T
        
    ;Acquire sample 0
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_0)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_0),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_0:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_0 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
        
    ;Acquire sample 1
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_1)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_1),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_1:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_1 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
        
    ;Acquire sample 2
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_2)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_2),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_2:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_2 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
        
    ;Acquire sample 3
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_3)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_3),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_3:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_3 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
        
    ;Acquire sample 4
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_4)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_4),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_4:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_4 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
        
    ;Acquire sample 5
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_5)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_5),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_5:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_5 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
        
    ;Acquire sample 6
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_6)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_6),A    ;  13T
        ;                       ---> +45T
        ;Inter-sample delay (252T)
        LD    A,#13                ;   7T
    intersample_delay_6:
        NOP                        ;   4T
        DEC   A                    ;   4T
        JP NZ, intersample_delay_6 ;  10T
        LD    A,#0x00              ;   7T
        NOP                        ;   4T 
        ;                       --->+252T
    
    ;Acquire sample 7
        IN   A, (#0xDD)            ;  11T
        RLCA                       ;   4T
        LD   A,(_uart_sample_7)    ;  13T
        RRCA                       ;   4T
        LD   (_uart_sample_7),A    ;  13T
        ;                       ---> +45T
        
    ; -------------------------------------------
    ; --- Process samples. Cycle budget: 549T ---
    ; -------------------------------------------
    ret                            ;  10T
    
    __endasm;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
void main(){
    while(1){
        
    }
}
 