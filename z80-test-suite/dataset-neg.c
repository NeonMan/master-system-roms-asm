/**
 * @file  dataset-add.c
 * @brief exhaust the ADD alu operations.
 *
 * And send the results through UART.
 *
 * Inputs: A
 * Outputs: !A, Flags
 */
#include <sms/uart.h>
#include <sms/console.h>
#include <stdint.h>

static void delay(){
    volatile uint16_t i;
    for(i = 0; i<50000; i++){
        i = i;
    }
}

static void print(char* str){
    while(*str){
        uart_putc(*str);
        str++;
    }
}

static const char hex_lut[17] = "0123456789ABCDEF";
static void print_hex(uint8_t b){
    uart_putc(hex_lut[(b>>4) & 0x0F]);
    uart_putc(hex_lut[(b   ) & 0x0F]);
}

uint8_t  input_a;
uint8_t  input_b;
uint16_t output_af;

static void do_test(){
    input_a = 255;
    do{
        input_a++;
        input_b = 254; /*Run inner loop once*/
        do{
            input_b++;
            __asm
            LD HL, #0x0000    ;
            PUSH HL           ;
            POP AF            ;AF <-- 0x0000

            LD HL, #_input_a  ;
            LD A, (HL)        ;
                              ;
            LD HL, #_input_b  ; Get operands
            LD B, (HL)

            ; --- Perform operation ---

            NEG A
            
            ; --- Copy result to variable --
            PUSH AF
            POP HL
            LD (#_output_af), HL
            
            __endasm;
            
            /*Send result through UART*/
            print_hex(output_af>>8);
            print_hex(output_af);
            uart_putc(',');
            
        }while(input_b < 255);
        uart_putc('\r');
        uart_putc('\n');
        con_putc('.');
    }while(input_a < 255);
}

void main(){
    con_init();
    con_put("Z80 NEG F=00h Dataset\n");
    con_put("Output via Control 2 UART\n\n");
    con_put("See README.md for more info\n");
    
    /*Send header through UART*/
    print("#Hex value of AF registers after performing NEG A; !A with F = 0x00\r\n");
    print("#A increments downwards [0..255]\r\n");
    print("#Commit ID: " COMMIT_ID "\r\n");
    con_putc('.');
    
    /*Send payload*/
    do_test();
    
    /*Send tail through UART*/
    print("#EOF\r\n");
    
    /*End*/
    con_put("\nDone!");
    delay();
    
    /*Reboot*/
    {
        void (*reset_call)(void) = (void*) 0x0000;
        reset_call();
    }
}