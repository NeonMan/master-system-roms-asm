	;Waits for one bit width (19200bps, 143T)
	;  Note: the TX/RX code has overhead which
	;  has been subtracted from this bith width
	.macro tx_bit_delay_19200
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T

		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T

		NOP     ;4T
	.endm


	;Waits for one bit width (14400bps, 192.67T)
	;  Note: the TX/RX code has overhead which
	;  has been subtracted from this bith width
	.macro tx_bit_delay_14400
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T

		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T

		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		;CP (HL) ;7*7  49T
		;NOP

	.endm


	;Waits for one bit width (9600bps)
	;  Note: the TX/RX code has overhead which
	;  has been subtracted from this bith width
	.macro tx_bit_delay_9600
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
		
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
		
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
		
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
		
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		NOP     ;7*4 +4 32T
		; Total --> 312T
	.endm

	.macro half_bit_delay_19200
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
	.endm

	.macro half_bit_delay_14400
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T

		CP (HL)
		CP (HL)
		CP (HL) ;7*3  21T
			; --> 91T
	.endm
	
	.macro half_bit_delay_9600
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
		
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL)
		CP (HL) ;7*10 70T
		
		CP (HL)
		CP (HL) ;7*2  14T
		; Total --> 154T
	.endm

	; --- Select baudrate ---

	;Bit width
	.macro tx_bit_delay
		tx_bit_delay_14400
	.endm

	;Half bit width
	;  While half a bit period would aim to sample at the
	;  middle of a bit, making this macro shorter might allow
	;  some extra leeway on the stop bit at higher bitrates.
	.macro half_bit_delay
		half_bit_delay_14400
	.endm



	;Correct the Overhead between RX/TX routines
	;62T - 36T = 26T
	.macro rx_bit_extra_delay
		CP (HL)
		CP (HL)
		CP (HL)
		NOP     ;3*7 +4 25T
	.endm
	
    UART_DOWN = 0xBB
    UART_UP   = 0xFB
    IO_PORT   = 0x3F
	
	READ_PORT = 0xDD
	
	;Send a single bit.
	;This will add an overhead of 62 T-States
	;	Uses: AF, AF'
	.macro tx_send_bit ?bit_set ?bit_clr ?bit_end
		RRCA
		JR C, bit_set
		JR NC, bit_clr
		
		bit_set:
		JR NC, bit_end ;<-- This jump always fails. Equalizes bit width
		EX AF, AF'
		LD A, #UART_UP
		JR bit_end
		
		bit_clr:
		EX AF, AF'
		LD A, #UART_DOWN
		JR bit_end
		
		bit_end:
		OUT (#IO_PORT),A
		EX AF, AF'
		tx_bit_delay
	.endm

	;Sends byte stored on A through serial port.
	;	Uses: AF, AF'
	.macro serial_write_inner
		;Start bit
		EX AF, AF'
		LD A, #UART_DOWN      ;  7T
		OUT (#IO_PORT),A      ; 11T
		EX AF, AF'
		tx_bit_delay
		
		;Data bits
		tx_send_bit
		tx_send_bit
		tx_send_bit
		tx_send_bit
		
		tx_send_bit
		tx_send_bit
		tx_send_bit
		tx_send_bit
		
		;Stop bit (ignore, just send line up)
		BIT #0, 0(IX)
		BIT #0, 0(IX)
		
		LD A, #UART_UP        ;  7T
		OUT (#IO_PORT),A      ; 11T
	.endm

	;Samples a bit, shifts result on D register MSB, waits for a bit
	;    Uses: AF, D
	.macro serial_read_bit
			IN A, (#READ_PORT) ; 11T
			RLC A              ; 8T
			RR D               ; 8T
			NOP                ; 4T
			NOP                ; 4T -> 36T
			
			; Sampling test code
			;LD A, #UART_DOWN  ; 7T
			;OUT (#IO_PORT),A  ; 11T
			;LD A, #UART_UP    ; 7T
			;OUT (#IO_PORT),A  ; 11T  -> 36T
			
			tx_bit_delay
			rx_bit_extra_delay
	.endm
	
	;Reads byte from serial port.
	;	On Success:
	;		- Stores result on A
	;		- Carry flag is SET
	;	On error:
	;		- Error code on A
	;		- Carry flag is CLEARED
	;
	;	Error codes:
	;		0: Timeout
	;
	;	Uses: AF, D, HL'
	.macro serial_read_inner ?start_bit_loop, ?start_bit_success, ?uart_end
		EXX
		LD HL, #0x07FF ;Retry count
		;Try detecting a start bit
		start_bit_loop:
			IN A, (#READ_PORT)        ;
			RLC A                     ; Read line status into Carry bit
			JR NC, start_bit_success  ; If low, try reading byte
			
			DEC HL                    ;
			LD A, H                   ; H is zero, so is A
			OR A, L                   ; If L is then nonzero
			JR NZ, start_bit_loop     ; Retry
		EXX
		XOR A, A                  ; Otherwise:
		JP uart_end               ; Return error, (Clear C)
		
		;Start bit detected, time to read a byte.
		start_bit_success:
			EXX
			LD D, #0x00
			;Wait a whole bit so we get inside the LSB
			tx_bit_delay
			;Wait half a bit so we sample near the middle of a bit
			half_bit_delay
			
			serial_read_bit
			serial_read_bit
			serial_read_bit
			serial_read_bit
			
			serial_read_bit
			serial_read_bit
			serial_read_bit
			serial_read_bit
			
			;We shall be lying above the STOP bit
			;Load result on A and return success
			XOR A, A
			CCF
			LD A, D
		uart_end:
	.endm
	
	;Actual macros that will be called
	.macro serial_setup
		;serial_setup
		;serial_setup_sim
	.endm
	
	.macro serial_write
		serial_write_inner
	.endm
	
	.macro serial_read
		serial_read_inner
	.endm
