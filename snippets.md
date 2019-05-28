# Assembler snippets

Some z80 assembler snippets.

## Indirect call

Combine `CALL` and `JP (HL)` to make a `CALL (HL)` equivalent.

    .macro CALL_HLp ?ind_jump, ?ind_call
            JR ind_call
        ind_jump:
            JP (HL)
        ind_call:
            CALL ind_jump
    .endm

## Position-independent call

Call any code "as if" it was loaded at address zero.

    ; --- At caller ---
    PUSH HL    ;
    PUSH DE    ; Optionally save HL/DE
    
    PUSH nn    ; Push Target Address
    PUSH nn    ; Push Expected return
    AND  A     ; Clear carry (?)
    RST #10    ; Push Real return
    
    ; --- At RST vector (Uses HL and DE) ---
    POP  DE       ; Get Real return
    POP  HL       ; Get Expected return
    SBC  HL, DE   ; HL holds -offset
    EX   DE, HL   ; DE holds -offset; HL holds real return
    
    EX   (SP), HL ; Stack points to unfixed target; Exchange with real return
    AND A         ; Clear carry (?)
    SBC  HL, DE   ; "Add" offset
    JP (HL)       ; Stack holds real return; HL fixed jump target. Jump
    
    
    ; --- At Callee (HL; DE are dirty) ---
    POP DE    ;
    POP HL    ; Optionally restore HL/DE
    RET