#include <sms/intv-dummy-rst.h>
#include <sms/console.h>
#include <sms/uart.h>
#include <stdint.h>

/*Interrupt vectors. we want to catch NMI (Pause button)*/
void nmi(){
    /*Reset ROM on button press*/
    void (*rv)(void) = (void*)0x0000;
    rv();
}

void int1(){
    /*Do nothing*/
}

void main(){
    con_init();
    con_put("UART RX test:\n\n");
    
    while(1){
        uart_getc();
        if(uart_status == UART_STATUS_OK){
            con_putc(uart_result);
            con_putc(' ');
            con_puth(uart_result);
            con_putc('h');
            con_putc('\n');
        }
    }
}