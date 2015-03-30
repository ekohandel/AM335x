/*
 * Address of registers
 */

/************************** CM WKUP Registers *********************************/
.set CM_WKUP_BASE_ADDR,             0x44E00400
.set CM_WKUP_UART0_CLKCTRL_OFFSET,  0x000000B4


/************************** Control Registers *********************************/
.set CONTROL_BASE_ADDR,             0x44E10000
.set CONTROL_CONF_UART0_RXD_OFFSET, 0x00000970
.set CONTROL_CONF_UART0_TXD_OFFSET, 0x00000974

/************************** UART0 Registers **********************************/
.set UART0_BASE_ADDR,               0x44E09000
.set UART0_THR_OFFSET,              0x00000000
.set UART0_RHR_OFFSET,              0x00000000
.set UART0_DLL_OFFSET,              0x00000000
.set UART0_DLH_OFFSET,              0x00000004
.set UART0_LCR_OFFSET,              0x0000000C
.set UART0_LSR_UART_OFFSET,         0x00000014
.set UART0_MDR1_OFFSET,             0x00000020
