#ifndef __UART_H
#define __UART_H

#include <stdint.h>

/**Stores the result of an uart_getc operation.*/
extern uint8_t uart_result;
/**Stores the result status of an uart_getc operation.*/
extern uint8_t uart_status;

#define UART_STATUS_OK  0x00
#define UART_STATUS_NOK 0xFF

/**Samples serial line and receives a byte.
 *
 * Sampled byte is stored on uart_result and uart_status stores the success
 * or not of the operation. A UART_STATUS_OK on the status means a byte has
 * been detected and has been correctly sampled, UART_STATUS_NOK usually
 * means a framing error has happened.
 */
void    uart_getc();

/**Returns uart operation status.
 *
 * @returns Value of uart_status.
 */
uint8_t uart_get_status();

/**Sends a byte through the serial line.
 * This operation always succeeds.
 *
 * @param c Byte to send.
 */
void    uart_putc(uint8_t c);

/**Sends a byte through the serial line returning at the stop bit.
 *
 * Same purpose as uart_putc but will return just after ssetting the STOP bit
 * so further processing can be done during this time instead of idling.
 * This operation always succeeds.
 *
 * @param c Byte to send.
 */
void uart_putc_fast(uint8_t c);

/**Sends a byte through the serial line.
 *
 * Same purpose as uart_putc but will transmit the bits as fast as the console
 * will allow, without trying to adhere to any standard baudrate.
 * This operation always succeeds.
 *
 * @param c Byte to send.
 */
void uart_putc_faster(uint8_t c);

/** Helper macro to make fast reads into buffers.*/
#define UART_BUFF_GETC(BUFF, IDX) do{ uart_getc(); }while(uart_status != UART_STATUS_OK); BUFF[IDX] = uart_result;

#endif
