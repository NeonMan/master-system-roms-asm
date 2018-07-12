	;Waits for one bit width
	.macro tx_bit_delay
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
		
		NOP
	.endm
	
	.macro rx_bit_extra_delay
		CP (HL)
		CP (HL)
		CP (HL)
	.endm
	
	.macro half_bit_delay
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
		CP (HL)
		CP (HL)
	.endm

    UART_DOWN = 0xBB
    UART_UP   = 0xFB
    IO_PORT   = 0x3F
	
	READ_PORT = 0xDD
	
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
		JR C, bit_end  ;<-- This jump always fails. Equalizes bit width
		EX AF, AF'
		LD A, #UART_DOWN
		JR bit_end
		
		bit_end:
		OUT (#IO_PORT),A
		EX AF, AF'
		tx_bit_delay
	.endm

	;Sends byte stored on A through serial port.
	;	Uses: AF
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
			IN A, (#READ_PORT)
			RLC A
			RR D
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
	;	Uses: AF, HL'
	.macro serial_read_inner ?start_bit_loop, ?start_bit_success, ?uart_end
		EXX
		LD HL, #0x07FF ;Retry count
		;Try detecting a start bit
		start_bit_loop:
			IN A, (#READ_PORT)        ;
			RLC A                     ; Read line status into Carry bit
			JR NC, start_bit_success  ; If low, try reading byte
			
			DEC HL                    ;
			LD A, H                   ;
			AND A, A                  ;
			JR NZ, start_bit_loop     ; H is zero, so is A
			CP A, L                   ; If L is then nonzero
			JR NZ, start_bit_loop     ; Retry 
		EXX
		XOR A, A                  ; Otherwise
		JP uart_end               ; Timeout; return with error
		
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
			LD A, D
			SCF
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