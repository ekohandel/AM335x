/*
 * IO mapping module
 */
.text
io_init:
  CALL io_uart0_init
  BX   LR


io_uart0_init:
  M_MOV32 R0, CONTROL_BASE_ADDR

  MOV  R1, #0x08  /* Fast output without PU or PD */
  STR  R1, [R0, #CONTROL_CONF_UART0_TXD_OFFSET]

  MOV  R1, #0x30  /* Fast input with PU */
  STR  R1, [R0, #CONTROL_CONF_UART0_RXD_OFFSET]

  BX   LR
