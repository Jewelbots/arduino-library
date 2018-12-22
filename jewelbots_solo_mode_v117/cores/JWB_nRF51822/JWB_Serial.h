#ifndef __JWB_SERIAL_H__
#define __JWB_SERIAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "Arduino.h"

void jwb_uart_write(const char * str);
void jwb_uart_write_many(int num_args, ...);
void jwb_uart_printf(const char * format_msg, ...);

#define JWB_SERIAL(...)  jwb_uart_write_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define JWB_SERIAL_PRINTF(...)  jwb_uart_printf(__VA_ARGS__)




#endif
