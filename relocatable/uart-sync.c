#include <sms/console.h>
#include <sms/uart.h>
#include <stdint.h>

static const char hello[] = "Sending 'U' on UART";

static void delay_loop(){
    volatile uint16_t i;
    for(i=0; i<10000; i++){
        i = i;
    }
}

void main(){
    delay_loop();
    con_init();
    con_put(hello);
    
    while(1){
        uart_putc_faster('U');
        uart_putc_faster('U');
        uart_putc_faster('U');
        uart_putc_faster('U');
    }
}
