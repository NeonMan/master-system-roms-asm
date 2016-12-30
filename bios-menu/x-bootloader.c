/**
 * @file x-bootloader.c
 * @brief XMODEM bootloader module.
 *
 */

#include <stdint.h>
#include <sms/console.h>
#include <sms/uart.h>
#include <crc/crc16-xmodem.h>
#include "x-bootloader.h"

/*XMODEM macros*/
#define MAX_UPLOAD_PACKETS 32
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

static uint8_t x_buffer[(MAX_UPLOAD_PACKETS * 128)];
static uint8_t frame_buffer[XMODEM_FRAME_SIZE];


#define FRAME_OK   0
#define FRAME_NONE 1
#define FRAME_NOK  2
#define FRAME_EOT  3
static uint8_t read_frame(uint8_t ack){
    /*A frame is composed of: */
    /* SOH Byte           1B  */
    /* Frame number       1B  */
    /* Inverse frame no   1B  */
    /* Payload          128B  */
    /* SUM/CRC          1/2B  */
    
    /*Send the ack character*/
    uart_putc(ack);
    
    /*Read 128 + 5 bytes*/
    UART_BUFF_GETC(frame_buffer,   0);
    if(uart_result == C_EOT) return FRAME_EOT;
    UART_BUFF_GETC(frame_buffer,   1);
    UART_BUFF_GETC(frame_buffer,   2);
    
    UART_BUFF_GETC(frame_buffer,   3);
    UART_BUFF_GETC(frame_buffer,   4);
    UART_BUFF_GETC(frame_buffer,   5);
    UART_BUFF_GETC(frame_buffer,   6);
    UART_BUFF_GETC(frame_buffer,   7);
    UART_BUFF_GETC(frame_buffer,   8);
    UART_BUFF_GETC(frame_buffer,   9);
    UART_BUFF_GETC(frame_buffer,  10);
    UART_BUFF_GETC(frame_buffer,  11);
    UART_BUFF_GETC(frame_buffer,  12);
    UART_BUFF_GETC(frame_buffer,  13);
    UART_BUFF_GETC(frame_buffer,  14);
    UART_BUFF_GETC(frame_buffer,  15);
    UART_BUFF_GETC(frame_buffer,  16);
    UART_BUFF_GETC(frame_buffer,  17);
    UART_BUFF_GETC(frame_buffer,  18);
    UART_BUFF_GETC(frame_buffer,  19);
    UART_BUFF_GETC(frame_buffer,  20);
    UART_BUFF_GETC(frame_buffer,  21);
    UART_BUFF_GETC(frame_buffer,  22);
    UART_BUFF_GETC(frame_buffer,  23);
    UART_BUFF_GETC(frame_buffer,  24);
    UART_BUFF_GETC(frame_buffer,  25);
    UART_BUFF_GETC(frame_buffer,  26);
    UART_BUFF_GETC(frame_buffer,  27);
    UART_BUFF_GETC(frame_buffer,  28);
    UART_BUFF_GETC(frame_buffer,  29);
    UART_BUFF_GETC(frame_buffer,  30);
    UART_BUFF_GETC(frame_buffer,  31);
    UART_BUFF_GETC(frame_buffer,  32);
    UART_BUFF_GETC(frame_buffer,  33);
    UART_BUFF_GETC(frame_buffer,  34);
    UART_BUFF_GETC(frame_buffer,  35);
    UART_BUFF_GETC(frame_buffer,  36);
    UART_BUFF_GETC(frame_buffer,  37);
    UART_BUFF_GETC(frame_buffer,  38);
    UART_BUFF_GETC(frame_buffer,  39);
    UART_BUFF_GETC(frame_buffer,  40);
    UART_BUFF_GETC(frame_buffer,  41);
    UART_BUFF_GETC(frame_buffer,  42);
    UART_BUFF_GETC(frame_buffer,  43);
    UART_BUFF_GETC(frame_buffer,  44);
    UART_BUFF_GETC(frame_buffer,  45);
    UART_BUFF_GETC(frame_buffer,  46);
    UART_BUFF_GETC(frame_buffer,  47);
    UART_BUFF_GETC(frame_buffer,  48);
    UART_BUFF_GETC(frame_buffer,  49);
    UART_BUFF_GETC(frame_buffer,  50);
    UART_BUFF_GETC(frame_buffer,  51);
    UART_BUFF_GETC(frame_buffer,  52);
    UART_BUFF_GETC(frame_buffer,  53);
    UART_BUFF_GETC(frame_buffer,  54);
    UART_BUFF_GETC(frame_buffer,  55);
    UART_BUFF_GETC(frame_buffer,  56);
    UART_BUFF_GETC(frame_buffer,  57);
    UART_BUFF_GETC(frame_buffer,  58);
    UART_BUFF_GETC(frame_buffer,  59);
    UART_BUFF_GETC(frame_buffer,  60);
    UART_BUFF_GETC(frame_buffer,  61);
    UART_BUFF_GETC(frame_buffer,  62);
    UART_BUFF_GETC(frame_buffer,  63);
    UART_BUFF_GETC(frame_buffer,  64);
    UART_BUFF_GETC(frame_buffer,  65);
    UART_BUFF_GETC(frame_buffer,  66);
    UART_BUFF_GETC(frame_buffer,  67);
    UART_BUFF_GETC(frame_buffer,  68);
    UART_BUFF_GETC(frame_buffer,  69);
    UART_BUFF_GETC(frame_buffer,  70);
    UART_BUFF_GETC(frame_buffer,  71);
    UART_BUFF_GETC(frame_buffer,  72);
    UART_BUFF_GETC(frame_buffer,  73);
    UART_BUFF_GETC(frame_buffer,  74);
    UART_BUFF_GETC(frame_buffer,  75);
    UART_BUFF_GETC(frame_buffer,  76);
    UART_BUFF_GETC(frame_buffer,  77);
    UART_BUFF_GETC(frame_buffer,  78);
    UART_BUFF_GETC(frame_buffer,  79);
    UART_BUFF_GETC(frame_buffer,  80);
    UART_BUFF_GETC(frame_buffer,  81);
    UART_BUFF_GETC(frame_buffer,  82);
    UART_BUFF_GETC(frame_buffer,  83);
    UART_BUFF_GETC(frame_buffer,  84);
    UART_BUFF_GETC(frame_buffer,  85);
    UART_BUFF_GETC(frame_buffer,  86);
    UART_BUFF_GETC(frame_buffer,  87);
    UART_BUFF_GETC(frame_buffer,  88);
    UART_BUFF_GETC(frame_buffer,  89);
    UART_BUFF_GETC(frame_buffer,  90);
    UART_BUFF_GETC(frame_buffer,  91);
    UART_BUFF_GETC(frame_buffer,  92);
    UART_BUFF_GETC(frame_buffer,  93);
    UART_BUFF_GETC(frame_buffer,  94);
    UART_BUFF_GETC(frame_buffer,  95);
    UART_BUFF_GETC(frame_buffer,  96);
    UART_BUFF_GETC(frame_buffer,  97);
    UART_BUFF_GETC(frame_buffer,  98);
    UART_BUFF_GETC(frame_buffer,  99);
    UART_BUFF_GETC(frame_buffer, 100);
    UART_BUFF_GETC(frame_buffer, 101);
    UART_BUFF_GETC(frame_buffer, 102);
    UART_BUFF_GETC(frame_buffer, 103);
    UART_BUFF_GETC(frame_buffer, 104);
    UART_BUFF_GETC(frame_buffer, 105);
    UART_BUFF_GETC(frame_buffer, 106);
    UART_BUFF_GETC(frame_buffer, 107);
    UART_BUFF_GETC(frame_buffer, 108);
    UART_BUFF_GETC(frame_buffer, 109);
    UART_BUFF_GETC(frame_buffer, 110);
    UART_BUFF_GETC(frame_buffer, 111);
    UART_BUFF_GETC(frame_buffer, 112);
    UART_BUFF_GETC(frame_buffer, 113);
    UART_BUFF_GETC(frame_buffer, 114);
    UART_BUFF_GETC(frame_buffer, 115);
    UART_BUFF_GETC(frame_buffer, 116);
    UART_BUFF_GETC(frame_buffer, 117);
    UART_BUFF_GETC(frame_buffer, 118);
    UART_BUFF_GETC(frame_buffer, 119);
    UART_BUFF_GETC(frame_buffer, 120);
    UART_BUFF_GETC(frame_buffer, 121);
    UART_BUFF_GETC(frame_buffer, 122);
    UART_BUFF_GETC(frame_buffer, 123);
    UART_BUFF_GETC(frame_buffer, 124);
    UART_BUFF_GETC(frame_buffer, 125);
    UART_BUFF_GETC(frame_buffer, 126);
    UART_BUFF_GETC(frame_buffer, 127);
    UART_BUFF_GETC(frame_buffer, 128);
    UART_BUFF_GETC(frame_buffer, 129);
    UART_BUFF_GETC(frame_buffer, 130);
    
    UART_BUFF_GETC(frame_buffer, 131);
    UART_BUFF_GETC(frame_buffer, 132);
    
    /*Test frame for errors*/
    {
        uint16_t frame_crc;
        uint16_t expected_crc;
        
        frame_crc = crc16_xmodem_init();
        frame_crc = crc16_xmodem_update(frame_crc, frame_buffer+3, 128);
        frame_crc = crc16_xmodem_finalize(frame_crc);
        
        expected_crc = ((uint16_t)frame_buffer[131])<<8 | frame_buffer[132] ;
        
        if(frame_crc == expected_crc){
            return FRAME_OK;
        }
        else{
            return FRAME_NOK;
        }
    }
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

uint8_t xboot_download(){
    uint16_t i;
    uint8_t rv;
    uint8_t packet_count;
    
    for(i=0; i<(MAX_UPLOAD_PACKETS * 128); i++){
        x_buffer[i] = 0xFF; /*<-- Fill buffer with RST opcodes.*/
    }
    
    for(i=0; i<(128 + 4); i++){
        frame_buffer[i] = '?';
    }
    
    /* XMODEM transfer */
    packet_count = 0;
    
    {
        rv = read_frame(C_CRC);
    }
    
    while(rv != FRAME_EOT){
        /*Select the adecuate acknowledge response*/
        switch(rv){
            case FRAME_EOT:
            break;
            
            case FRAME_OK:
            /*Frame ok*/
            if(packet_count < MAX_UPLOAD_PACKETS){
                con_putc('.');
                /*Copy packet data to the execute buffer*/
                for(i=0; i<128; i++){
                    x_buffer[(packet_count * 128) + i] = frame_buffer[i + 3];
                }
                /*increment packet counter*/
                packet_count++;     
            }
            else{
                packet_count = 255;
                con_putc('!');
            }
            /*Answer with ACK*/
            rv = read_frame(C_ACK);
            break;
            
            default:
            case FRAME_NOK:
            /*Frame corrupted*/
            con_putc('?');
            /*Answer with a NAK*/
            rv = read_frame(C_NAK);
            break;
        }
    }
    
    /*End of transmission*/
    con_putc('*');
    /*Send a few ACK and finish*/
    uart_putc(C_ACK);
    uart_putc(C_ACK);
    uart_putc(C_ACK);
    
    /*Determine if the upload was too big*/
    if(packet_count <= MAX_UPLOAD_PACKETS){
        con_put("\nUpload complete!\n");
        return BOOT_READY;
    }
    else{
        con_put("\nError: Upload too big.\n");
        delay();
        delay();
        return BOOT_ERROR;
    }
    
}

void xboot_launch(){
    uint16_t addr = (uint16_t) x_buffer;
    void (*exec_ptr)(void) = (void*) addr;
    exec_ptr();
    while(1){
    }
}