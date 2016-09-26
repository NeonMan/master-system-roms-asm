#include <crc/crc16-xmodem.h>
#include <sms/uart.h>
#include <sms/console.h>
#include <stdint.h>

static uint8_t i;

void main(){
   
    /*Draw header and progress info*/
    con_init();
    con_put("XMDump");
    
    for(i=2; i<29; i++){
        con_gotoxy(i,2);
        con_putc(0x80);  /*Horizontal bar*/
        con_gotoxy(i,4);
        con_putc(0x80);
    }
    con_gotoxy(1,3);
    con_putc(0x80 + 1); /*Vertical bar*/
    con_gotoxy(29,3);
    con_putc(0x80 + 1);
    con_gotoxy(0,6);
    con_put(" Size: 4194304B Sent: 4194304B\n");
    con_put("  CRC: FFFFh     Err: 0\n");
    
    
    /*Draw menu*/
    con_gotoxy(0, 15);
    con_put("Start Addr:\n");
    con_put("  End Addr:\n");
    
    
    
    /*Make the linker link*/
    uart_getc();
    crc16_xmodem_update(0, 0, 0);
    while(1){
        
    }
}
