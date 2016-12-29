/**
 * @file x-bootloader.c
 * @brief XMODEM bootloader module.
 *
 */

#include <stdint.h>
#include <sms/console.h>
#include <sms/uart.h>
#include <crc/crc16-xmodem.h>

/*XMODEM macros*/
#define MAX_UPLOAD_FRAMES 32
#define C_NAK 0x15
#define C_ACK 0x06
#define C_SOH 0x01
#define C_EOT 0x04
#define C_CRC 'C'
#define XMODEM_FRAME_SIZE (128 + 5)

#define CRC_MODE_CHECKSUM 0
#define CRC_MODE_CRC16    1
#define CRC_MODE_UNDEF    0xFF

#define READ_RETRIES 16
#define BYTE_RETRIES 250

static uint8_t  crc_mode;
uint8_t         x_buffer[(MAX_UPLOAD_FRAMES * 128) + 6];
uint8_t         frame_buffer[128 + 4];
static uint16_t packet_crc;
static uint8_t  crc_mode;
static uint8_t  error_count;
static uint8_t  data_left;
static uint16_t x_buffer_index;


#define FRAME_OK   0
#define FRAME_NONE 1
#define FRAME_NOK  2
#define FRAME_EOT  3
uint8_t read_frame(uint8_t ack){
    /*A frame is composed of: */
    /* SOH Byte           1B  */
    /* Frame number       1B  */
    /* Inverse frame no   1B  */
    /* Payload          128B  */
    /* SUM                1B  */
    uint16_t bytes_left;
    uint8_t  retry_left;
    uint8_t  frame_index;
    
    retry_left = BYTE_RETRIES;
    bytes_left = XMODEM_FRAME_SIZE;
    frame_index = 0;
    uart_putc_fast(ack);
    /*read a frame*/
    do{
        
        uart_getc();
        if(uart_status == UART_STATUS_OK){
            frame_buffer[frame_index] = uart_result;
            frame_index++;
            retry_left = BYTE_RETRIES;
        }
        else{
            retry_left--;
        }
    }while(retry_left);
    
    
    if(frame_index == 0){
            return FRAME_NONE; /*Timeout*/
    }
    
    {
        /*Debug -- Print frame*/
        
        for(frame_index=0; frame_index <132; frame_index++){
            con_putc(frame_buffer[frame_index]);
        }
        
        con_put("\n---------------\n");
        
    }
    return FRAME_NONE;
}

static void delay(){
    volatile uint16_t i;
    for(i=0; i<20000; i++){
            i = i;
            i = i;
            i = i;
            i = i;
    }
}

void xboot(){
    uint16_t i;
    uint8_t rv;
    uint8_t  packets_left;
    uint8_t  retry_count;
    uint8_t eot_recevied;
    
    con_clear();
    con_gotoxy(0, 0);
    con_put("XMODEM bootloader for SMS.");
    
    con_gotoxy(0,2);
    con_put("Connect an UART transceiver on\n");
    con_put("the SMS CONTROL 2 port as\n");
    con_put("Shown below (SMS front):\n\n");
    con_put("     CONTROL 2\n");
    con_put("     o o o o o\n");
    con_put("      o o o o\n");
    con_put("        | | `---PC TX (SMS RX)\n");
    con_put("        | `-----Ground\n");
    con_put("        `-------PC RX (SMS TX)\n");
    con_put("\n");
    con_put("Serial config: 4800/8/1/N\n");
    con_put("\n");
    con_put("Start an XMODEM transfer on\n");
    con_put("the PC now. Max ROM size: "  "4K" "\n\n");
    
    for(i=0; i<((MAX_UPLOAD_FRAMES * 128) + 6); i++){
        x_buffer[i] = 0xFF; /*<-- Fill buffer with RST opcodes.*/
    }
    
    for(i=0; i<(128 + 4); i++){
        frame_buffer[i] = '?';
    }
    
    /* XMODEM transfer */
    packet_crc = crc16_xmodem_init();
    crc_mode = CRC_MODE_UNDEF;
    error_count = 0;
    packets_left = MAX_UPLOAD_FRAMES;
    retry_count = READ_RETRIES;
    eot_recevied = 0;
    x_buffer_index = 0;
    
    /*Send initial C/NACK*/
    
    do{
        rv = read_frame(C_NAK);
        delay();
    }while (rv != FRAME_OK);
    packets_left--;
    con_put("Transmission started:\n");
    
    /*Read the remaining frames*/
    do{
        rv = read_frame(C_ACK);
        if(rv == FRAME_NOK){
            con_put("!");
            uart_putc_fast(C_NAK);
        }
        else if(rv == FRAME_NONE){
            retry_count--;
            if(retry_count){
                continue;
            }
            else{
                break;
            }
        }
        else if(rv == FRAME_EOT){
            packets_left = 0;
            eot_recevied = 1;
            uart_putc_fast(C_ACK);
        }
        else{
            retry_count = READ_RETRIES;
            packets_left--;
            con_put(".");
            uart_putc_fast(C_ACK);
        }
    }while(packets_left);
    
    /*Check the circumstances of the exit*/
    con_put("\n");
    if(eot_recevied || (packets_left == 0)){
        con_put("Transmission completed!\n");
        con_put("Booting...");
        delay();
        /*Boot it!*/
        {
            void (* buffer_call)(void) = (void (*)(void)) x_buffer;
            /*buffer_call();*/
            while(1){
                
            }
        }
    }
    else if(packets_left){
        con_put("Transmission timeout!\n");
        con_put("Exitting...\n");
        delay();
        
    }
}