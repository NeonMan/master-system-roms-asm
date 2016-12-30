#ifndef X_BOOTLOADER_H
#define X_BOOTLOADER_H

#include <stdint.h>

#define BOOT_READY 0
#define BOOT_ERROR 255

uint8_t xboot_download();
void    xboot_launch();

#endif
