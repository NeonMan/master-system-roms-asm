
=== Z80 Dataset ROMs ===

This romset is intended to generate a full dataset of all the 8bit ALU
operations of the Z80. It will perform the operation `OP A, B` where OP
is one of ADD, ADC, SUB, SBC, CP, AND, OR, XOR. If relevant the FLAG register
will be tested in any of the required staes.

=== Serial port connections ===

You need to connect an UART transceiver on the SMS `CONTROL 2` port as shown
on the diagram below. Transceiver input must be 5V tolerant.

         CONTROL 2
         o o o o o--Vcc (5V, NOT required most times)
          o o o o
            | | `---PC TX (SMS RX)
            | `-----Ground
            `-------PC RX (SMS TX)

Serial port configuration is 4800bps, 8 data bits, 1 stop bit, no flow control.
