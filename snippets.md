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
