#include <sms/console.h>
#include <stdint.h>

#define CANARY_SAFE_RETURN
#include <sms/canary.h>

static void delay_loop(){
    volatile uint16_t i;
    for(i=0; i<25000; i++){
        i = i;
    }
}

/*This function will trigger a stack overflow past the canary address*/
/*with safe returns, before returninf from every function the canary */
/*state will be checked and console reset if the canary dies.        */
static uint8_t trigger_canary(uint8_t dummy_param){
    volatile uint8_t dummy = dummy_param;
    return trigger_canary(dummy_param + 1);
}

void main(){
    int8_t i;
    canary_setup();
    con_init();
    con_put("Canary test.\n");
    con_put("Reset in: ");
    for(i=0; i<5; i++){
        con_putc('5' - i);
        con_putc(' ');
        delay_loop();
    }
    trigger_canary(1);
}
