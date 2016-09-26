project(crc-libs C)

#Implementations of common CRC libraries. The 'bbb' version has a smaller
#ROM footprint at the cost of being slower.

#CRC16 CCITT
ADD_LIBRARY(crc16-ccitt crc16-ccitt.c crc16-ccitt.h)
ADD_LIBRARY(crc16-ccitt-bbb crc16-ccitt-bbb.c crc16-ccitt.h)

#CRC16 as used by XMODEM
ADD_LIBRARY(crc16-xmodem crc16-xmodem.c crc16-xmodem.h)
ADD_LIBRARY(crc16-xmodem-bbb crc16-xmodem-bbb.c crc16-xmodem.h)
