/**
 * @file  test-roms/test-interrupt.c
 * @brief checks whether the INT line works.
 *
 * Prints a dot on SDSC for every received INT.
 */

#include <stdint.h>
#include "sms/sdsc.h"
#include "sms/intv.h"
#include "sms/intv-dummy-rst.h"

void nmi(){
    sdsc_putc('N');
}

void int1(){
    sdsc_putc('I');
}

void main(){
	__asm
	IM 1
	EI
	__endasm;
    while(1){
        
    }
}
