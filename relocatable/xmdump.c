#include <crc/crc16-xmodem.h>
#include <sms/uart.h>
#include <stdint.h>

void main(){
    /* Make the linker link :P */
    uart_putc('C');
    uart_getc();
    crc16_xmodem_update(0, 0, 0);
    while(1){
        
    }
}
