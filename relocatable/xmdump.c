#include <crc/crc16-xmodem.h>
#include <sms/uart.h>
#include <sms/console.h>
#include <sms/sms.h>
#include <stdint.h>

static int8_t i;
static uint8_t tmp_reg;

/*Pad input variables*/
static uint8_t last_input;

/*Address increment*/
static uint16_t increment; /*Valid values 1,16,256,4096*/

static uint16_t addr_start;
static uint16_t addr_end;

static uint8_t bank_start;
static uint8_t bank_end;

#define MEDIA_CARTRIDGE 0
#define MEDIA_CARD      1
#define MEDIA_BIOS      2
#define MEDIA_EXT       3
#define MEDIA_COUNT     4
static uint8_t current_media;
static const char media_names[] = 
    "Cart.\0"
    "Card \0"
    "BIOS \0"
    "Ext. \0"
    ;


static int8_t current_cursor;
#define CURSOR_COUNT 6
static const uint8_t cursors[CURSOR_COUNT][2] = {
    {7,17}, {7+15,17},
    {7,18}, {7+15,18},
    {7,19}, {12,21}
};

/*XMODEM related stuff*/
uint16_t current_crc;
uint16_t packet_crc;
uint8_t  packet_count;
uint8_t  packet_buffer[128];
uint16_t error_count;
#define CRC_MODE_CHECKSUM 0
#define CRC_MODE_CRC16    1
#define CRC_MODE_UNDEF    0xFF
uint8_t  crc_mode;

#define C_NAK 0x15
#define C_ACK 0x06
#define C_SOH 0x01
#define C_EOT 0x04

static void set_status(void* s){
    con_gotoxy(7,8);
    con_put("              ");
    con_gotoxy(7,8);
    con_put(s);
}

/*Reads a packet, returns true if data left*/
static uint8_t read_packet(){
    uint8_t  idx;
    uint8_t* rom_buffer;
    /*Set the SLOT2 mapper to the selected bank*/
    *((volatile uint8_t*) SMS_SLOT2_CONTROL_ADDRESS) = bank_start;
    idx = 128;
    
    rom_buffer = ((uint8_t*)SMS_SLOT2_BASE_ADDRESS);
    rom_buffer += addr_start;
    
    do{
        idx--;
        packet_buffer[127-idx] = *rom_buffer;
        ++rom_buffer;
        
    }while(idx);
    
    addr_start += 128;
    
    if(bank_start != bank_end){
        if(addr_start > 0x3FFF){
            bank_start++;
            addr_start = addr_start & 0x3FFF;
        }
    }

    if(bank_start == bank_end){
        if(addr_start >= addr_end){
            return 0;
        }
    }
    
    return 1;
}

static void get_checksum(){
    uint8_t idx;
    if(crc_mode == CRC_MODE_CHECKSUM){
        packet_crc = 0;
        for(idx = 0; idx<128; idx++){
            packet_crc = packet_crc + packet_buffer[idx];
        }
    }
    else{
        packet_crc = crc16_xmodem_init();
        packet_crc = crc16_xmodem_update(packet_crc, packet_buffer, 128);
        packet_crc = crc16_xmodem_finalize(packet_crc);
    }
}

static void send_checksum(){
    if(crc_mode == CRC_MODE_CHECKSUM){
        uart_putc(packet_crc);
    }
    else{
        uart_putc(packet_crc>>8);
        uart_putc(packet_crc);
    }
}

static void send_header(){
    uart_putc(C_SOH);
    uart_putc(packet_count);
    uart_putc(255-packet_count);
}

static void send_packet(){
    uint8_t idx;
    for(idx=0; idx<128;idx++){
        uart_putc(packet_buffer[idx]);
    }
}

static void send_eot(){
    uart_putc(C_EOT);
}

/*XMODEM transfer*/
static void transfer(){
    uint8_t data_left;
    
    /*Temporal fix. Make the addresses align to 128B*/
    addr_start = addr_start & 0xFF80;
    addr_end   = addr_end & 0xFF80;
    
    if(bank_end<bank_start){
        bank_end = bank_start;
    }
    /*----------------------------------------------*/
    
    /*XMODEM PROTOCOL!*/
    current_crc = crc16_xmodem_init();
    packet_crc = crc16_xmodem_init();
    packet_count = 1;
    crc_mode = CRC_MODE_UNDEF;
    data_left = 1;
    error_count = 0;
    
    /*Select media slot*/
    switch(current_media){
        case MEDIA_BIOS:
        sms_enable_port = 0b11100000;
        break;
        case MEDIA_CARD:
        sms_enable_port = 0b11001000;
        break;
        case MEDIA_CARTRIDGE:
        sms_enable_port = 0b10101000;
        break;
        case MEDIA_EXT:
        sms_enable_port = 0b01101000;
        break;
        
        default:
        break;
    }
    
    /*Status: Waiting for C or ACK*/
    set_status("Wait ACK");
    do{
        /*Read the serial line til we get a char*/
        do{
            uart_getc();
        }while(uart_status != UART_STATUS_OK);
        
        /*Select XMODEM CRC mode*/
        if(uart_result == 'C'){
            crc_mode = CRC_MODE_CRC16;
            set_status("Transfer (CRC)");
        }
        else if(uart_result == C_NAK){
            crc_mode = CRC_MODE_CHECKSUM;
            set_status("Transfer (SUM)");
        }
    }while(crc_mode == CRC_MODE_UNDEF);
    
    /*Basic algorithm. While there is data left*/
    while(data_left){
        uint8_t has_ack;
        has_ack = 0;
        /*Read a packet*/
        data_left = read_packet();
        get_checksum();
        /*Send the packet til we get an ACK, if not, resend.*/
        do{
            send_header();
            send_packet();
            send_checksum();
            
            while(!has_ack){
              do{
                  uart_getc();
              }while(uart_status != UART_STATUS_OK);
              if(uart_result == C_ACK){
                  has_ack = 1;
              }
              else if(uart_result == C_NAK){
                  error_count++;
                  break; /*Re-send on NACK*/
              }
            }
        }while(!has_ack);
        packet_count++;
    }
    
    send_eot();
    
    set_status("FINISHED");
}

