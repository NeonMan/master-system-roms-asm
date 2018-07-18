
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

# Feedback

Feel free to drop a line at `root@heavydeck.net` for feedback or anything that
crosses your mind. More projects and stuff at my site! [heavydeck.net][site]

[tt]: https://ttssh2.osdn.jp/index.html.en
[site]: http://heavydeck.net/
