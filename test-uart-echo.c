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
    con_put("UART echo test:\n\n");
    con_put("Connect an UART transceiver on\n");
    con_put("the SMS CONTROL 2 port as\n");
    con_put("Shown below (SMS front):\n\n");
    con_put("     CONTROL 2                \n");
    con_put("     o o o o o                \n");
    con_put("      o o o o                 \n");
    con_put("        | | `---PC TX (SMS RX)\n");
    con_put("        | `-----Ground        \n");
    con_put("        `-------PC RX (SMS TX)\n");
    con_put("                              \n");
    con_put("Serial port parameters are:   \n");
    con_put(" 4800bps      8 data bits     \n");
    con_put(" 1 start bit  1 stop bit      \n");
    con_put("");
    con_put("Write something on the term\n");
    con_put("and it will be echoed back.");
    while(1){
        uart_getc();
        if(uart_status == UART_STATUS_OK){
            uart_putc(uart_result);
        }
    }
}