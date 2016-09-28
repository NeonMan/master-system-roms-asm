#ifndef __CANARY_H
#define __CANARY_H

static const unsigned char canary_value[2] = {0x16, 0xe9};

#ifndef CANARY_ADDRESS
extern unsigned char* crt0_canary;
#else
#define crt0_canary ((unsigned char*) CANARY_ADDRESS)
#endif

#ifdef CANARY_SAFE_RETURN
#define return canary_check(); return
#endif

static void canary_check(){
    if(
        (crt0_canary[0] != canary_value[0]) ||
        (crt0_canary[1] != canary_value[1])
    ){
        /*Canary died. Reset the console*/
        __asm
        JP 0x0000
        __endasm;
    }
}

static void canary_setup(){
    crt0_canary[0] = canary_value[0];
    crt0_canary[1] = canary_value[1];
}

#endif
