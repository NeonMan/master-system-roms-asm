#include <sms/intv-dummy-rst.h>
#include <sms/console.h>
#include <sms/uart.h>
#include <stdint.h>

static volatile uint8_t pause_button;
static const char hello[] = "Hello world!!! UUUUUUUU\r\n";

/*Interrupt vectors. we want to catch NMI (Pause button)*/
void nmi(){
    pause_button = 1;
}

void int1(){
    /*Do nothing*/
}

void main(){
    uint8_t c;
    con_init();
    con_put("UART TX test.\n");
    con_put("Press PAUSE to send.\n");
    pause_button = 0;
    c = 0xFF;
    
    while(1){
        if(pause_button){
            pause_button = 0;
            uart_putc(c);
            con_puth(c);
            con_putc(' ');
            c = c + 1;
        }
    }
}
