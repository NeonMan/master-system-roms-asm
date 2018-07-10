
    UART_DOWN = 0xBB
    UART_UP   = 0xFB
    IO_PORT   = 0x3F
	
	READ_PORT = 0xDD

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
	;	Uses: AF, D
	.macro serial_read_inner ?start_bit_loop, ?start_bit_success, ?uart_end
		LD D, #0x00
		start_bit_loop:
			IN A, (#READ_PORT)        ;
			RLC A                     ; Read line status into Carry bit
			
			JR NC, start_bit_success  ; If low, try reading byte
			
			DEC D                     ;
			JR NZ, start_bit_loop     ; Otherwise retry 
			XOR A, A                  ; -OR-
			JP uart_end               ; Timeout; return with error
		
		start_bit_success:
			LD A, #0x55
			SCF         ;Say we detected a 0x55
		
		uart_end:
	.endm
	
	;Simulates an XMODEM transmission.
	;
	;	Uses: AF
	;		HL' Number of packets
	;		 B' Current packet
	;		 C' Current byte
	.macro serial_read_sim
		EXX
		
		EXX
		AND A, A  ;Clear C
		CCF       ;Set C (success)
	.endm
	
	;Prepares shadow registers for use with serial_read_sim
	.macro serial_setup_sim
		EXX
		LD HL, #0x10
		XOR A, A
		LD B, A
		LD C, A
		EXX
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
		;serial_read_sim
	.endm