/**
 * \file crc16-xmodem.c
 * Functions and types for CRC checks.
 *
 * Generated on Mon Sep 26 10:47:31 2016,
 * by pycrc v0.9, https://pycrc.org
 * using the configuration:
 *    Width         = 16
 *    Poly          = 0x1021
 *    Xor_In        = 0x0000
 *    ReflectIn     = False
 *    Xor_Out       = 0x0000
 *    ReflectOut    = False
 *    Algorithm     = bit-by-bit-fast
 *****************************************************************************/
#include "crc16-xmodem.h"     /* include the header file generated with pycrc */
#include <stdint.h>

/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc16_xmodem_update(crc_t crc, const void *data, uint16_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int i;
    int bit;
    unsigned char c;

    while (data_len--) {
        c = *d++;
        for (i = 0x80; i > 0; i >>= 1) {
            bit = !!(crc & 0x8000);
            if (c & i) {
                bit = !bit;
            }
            crc <<= 1;
            if (bit) {
                crc ^= 0x1021;
            }
        }
    }
    return crc;
}
