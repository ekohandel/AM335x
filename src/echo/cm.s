/*
 * Clock Manager Module
 */ 
.text
cm_init:
  CALL cm_wkup_uart_init
  BX   LR

cm_wkup_uart_init:
  M_MOV32 R0, CM_WKUP_BASE_ADDR
  MOV     R1, #0x02   /* Fully Functional and Explicitly Enalbled */
  STR     R1, [R0, #CM_WKUP_UART0_CLKCTRL_OFFSET]
  BX      LR
