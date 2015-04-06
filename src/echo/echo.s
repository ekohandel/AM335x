/* A File to write echo on the serial port */
.include "macros.s"
.include "regs.s"
.include "cm.s"
.include "io.s"
.include "uart0.s"

.section reset
.global _start
_start:

  M_MOV32 R13, 0x402F0800

  CALL cm_init
  CALL io_init
  CALL uart0_init

1:
  CALL uart0_read_byte
  CALL uart0_write_byte
  B 1b
