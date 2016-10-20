#include "SEGGER_RTT.h"

void hard_fault_handler_c(unsigned int *hardfault_args, unsigned lr_value) {
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;

  stacked_r0 = ((unsigned long)hardfault_args[0]);
  stacked_r1 = ((unsigned long)hardfault_args[1]);
  stacked_r2 = ((unsigned long)hardfault_args[2]);
  stacked_r3 = ((unsigned long)hardfault_args[3]);
  stacked_r12 = ((unsigned long)hardfault_args[4]);
  stacked_lr = ((unsigned long)hardfault_args[5]);
  stacked_pc = ((unsigned long)hardfault_args[6]);
  stacked_psr = ((unsigned long)hardfault_args[7]);

  SEGGER_RTT_printf(0, "[Hard fault handler]\n");
  SEGGER_RTT_printf(0, "R0   = %x\n", stacked_r0);
  SEGGER_RTT_printf(0, "R1   = %x\n", stacked_r1);
  SEGGER_RTT_printf(0, "R2   = %x\n", stacked_r2);
  SEGGER_RTT_printf(0, "R3   = %x\n", stacked_r3);
  SEGGER_RTT_printf(0, "R12   = %x\n", stacked_r12);
  SEGGER_RTT_printf(0, "Stacked LR  = %x\n", stacked_lr);
  SEGGER_RTT_printf(0, "Stacked PC   = %x\n", stacked_pc);
  SEGGER_RTT_printf(0, "Stacked PSR   = %x\n", stacked_psr);
  SEGGER_RTT_printf(0, "Current LR   = %x\n", lr_value);

  while (1)
    ;
}
void HardFault_Handler(void) {
  __asm volatile(
      "    MOVS     r0, #4\n"
      "    MOV     r1, LR\n"
      "    TST     r0, r1\n"
      "    BEQ        stacking_used_MSP\n"
      "    MRS        R0, PSP \n"
      "    B            get_LR_and_branch\n"
      "stacking_used_MSP:   \n"
      "    MRS    R0, MSP   \n"
      "get_LR_and_branch:   \n"
      "    MOV        R1, LR\n"
      "		 ldr 				r2, "
      "handler2_address_const                        \n"
      " 	 bx 				r2                      "
      "                          \n"
      "handler2_address_const: .word hard_fault_handler_c		"
      "						\n");
}
