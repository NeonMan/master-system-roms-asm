/**
 * @file  dataset-add.c
 * @brief exhaust the ADD alu operations.
 *
 * Inputs: A, B
 * Outputs: (A + B), Flags
 */
#include <sms/uart.h>
#include <stdint.h>

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

static void do_test(){
    
}

void main(){
    /*Send header through UART*/
    print("/*" __FILE__ "*/");
    print("static const uint16_t result[] = {\n");
    
    /*Send payload*/
    do_test();
    
    /*Send tail through UART*/
    print("\n};\n");
    
    /*Reboot*/
    {
        void (*reset_call)(void) = (void*) 0x0000;
        reset_call();
    }
}