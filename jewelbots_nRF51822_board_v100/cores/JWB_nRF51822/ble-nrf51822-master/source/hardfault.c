#include "nordic_common.h"
#include "nrf_log.h"

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

  NRF_LOG("[Hard fault handler]\r\n");
  NRF_LOG_PRINTF("R0   = %x\r\n", stacked_r0);
  NRF_LOG_PRINTF("R1   = %x\r\n", stacked_r1);
  NRF_LOG_PRINTF("R2   = %x\r\n", stacked_r2);
  NRF_LOG_PRINTF("R3   = %x\r\n", stacked_r3);
  NRF_LOG_PRINTF("R12   = %x\r\n", stacked_r12);
  NRF_LOG_PRINTF("Stacked LR  = %x\r\n", stacked_lr);
  NRF_LOG_PRINTF("Stacked PC   = %x\r\n", stacked_pc);
  NRF_LOG_PRINTF("Stacked PSR   = %x\r\n", stacked_psr);
  NRF_LOG_PRINTF("Current LR   = %x\r\n", lr_value);
  UNUSED_PARAMETER(stacked_r0);
  UNUSED_PARAMETER(stacked_r1);
  UNUSED_PARAMETER(stacked_r2);
  UNUSED_PARAMETER(stacked_r3);
  UNUSED_PARAMETER(stacked_r12);
  UNUSED_PARAMETER(stacked_lr);
  UNUSED_PARAMETER(stacked_pc);
  UNUSED_PARAMETER(stacked_psr);
  __asm volatile("bkpt 10");
  while (1)
    ; // todo: replace while
}
// clang-format off
__asm void HardFault_Handler(void)
{
	ALIGN 
	MOVS 	r0, #4
	MOV 	r1, LR
	TST 	r0, r1
	BEQ		stacking_used_MSP
	MRS		R0, PSP ;First parameter -stacking was using PSP
	B			get_LR_and_branch
stacking_used_MSP
	MRS	R0, MSP ; first parameter - stacking was using MSP
get_LR_and_branch	
	MOV		R1, LR
	LDR		R2,=__cpp(hard_fault_handler_c)
	BX		R2
}
// clang-format on
