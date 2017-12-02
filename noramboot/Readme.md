
# N0RAM Bootloader

A project to check if I can make a reimplementation of the regular bootloader
implemented on the replacement BIOS ROM with the following limitation. Use Zero
RAM bytes so (almost?) all of the console RAM is available to bootload ROMs.

N0RAM will implement two modes of uploading a ROM, regular UART so anyone with
a cheap chineesium UART-to-USB adapter can half-reliably upload a ROM. And
synchronous serial, a faster, more reliable protocol that requires some custom
circuitry to work.

# UART/XMODEM operation

Connect UART transceiver on the CONTROL 2 port as follows. 5V connections is
most likely not needed unless you need to power up the transceiver from the
console side (for instance if using a MAX232-based transceiver).

	   CONTROL 2
	   o o o o o-- Vcc (5 Volts)
		o o o o
		  | | `-- RX (PC TX)
		  | `---- Ground
		  `------ TX (PC RX)

Once connected and before running the bootloader, prepare the XMODEM transfer
with the serial port configuration 8/N/1 at ??? bps. XMODEM program must 
support CRC mode since the bootloader won't accept the original SUM mode.

At the end of the transmission the ROM will be immediatly chainloaded.

(Note: CRC might be ditched if it not feasible to implement CRC mode with the
zero RAM usage limitation)

# Synchronous serial operation

Protocol and helper hardware in development! Pinout most likely will match the
following diagram. RX/TX are signals from the Master System POV as with the
UART mode diagram.

	   CONTROL 2
		 ,------- RX Data 1 (Nibble mode)
		 | ,------- RX Data 2 (Nibble mode)
		 | | ,--- RX Data 3 (Nibble mode)
	   o o o o o--- Vcc (5 Volts)
	   |o o o o
	   || | | `-- TX Clock
	   || | `------ Ground
	   || `------ TX Data
	   |`---------- RX Clock
	   `--------- RX Data 0 (Serial/Nibble mode)

## Protocol description

Basically two serial lines with a dedicated clock signal so transmission is
both faster and more reliable. No need to do UART framing! Do a better worded
explanation when it is done.