/*Increment the parameters on the screen*/
static void increment_val(){
    uint8_t bank_increment = (increment>>8) | increment;
    switch(current_cursor){
        case 0: /*Start addr*/
        addr_start += increment;
        addr_start &= 0x3FFF;
        break;
        
        case 1: /*Start bank*/
        bank_start += bank_increment;
        break;
        
        case 2: /*End addr*/
        addr_end += increment;
        addr_end &= 0x3FFF;
        break;
        
        case 3: /*End bank*/
        bank_end += bank_increment;
        break;
        
        case 4: /*Media*/
        current_media++;
        if(current_media == MEDIA_COUNT)
            current_media = 0;
        break;
        
    }
}

static void refresh_main_menu(){
    /*Scan input*/
    tmp_reg = sms_ab_port;
    if(last_input != tmp_reg){
        if      (!(tmp_reg & (1<<0))){
            current_cursor -= 2;
        }
        else if (!(tmp_reg & (1<<1))){
            current_cursor += 2;
        }
        else if (!(tmp_reg & (1<<2))){
            current_cursor -= 1;
        }
        else if (!(tmp_reg & (1<<3))){
            current_cursor += 1;
        }
        else if (!(tmp_reg & (1<<5))){
            /*Change increment rate for address values*/
            increment = (increment << 4);
            if (increment == 0)
                increment++;
        }
        else if (!(tmp_reg & (1<<4))){
            /*Increment value if pinting at a value*/
            if(current_cursor < 5){
                increment_val();
            }
            /*Start transfer if pointing to start*/
            else if(current_cursor == 5){
                transfer();
            }
        }
        
        /*Correct cursor overflow*/
        if(current_cursor >= CURSOR_COUNT){
            current_cursor -= CURSOR_COUNT;
        }
        else if (current_cursor < 0){
            current_cursor += CURSOR_COUNT;
        }
        last_input = tmp_reg;
    }
    
    /*Paint cursor*/
    for(i=0; i<CURSOR_COUNT; i++){
        con_gotoxy(cursors[i][0], cursors[i][1]);
        if(i == current_cursor) con_putc('>');
        else                    con_putc(' ');
    }
    
    /* --- Paint address parameters --- */
    /*Start addr:*/
    con_gotoxy(cursors[0][0] + 1, cursors[0][1]);
    con_puth(addr_start>>8);
    con_puth(addr_start);
    /*Start bank:*/
    con_gotoxy(cursors[1][0] + 1, cursors[1][1]);
    con_puth(bank_start);
    /*End addr:*/
    con_gotoxy(cursors[2][0] + 1, cursors[2][1]);
    con_puth(addr_end>>8);
    con_puth(addr_end);
    /*end bank:*/
    con_gotoxy(cursors[3][0] + 1, cursors[3][1]);
    con_puth(bank_end);
    /*Media*/
    con_gotoxy(cursors[4][0] + 1, cursors[4][1]);
    con_put(media_names + (current_media * 6));
    
    /*Debug stuff*/
    con_gotoxy(10,0);
    con_puth(increment>>8);
    con_puth(increment);
}

static void init(){
    /*Init stuff*/
    addr_start = 0x0000;
    addr_end   = 0x3FFF;
    bank_start = 0;
    bank_end = 1;
    current_media = MEDIA_CARTRIDGE;
    current_cursor = 0;
    last_input = 0xFF;
    increment = 1;
   
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
    /*Paint the four corners*/
    con_gotoxy(1,2);
    con_putc(0x80 + 2);
    con_gotoxy(1,4);
    con_putc(0x80 + 4);
    con_gotoxy(29,2);
    con_putc(0x80 + 3);
    con_gotoxy(29,4);
    con_putc(0x80 + 5);
    
    /*Paint stats menu*/
    con_gotoxy(0,6);
    con_put(" Size: ???????B Sent: ???????B\n");
    con_put("  CRC: ????h     Err: ?\n");
    con_put(" Stat: IDLE\n");
    
    /*Draw settings menu*/
    con_gotoxy(0, 15);
    con_put("Dump settings\n\n");
    con_put("Start:  ????h   Bank:  ??h\n");
    con_put("  End:  ????h   Bank:  ??h\n");
    con_put("Media:  ?????\n\n");
    con_gotoxy(13,21);
    con_put("START");
}

void main(){
    init();
    crc16_xmodem_update(0,0,0);
    while(1){
        refresh_main_menu();
    }
}
