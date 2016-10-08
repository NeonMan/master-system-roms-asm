
#include <sms/uart.h>
#include <sms/console.h>
#include <sms/sms.h>
#include <stdint.h>
#include <sms/intv-dummy-int.h>

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


/*Unused rst vectors declared here as well*/
void rst_08h(){
    
}

void rst_10h(){
    
}

void rst_18h(){
    
}

void rst_20h(){
    
}

void rst_28h(){
    
}

void rst_30h(){
    
}

/*A failed bank switch on return will mean the execution of RST 0x38*/
void rst_38h(){
    con_put("\nRET too slow!");
    while(1){
        
    }
}

/*Some forward decl*/
void test_page_0_1();
void test_page_2();

/*A place to store A*/
volatile uint8_t tmp_a;

void main(){
    void (*test)(void);
    volatile char c;
    uint16_t i;
    c = 1;
    tmp_a = 0;
    con_init();
    print("Mapper read stress test.\n");
    
    /*Prepare the mapper*/
    SMS_MAPPER_SLOT1(0); /*<-- Mirror Page 0 to SLOT 1*/
    SMS_MAPPER_SLOT2(2); /*<-- SLOT2 has now Page 2*/
    
    i = 65535;
    
    do
    {
        /*Call test_page_0_1 *but* on the mirrored address*/
        test  = (void*) 0x7ffe;
        test();
        
        /*Copy A*/
        __asm
        PUSH HL
          LD HL, #_tmp_a
          LD (HL), A
        POP HL
        __endasm;
        
        if(tmp_a != 0xFF){
            con_put("\nFetch too slow!");
            while(1){
                /*Stop execution*/
            }
        }
        i--;
        if((i%4096) == 0){
            con_putc('.');
        }
    }
    while(c);
    
    while(c){
        
    }
    /*Will never get here but i have to make the compiler link this*/
    test_page_0_1();
    test_page_2();
}

/*This function lies at the edge between page 0 and page 1*/
/*The opcode ADD A, n shall lie just between pages. immediate of ADD lies on */
/*0x4000 and ADD itself on 0x3fff*/

void test_page_0_1(){
    __asm
    
    .area	_HEADER (ABS)
    .org 0x3fee
    
    XOR A, A
    ADD A, #0x00
    RET
    
    .area _CODE
    __endasm;
}

/*This function will provide the 'correct' immediate */
/*It is located just at the start of page 2*/

void test_page_2(){
    __asm
    .area	_HEADER (ABS)
    .org 0x7ffc
    
    .db 0xFF
    RET
    
    .area _CODE
    __endasm;
}
