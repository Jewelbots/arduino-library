#ifndef HARDFAULT_H
#define HARDFAULT_H
#include <stdint.h>
void hard_fault_handler_c(unsigned int *hardfault_args, unsigned lr_value);
#endif
