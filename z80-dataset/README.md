
Z80 Dataset ROMs
================

This romset is intended to generate a full dataset of all the 8bit ALU
operations of the Z80. It will perform the operation `OP A, B` where OP
is one of ADD, ADC, SUB, SBC, CP, AND, OR, XOR. If relevant the FLAG register
will be tested in any of the required staes. `NEG` dataset is also provided.

CSV format
==========

The data dumped through the serial port has 256 columns and a multiple of 256
rows. Details are available on the CSV files themselves but this is common for
all of them.

On any row like this, you will have the HEX value of having performed an
operation. In this case `ADD A, B` where `A = 01h` incrementing by one for each
row and `B` increments from zero to 255 from left to right.

    0100,0200,0300,0400,0500,0600, [...] FCA8,FDA8,FEA8,FFA8,0051,
    0200,0300,0400,0500,0600,0700, [...] FDA8,FEA8,FFA8,0051,0111,

Each entry is the value of the `AF` register after performing the operation
where `A` is the most significant byte and `F` is the least significant byte.

There will be many 255x255 datasets per CSV, usually the only difference is the
value of the `F` register before performing the operation, useful for Carry ops
and to test wether a bit is left intact during the operation.

Serial port connections
=======================

You need to connect an UART transceiver on the SMS `CONTROL 2` port as shown
on the diagram below. Transceiver input must be 5V tolerant.

         CONTROL 2
         o o o o o--Vcc (5V, NOT required most times)
          o o o o
            | | `---PC TX (SMS RX)
            | `-----Ground
            `-------PC RX (SMS TX)

Serial port configuration is 4800bps, 8 data bits, 1 stop bit, no flow control.
