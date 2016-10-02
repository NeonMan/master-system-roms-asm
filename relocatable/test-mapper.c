#include <sms/uart.h>
#include <sms/console.h>
#include <sms/sms.h>
#include <stdint.h>

static void delay_loop(){
    volatile uint16_t i;
    for(i=0;i<15000;i++){
        i = i;
    }
}

static void print_nc(uint8_t* str){
    while(*str){
        uart_putc(*str);
        con_putc(*str);
        str++;
    }
}

static void print(uint8_t* str){
    print_nc(str);
    uart_putc('\r');
    uart_putc('\n');
    con_putc('\n');
}

#define BUFFER_SIZE      1024
#define TEST_RESULT_OK   0x00
#define TEST_RESULT_NOK  0xFF

#define TEST_ITERATIONS  100
uint8_t read_buffer[BUFFER_SIZE];
uint8_t test_result;

static void print_result(char* msg, uint8_t is_ok){
    print_nc(msg);
    print_nc(": ");
    if(is_ok == TEST_RESULT_OK){
        print("OK");
    }
    else{
        print("ERR");
    }
}

/**
 * @brief Change slots and read a single SLOT2 byte.
 * 
 * Reading it just after a SLOT0 read, forcing a "close" read.
 */
#define TEST_01_NAME "Slot 0/2 byte read"
static void _test_01(){
    __asm
    
    LD HL, #_test_result
    LD (HL), #TEST_RESULT_OK
    
    ;Map slot 2 to bank 0
    LD HL, #0xFFFF
    LD (HL), #0x00
    
    ;Copy a block from SLOT2 base address
    LD DE, #_read_buffer
    LD HL, #SMS_SLOT2_BASE_ADDRESS
    
    ;First force a readout from 0x0000
    LD BC, #0x0000
    LD A, (BC)
    
    ;Copy a byte from SLOT2
    LD A, (HL)
    
    ;Test result
    LD D,A
    LD A, (BC)
    CP D
    JP Z, test01nok
    
    LD HL, #_test_result
test01ok:    
    LD (HL), #TEST_RESULT_OK
    ret
test01nok:
    LD (HL), #TEST_RESULT_NOK
    ret
    
    ;Copy some bytes from SLOT2 base address
    ;intercalated with SLOT0 data
    __endasm;
}

static void test_01(){
    uint16_t i;
    for(i=0; i<(TEST_ITERATIONS*10); i++){
        _test_01();
        if(test_result != TEST_RESULT_OK){
            break;
        }
    }
}

void main(){

    con_init();
    
    print("Mapper test.");
    
    /*Test 01*/
    test_01();
    print_result(TEST_01_NAME, test_result);
    
    while(1){
        
    }
}
