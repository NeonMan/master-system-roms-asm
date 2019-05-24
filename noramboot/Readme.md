
# N0RAM Bootloader

A project to check if I can make a reimplementation of the regular bootloader
implemented on the replacement BIOS ROM with the following limitation. Use Zero
RAM bytes so (almost?) all of the console RAM is available to bootload ROMs.

N0RAM implements an XMODEM client so anyone with a cheap chineesium UART-to-USB
adapter can half-reliably upload a ROM.

# UART/XMODEM operation

Connect UART transceiver on the CONTROL 2 port as follows. 5V connections is
most likely not needed unless you need to power up the transceiver from the
console side (for instance if using a MAX232-based transceiver).

       CONTROL 2
       o o o o o-- Vcc (5 V; Usually not needed)
        o o o o
          | | `-- Console TX (PC RX)
          | `---- Ground
          `------ Console RX (PC TX)

Once connected and before running the bootloader, prepare the XMODEM transfer
with the serial port configuration 8/N/1 at 9600 bps. XMODEM program must 
support CRC mode since the bootloader won't accept the original SUM mode.

At any time the ROM can be reset by pressing the [PAUSE] button.

At the end of the transmission the ROM will be immediatly chainloaded.

# XMODEM software

Any XMODEM-CRC sofware should work, This ROM has been tested with [Teraterm][tt]
windows, on windows 10 using an off-the-shelf 5V USB-to-UART converter.

# ROM sizes and VRAM uploading

On an XMODEM transmission, the first 8K of data will be loaded at the RAM
base address `0xC000` and, if available, any further packets up to 16KB of
additional data will be stored on the VDP VRAM, giving a maximum upload
size of 24KB.

    
# Upload file format
    
    Offset   Description
    0x0000 +-------------+
           | Program ROM |
    0x1FFF +-------------+  <-- Up to 8K loaded to RAM
    0x2000 +-------------+  <-- If >8K, data is loaded to VRAM
           |             |
           |  VRAM data  |
           |             |
    0x5FFF +-------------+  <-- Up to a maximum of 24K

# Feedback

Feel free to drop a line at `root@heavydeck.net` for feedback or anything that
crosses your mind. More projects and stuff at my site! [heavydeck.net][site]

# Version history

* `V1.0` Working transmission, 4800bps
* `V1.1` Performance upgrade to 9600bps
* `V1.2` Added support for VRAM uploads

[tt]: https://ttssh2.osdn.jp/index.html.en
[site]: http://heavydeck.net/
