/*
 * Macros to help with the assembly
 */
.macro M_MOV32 reg, imm16
  MOVW \reg, #(\imm16 & 0xFFFF)
  MOVT \reg, #(\imm16 >> 16) 
.endm

.macro CALL label
  PUSH {LR}
  BL   \label
  POP  {LR}
.endm
