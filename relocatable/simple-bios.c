/**
 * @file  test-roms/simple-bios.c
 * @brief Most simple BIOS, will switch to cartridge ROM.
 *
 * This BIOS requires to be loaded at the RAM base address. This can be
 * accomplished automagically by linking against the Crt0-ram.
 */

#include <stdint.h>
#include "sms/io.h"
#include "sms/mapper.h"
#include "sms/intv-dummy.h"

/** Makes a jump to the chainload code.
 *
 *  @param address reset verctor address, 0x0000 for regular ROMS
 *  @param io      value of the media selector register.
 */
void boot_call(uint16_t address, uint8_t io){
  void (*chainload_entry) (void) = (void*)address;
  __io_chip = io;
  chainload_entry();
}

/** ROM entry point.*/
void main(){  
  /* --- Configure IO chip --- */
  io_set(IO_ENABLE_BIOS & IO_ENABLE_RAM & UNDEFINED_BIT_MASK);
  
  /* --- Do stuff --- */
  
  /* Chainload a ROM cartridge */
  boot_call(0x0000, 0xA8);
  
  while(1){
    
  }
}
