#include <sms/intv-dummy.h>
/*#include <sms/console.h>*/
#include <sms/uart.h>
#include <sms/io.h>
#include <stdint.h>

static const char hello[] = "\nMaster System boot ROM\n";

static void delay_loop(){
    volatile uint16_t i;
    for(i=0; i<10000; i++){
        i = i;
    }
}

static void print(const char* str){
#ifdef __CONSOLE_H
    con_put(str);
#endif
    
#ifdef __UART_H
    while(*str){
        if((*str)=='\n'){
            uart_putc('\r');
        }
        uart_putc(*str);
        str++;
    }
#endif
}

static uint8_t detect_rom(){
    uint8_t* rom = (uint8_t*) 0x0000;
    uint8_t difference_count;
    uint8_t i;
    
    difference_count = 0;
    for(i=0; i<128; i++){
        if(rom[0] != rom[1]){
            difference_count++;
        }
        rom++;
    }
    
    if(difference_count>16){
        return 1;
    }
    else{
        return 0;
    }
}

static void boot(){
    void (*rst_vector)(void) = (void*) 0x0000;
    uint8_t *ram_base = (void*) 0xC000;
    print("Booting\n");
    
    /*Store current IO chip value to RAM base*/
    *ram_base = io_get();
    
    /*Boot the ROM*/
    rst_vector();
}

static void boot_xmodem(){
    print("  XMODEM........... ");
    
    print("Failed\n");
}

static void boot_ext(){
    /*Enable EXPANSION*/
    io_enable(IO_ENABLE_EXPANSION);
    print("  Expansion port... ");
    if(detect_rom()){
        boot();
    }

    print("Failed\n");
    /*Disable CARD*/
    io_disable(IO_DISABLE_EXPANSION);
}

static void boot_card(){
    /*Enable CARD*/
    io_enable(IO_ENABLE_CARD);
    print("  Card............. ");
    if(detect_rom()){
        boot();
    }

    print("Failed\n");
    /*Disable CARD*/
    io_disable(IO_DISABLE_CARD);
}

static void boot_cart(){
    /*Enable CARTRIDGE*/
    io_enable(IO_ENABLE_CARTRIDGE);
    print("  Cartridge........ ");
    if(detect_rom()){
        boot();
    }
    
    print("Failed\n");
    /*Disable CARTRIDGE*/
    io_disable(IO_DISABLE_CARTRIDGE);
}

static void boot_bios(){
    /*Enable BIOS*/
    io_enable(IO_ENABLE_BIOS);
    if(detect_rom()){ boot(); }
    
    print("BIOS not found!\n");
    /*Disable BIOS*/
    io_disable(IO_DISABLE_BIOS);
}

void main(){
    delay_loop();
#ifdef __CONSOLE_H
    con_init();
#endif
    
    /*Say hello*/
    print(hello);
    
    /*Deactivate all ROMs*/
    io_set(IO_ENABLE_RAM & IO_ENABLE_PERIPHERAL);
    
    /*Try booting ROMs*/
    boot_xmodem();
    boot_ext();
    boot_card();
    boot_cart();
    
    print("No ROM found! Booting BIOS.\n");
    boot_bios();
    
    while(1){
        
    }
}
