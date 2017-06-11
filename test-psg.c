/**
 * @file  test-roms/psg-test.c
 * @brief Play simple sounds on the PSG.
 *
 * Plays four notes on channel 0 in sequence, no interrupts needed.
 */

#include <stdint.h>

#include "sms/intv-dummy.h"
#include "sms/sdsc.h"
#include "sms/console.h"
#include "sms/sms.h"

static void delay_loop(uint16_t iter){
    volatile uint16_t i;
    while(iter--){
        i = 0x0FFF;
        while(i--){
            i = i;
        }
    }
}

void print(const char* str){
    sdsc_puts(str);
    con_put(str);
}

void main(){
    uint16_t tones[4] = {
        0b0111111110,
        0b0101111110,
        0b0011111110,
        0b0010111110
    };
    uint8_t i = 0;
    con_init();
    print("PSG test.\n\n");
    
    /*Initialize PSG*/
    /*Set tone for channel 1*/
    sms_psg_port = 0b10000000 | (tones[i] & 0b00001111);
    sms_psg_port = 0b00000000 | ((tones[i] >> 4) & 0b00111111);
    
    /*Set all channels to silence except the first*/
    sms_psg_port = 0b10010000;
    sms_psg_port = 0b10111111;
    sms_psg_port = 0b11011111;
    sms_psg_port = 0b11111111;
    
    while(1){
        i = (i + 1) % 4;
        delay_loop(0x000F);
        print(".");
        sms_psg_port = 0b10000000 | (tones[i] & 0b00001111);
        sms_psg_port = 0b00000000 | ((tones[i] >> 4) & 0b00111111);
    }
}