/*
 * UART0 Peripheral
 */
.section text
uart0_init:
  M_MOV32 R0, UART0_BASE_ADDR

  MOV  R1, #0x80 /* Enable Baud Config */
  STR  R1, [R0, #UART0_LCR_OFFSET]

  MOV  R1, #0x00 /* 115200 Baud Rate High Latch */
  STR  R1, [R0, #UART0_DLH_OFFSET]
  
  MOV  R1, #0x1A /* 115200 Baud Rate Low Latch */
  STR  R1, [R0, #UART0_DLL_OFFSET]

  MOV  R1, #0x03 /* 8N1 Format */
  STR  R1, [R0, #UART0_LCR_OFFSET]

  MOV  R1, #0x00 /* UART 16x Mode */
  STR  R1, [R0, #UART0_MDR1_OFFSET]

  BX   LR


uart0_read_byte:
  /* Returns byte in R0 */
  M_MOV32 R0, UART0_BASE_ADDR

1:
  LDR  R1, [R0, #UART0_LSR_UART_OFFSET]
  ANDS R1, R1, #0x01
  BNE  2f
  B    1b
2:
  LDR  R0, [R0, #UART0_RHR_OFFSET]
  BX   LR

uart0_write_byte:
  /* Expect byte in R0 */
  M_MOV32 R1, UART0_BASE_ADDR
  STR  R0, [R1, #UART0_THR_OFFSET]
1:
  LDR  R0, [R1, #UART0_LSR_UART_OFFSET]
  ANDS R0, R0, #0x40
  BNE  2f
  B    1b
2:
  BX   LR
